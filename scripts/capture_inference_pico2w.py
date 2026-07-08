#!/usr/bin/env python3
"""
Capture Pico 2W serial output (4 LD2412 sensors) to a log file
and stream to a Web UI with real-time per-sensor gate charts.

Usage: python3 capture_inference_pico2w.py [port] [baudrate] [output_file]
Example: python3 capture_inference_pico2w.py /dev/cu.usbmodem14201 115200
"""

import serial
import socket
import sys
from datetime import datetime
import os
import asyncio
import threading
import glob
import time
import uvicorn
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse
import json
from contextlib import asynccontextmanager

# Global variables to share data between serial thread and web server
serial_clients = set()
running = True

port_arg = None
baudrate_arg = 115200
output_file_arg = None

@asynccontextmanager
async def lifespan(app: FastAPI):
    global running
    loop = asyncio.get_running_loop()
    serial_thread = threading.Thread(
        target=serial_capture_thread,
        args=(port_arg, baudrate_arg, output_file_arg, loop),
        daemon=True
    )
    serial_thread.start()
    yield
    running = False

app = FastAPI(lifespan=lifespan)

def find_serial_port():
    """Try to find Pico port automatically"""
    ports = (glob.glob('/dev/cu.usbmodem*') +
             glob.glob('/dev/tty.usbmodem*') +
             glob.glob('/dev/cu.usbserial*') +
             glob.glob('/dev/tty.usbserial*'))
    if ports:
        return ports[0]
    return None

def get_local_ip():
    """Get the local network IP address"""
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s.connect(("8.8.8.8", 80))
        ip = s.getsockname()[0]
        s.close()
        return ip
    except Exception:
        return "127.0.0.1"

def serial_capture_thread(port, baudrate, output_file, asyncio_loop):
    """Background thread to capture serial data, log it, and broadcast it"""
    global running
    try:
        ser = serial.Serial()
        ser.port = port
        ser.baudrate = baudrate
        ser.timeout = 0.1
        ser.open()
        ser.dtr = True
        ser.rts = True
        # DTR toggle for Pico 2W
        ser.dtr = False
        time.sleep(0.05)
        ser.dtr = True
    except serial.SerialException as e:
        print(f"\n❌ Serial Error: {e}")
        print(f"\nAvailable ports might be:")
        for p in glob.glob('/dev/cu.usb*') + glob.glob('/dev/tty.usb*'):
            print(f"  - {p}")
        os._exit(1)

    print(f"📡 Connected to {port} at {baudrate} baud...")
    print(f"📝 Logging to: {output_file}")

    with open(output_file, 'w', buffering=1) as f:
        header = f"=== Pico 2W Serial Log (4 Sensors) ===\n"
        header += f"Port: {port}\n"
        header += f"Baudrate: {baudrate}\n"
        header += f"Started: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n"
        header += "=" * 50 + "\n\n"
        f.write(header)
        print(header)

        while running:
            line = ser.readline()
            if line:
                try:
                    decoded_line = line.decode('utf-8', errors='replace').rstrip()
                    if decoded_line:
                        timestamp = datetime.now().strftime('%H:%M:%S.%f')[:-3]
                        log_line = f"[{timestamp}] {decoded_line}"

                        f.write(log_line + "\n")
                        print(log_line)

                        # Broadcast to UI (Filter out verbose raw_baseline to avoid flooding)
                        if serial_clients and "raw_baseline" not in decoded_line:
                            asyncio.run_coroutine_threadsafe(
                                broadcast_log(log_line),
                                asyncio_loop
                            )
                except Exception as e:
                    print(f"Error processing line: {e}")

    if ser.is_open:
        ser.close()

async def broadcast_log(message):
    """Send log line to all connected websocket clients"""
    if not serial_clients:
        return
    disconnected = set()
    for client in list(serial_clients):
        try:
            await client.send_text(message)
        except WebSocketDisconnect:
            disconnected.add(client)
        except Exception:
            disconnected.add(client)

    for client in disconnected:
        if client in serial_clients:
            serial_clients.remove(client)

HTML_PAGE = """
<!DOCTYPE html>
<html>
<head>
    <title>Pico 2W — 4-Sensor Monitor</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        * { box-sizing: border-box; margin: 0; padding: 0; }
        body {
            font-family: -apple-system, system-ui, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
            background: #0a0a0f;
            color: #e0e0e0;
            padding: 16px;
            min-height: 100vh;
        }
        header {
            display: flex; justify-content: space-between; align-items: center;
            border-bottom: 1px solid #222; padding-bottom: 12px; margin-bottom: 16px;
        }
        h1 { font-size: 1.4rem; color: #fff; }
        h1 span { color: #888; font-weight: 400; font-size: 0.9rem; }
        #status {
            padding: 4px 12px; border-radius: 4px; font-size: 0.85em;
            font-weight: 600; background: #ff9800; color: #000;
        }
        .connected { background: #4caf50 !important; color: #fff !important; }

        .main-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            grid-template-rows: auto auto;
            gap: 12px;
            margin-bottom: 12px;
        }
        @media (max-width: 900px) {
            .main-grid { grid-template-columns: 1fr; }
        }

        .sensor-card {
            background: #12121a;
            border: 1px solid #1e1e2e;
            border-radius: 10px;
            padding: 12px;
            position: relative;
            overflow: hidden;
        }
        .sensor-card::before {
            content: '';
            position: absolute; top: 0; left: 0; right: 0; height: 3px;
        }
        .sensor-card.s0::before { background: linear-gradient(90deg, #3b82f6, #60a5fa); }
        .sensor-card.s1::before { background: linear-gradient(90deg, #10b981, #34d399); }
        .sensor-card.s2::before { background: linear-gradient(90deg, #f59e0b, #fbbf24); }
        .sensor-card.s3::before { background: linear-gradient(90deg, #ef4444, #f87171); }

        .sensor-header {
            display: flex; justify-content: space-between; align-items: center;
            margin-bottom: 8px;
        }
        .sensor-title {
            font-size: 0.95rem; font-weight: 600;
        }
        .s0 .sensor-title { color: #60a5fa; }
        .s1 .sensor-title { color: #34d399; }
        .s2 .sensor-title { color: #fbbf24; }
        .s3 .sensor-title { color: #f87171; }

        .pred-badge {
            padding: 3px 10px; border-radius: 4px; font-size: 0.8rem;
            font-weight: 700; letter-spacing: 0.5px;
            background: #1a1a2e; color: #666;
        }
        .pred-badge.detected { background: #065f46; color: #6ee7b7; }
        .pred-badge.background-class { background: #1a1a2e; color: #888; }

        .chart-wrap {
            height: 180px;
            position: relative;
        }

        .bottom-row {
            display: flex; gap: 12px;
        }
        @media (max-width: 900px) { .bottom-row { flex-direction: column; } }

        .log-panel {
            flex: 1;
            background: #12121a; border: 1px solid #1e1e2e; border-radius: 10px;
            padding: 12px; display: flex; flex-direction: column;
            max-height: 300px;
        }
        .log-panel h3 { font-size: 0.9rem; margin-bottom: 8px; color: #888; }
        #console {
            flex: 1; background: #000; color: #00ff88; font-family: 'SF Mono', 'Fira Code', monospace;
            font-size: 11px; padding: 8px; overflow-y: auto; border-radius: 6px;
            border: 1px solid #1a1a2a; white-space: pre-wrap; word-wrap: break-word;
            line-height: 1.5;
        }
        .log-pred { color: #00bcd4; font-weight: bold; }
        .log-feat { color: #9c27b0; }
        .log-s0 { border-left: 2px solid #3b82f6; padding-left: 4px; }
        .log-s1 { border-left: 2px solid #10b981; padding-left: 4px; }
        .log-s2 { border-left: 2px solid #f59e0b; padding-left: 4px; }
        .log-s3 { border-left: 2px solid #ef4444; padding-left: 4px; }
    </style>
</head>
<body>
    <header>
        <h1>📡 Pico 2W Monitor <span>4 Sensors</span></h1>
        <div id="status">Connecting...</div>
    </header>

    <div class="main-grid">
        <div class="sensor-card s0">
            <div class="sensor-header">
                <div class="sensor-title">Sensor 0 — GP0/GP1</div>
                <div class="pred-badge" id="pred0">WAITING</div>
            </div>
            <div class="chart-wrap"><canvas id="chart0"></canvas></div>
        </div>
        <div class="sensor-card s1">
            <div class="sensor-header">
                <div class="sensor-title">Sensor 1 — GP4/GP5</div>
                <div class="pred-badge" id="pred1">WAITING</div>
            </div>
            <div class="chart-wrap"><canvas id="chart1"></canvas></div>
        </div>
        <div class="sensor-card s2">
            <div class="sensor-header">
                <div class="sensor-title">Sensor 2 — GP10/GP11</div>
                <div class="pred-badge" id="pred2">WAITING</div>
            </div>
            <div class="chart-wrap"><canvas id="chart2"></canvas></div>
        </div>
        <div class="sensor-card s3">
            <div class="sensor-header">
                <div class="sensor-title">Sensor 3 — GP14/GP15</div>
                <div class="pred-badge" id="pred3">WAITING</div>
            </div>
            <div class="chart-wrap"><canvas id="chart3"></canvas></div>
        </div>
    </div>

    <div class="bottom-row">
        <div class="log-panel">
            <h3>Raw Serial Output</h3>
            <div id="console"></div>
        </div>
    </div>

    <script>
        const wsProtocol = window.location.protocol === "https:" ? "wss:" : "ws:";
        const wsUrl = wsProtocol + "//" + window.location.host + "/ws";
        let ws;

        const statusEl = document.getElementById('status');
        const consoleEl = document.getElementById('console');

        Chart.defaults.color = '#555';
        Chart.defaults.borderColor = '#1a1a2a';

        const SENSOR_COLORS = [
            { mv: 'rgba(59, 130, 246, 0.8)',  st: 'rgba(59, 130, 246, 0.3)' },
            { mv: 'rgba(16, 185, 129, 0.8)', st: 'rgba(16, 185, 129, 0.3)' },
            { mv: 'rgba(245, 158, 11, 0.8)', st: 'rgba(245, 158, 11, 0.3)' },
            { mv: 'rgba(239, 68, 68, 0.8)',  st: 'rgba(239, 68, 68, 0.3)' }
        ];

        const charts = [];
        const sensorState = [];

        for (let s = 0; s < 4; s++) {
            sensorState.push({
                pred: 'WAITING', conf: 0, detected: false,
                mvVals: Array(14).fill(0), stVals: Array(14).fill(0)
            });

            const ctx = document.getElementById('chart' + s).getContext('2d');
            charts.push(new Chart(ctx, {
                type: 'bar',
                data: {
                    labels: Array.from({length: 14}, (_, i) => '' + i),
                    datasets: [
                        { label: 'Moving', data: Array(14).fill(0), backgroundColor: SENSOR_COLORS[s].mv, borderRadius: 2 },
                        { label: 'Stationary', data: Array(14).fill(0), backgroundColor: SENSOR_COLORS[s].st, borderRadius: 2 }
                    ]
                },
                options: {
                    responsive: true, maintainAspectRatio: false,
                    animation: { duration: 0 },
                    plugins: { legend: { display: true, position: 'top', labels: { boxWidth: 10, font: { size: 10 } } } },
                    scales: {
                        y: { min: 0, max: 100, ticks: { font: { size: 10 } }, grid: { color: '#111' } },
                        x: { ticks: { font: { size: 10 } }, grid: { color: '#111' } }
                    }
                }
            }));
        }

        function parseSensorIndex(line) {
            // Lines from human_vehicle_pico2w: "S0:pred=..." or "[timestamp] S0:pred=..."
            const m = line.match(/S([0-3]):/);
            return m ? parseInt(m[1]) : -1;
        }

        function updateStateFromLine(line) {
            const sIdx = parseSensorIndex(line);
            if (sIdx < 0) return;
            const state = sensorState[sIdx];

            if (line.includes("pred=")) {
                const predMatch = line.match(/pred=([^,]+)/);
                const confMatch = line.match(/conf=([0-9.]+)/);
                const detMatch = line.match(/detected=([01])/);
                if (predMatch) state.pred = predMatch[1];
                if (confMatch) state.conf = parseFloat(confMatch[1]);
                if (detMatch) state.detected = detMatch[1] === '1';
            }

            if (line.includes(",GATE,")) {
                let mvIdx = line.indexOf("mv:");
                let stIdx = line.indexOf(",st:");
                if (mvIdx !== -1 && stIdx !== -1) {
                    const mvPart = line.substring(mvIdx + 3, stIdx);
                    const stPart = line.substring(stIdx + 4);
                    state.mvVals = mvPart.split(",").map(Number);
                    state.stVals = stPart.split(",").map(Number);
                }
            }
        }

        function renderSensor(sIdx) {
            const state = sensorState[sIdx];
            const chart = charts[sIdx];

            chart.data.datasets[0].data = state.mvVals;
            chart.data.datasets[1].data = state.stVals;
            chart.update();

            const badge = document.getElementById('pred' + sIdx);
            if (state.pred === 'WAITING') {
                badge.innerText = 'WAITING';
                badge.className = 'pred-badge';
            } else {
                badge.innerText = state.pred.toUpperCase() + ' ' + (state.conf * 100).toFixed(0) + '%';
                if (state.detected) {
                    badge.className = 'pred-badge detected';
                } else {
                    badge.className = 'pred-badge background-class';
                }
            }
        }

        function processLine(line) {
            updateStateFromLine(line);

            const sIdx = parseSensorIndex(line);
            if (sIdx >= 0) renderSensor(sIdx);

            // Log panel
            const div = document.createElement('div');
            div.innerText = line;

            if (sIdx >= 0) div.classList.add('log-s' + sIdx);
            if (line.includes("features=") || line.includes("FEAT,")) div.classList.add('log-feat');
            if (line.includes("pred=")) div.classList.add('log-pred');

            consoleEl.appendChild(div);
            if (consoleEl.children.length > 2000) consoleEl.removeChild(consoleEl.firstChild);
            consoleEl.scrollTop = consoleEl.scrollHeight;
        }

        function connect() {
            ws = new WebSocket(wsUrl);
            ws.onopen = () => { statusEl.innerText = "Connected"; statusEl.className = "connected"; };
            ws.onclose = () => { statusEl.innerText = "Reconnecting..."; statusEl.className = ""; setTimeout(connect, 2000); };
            ws.onmessage = (event) => processLine(event.data);
        }

        connect();
    </script>
</body>
</html>
"""

@app.get("/")
def get_ui():
    """Serves the Web UI"""
    return HTMLResponse(HTML_PAGE)

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    """Handles WebSocket connections for real-time logs"""
    await websocket.accept()
    serial_clients.add(websocket)
    try:
        while True:
            await websocket.receive_text()
    except WebSocketDisconnect:
        if websocket in serial_clients:
            serial_clients.remove(websocket)

if __name__ == "__main__":
    port_arg = sys.argv[1] if len(sys.argv) > 1 else find_serial_port()
    baudrate_arg = int(sys.argv[2]) if len(sys.argv) > 2 else 115200
    output_file_arg = sys.argv[3] if len(sys.argv) > 3 else None

    if port_arg is None:
        print("❌ Could not find serial port automatically.")
        print("\nUsage: python3 capture_inference_pico2w.py <port> [baudrate] [output_file]")
        sys.exit(1)

    # Setup logs directory
    os.makedirs("logs", exist_ok=True)
    if output_file_arg is None:
        ts = datetime.now().strftime("%Y%m%d_%H%M%S")
        output_file_arg = os.path.join("logs", f"pico_inference_{ts}.txt")

    # Start FastAPI Web Server
    local_ip = get_local_ip()
    print(f"\n🌐 Starting Web Server at http://{local_ip}:8000")
    print(f"📱 Connect from your phone at: http://{local_ip}:8000")
    print(f"🏠 Or locally at: http://localhost:8000\n")

    try:
        uvicorn.run(app, host="0.0.0.0", port=8000, log_level="error")
    except KeyboardInterrupt:
        pass
    finally:
        running = False
        print("\n✓ Stopped.")
