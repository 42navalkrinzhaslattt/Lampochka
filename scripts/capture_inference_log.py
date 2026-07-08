#!/usr/bin/env python3
"""
Capture Arduino serial output to a timestamped log file and stream to a Web UI.
Usage: python3 capture_inference_log.py [port] [baudrate]
Example: python3 capture_inference_log.py /dev/cu.usbmodem14201 115200
"""

import serial
import socket
import sys
from datetime import datetime
import os
import asyncio
import threading
import glob
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

def find_arduino_port():
    """Try to find Arduino port automatically"""
    ports = glob.glob('/dev/cu.usbmodem*') + glob.glob('/dev/tty.usbmodem*')
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
        ser = serial.Serial(port, baudrate, timeout=1)
    except serial.SerialException as e:
        print(f"\n❌ Serial Error: {e}")
        print(f"\nAvailable ports might be:")
        for p in glob.glob('/dev/cu.usb*') + glob.glob('/dev/tty.usb*'):
            print(f"  - {p}")
        os._exit(1)

    print(f"📡 Connected to {port} at {baudrate} baud...")
    print(f"📝 Logging to: {output_file}")
    
    with open(output_file, 'w', buffering=1) as f:
        header = f"=== Arduino Serial Log ===\n"
        header += f"Port: {port}\n"
        header += f"Baudrate: {baudrate}\n"
        header += f"Started: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n"
        header += "=" * 50 + "\n\n"
        f.write(header)
        print(header)
        
        while running:
            if ser.in_waiting > 0:
                try:
                    line = ser.readline().decode('utf-8', errors='replace').rstrip()
                    if line:
                        timestamp = datetime.now().strftime('%H:%M:%S.%f')[:-3]
                        log_line = f"[{timestamp}] {line}"
                        
                        f.write(log_line + "\n")
                        print(log_line)
                        
                        # Broadcast to UI (Filter out verbose raw_baseline to avoid flooding)
                        if serial_clients and "raw_baseline" not in line:
                            asyncio.run_coroutine_threadsafe(
                                broadcast_log(log_line),
                                asyncio_loop
                            )
                except Exception as e:
                    print(f"Error processing line: {e}")
            else:
                # Small sleep to prevent CPU hogging
                import time
                time.sleep(0.01)
                
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
            
    # Cleanup disconnected clients
    for client in disconnected:
        if client in serial_clients:
            serial_clients.remove(client)

HTML_PAGE = """
<!DOCTYPE html>
<html>
<head>
    <title>Arduino Inference Monitor</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body { font-family: -apple-system, system-ui, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif; background-color: #121212; color: #e0e0e0; margin: 0; padding: 20px; }
        .container { max-width: 1200px; margin: 0 auto; display: flex; flex-direction: column; gap: 20px; height: 95vh;}
        header { display: flex; justify-content: space-between; align-items: center; border-bottom: 1px solid #333; padding-bottom: 10px; flex-shrink: 0;}
        h1 { margin: 0; font-size: 1.5rem; color: #fff; }
        #status { padding: 4px 10px; border-radius: 4px; font-size: 0.9em; font-weight: bold; background-color: #ff9800; color: #000; }
        .connected { background-color: #4caf50 !important; color: white !important;}
        
        .dashboard { display: flex; gap: 20px; flex: 1; flex-direction: column; overflow: hidden; }
        @media (min-width: 768px) { .dashboard { flex-direction: row; align-items: stretch; } }
        
        .panel { background-color: #1e1e1e; border: 1px solid #333; border-radius: 8px; padding: 15px; display: flex; flex-direction: column; min-height: 0;}
        .charts-panel { flex: 2; gap: 20px; overflow-y: auto;} /* Charts take 2/3 space on desktop */
        .log-panel { flex: 1; overflow: hidden; display: flex; flex-direction: column;} /* Log takes 1/3 space */
        
        .chart-container { position: relative; height: 35vh; width: 100%; border: 1px solid #2a2a2a; border-radius: 4px; background: #161616; padding: 10px; box-sizing: border-box; min-height: 250px;}
        
        #console { flex: 1; background-color: #000; color: #00ff00; font-family: monospace; font-size: 12px; padding: 10px; overflow-y: auto; border-radius: 4px; border: 1px solid #333; white-space: pre-wrap; word-wrap: break-word; line-height: 1.4; min-height: 0;}
        .log-pred { color: #00bcd4; font-weight: bold; }
        .log-feat { color: #9c27b0; }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <h1>📡 Arduino Monitor</h1>
            <div id="status">Connecting...</div>
        </header>

        <div class="dashboard">
            <div class="panel charts-panel">
                <h3 style="margin-top:0;">Gate Energy (0-13)</h3>
                <div class="chart-container">
                    <canvas id="gateChart"></canvas>
                </div>
                
                <h3 style="margin-top:20px;">Current Prediction</h3>
                <div class="chart-container" style="display: flex; align-items: center; justify-content: center; min-height: 100px; height: 100px;">
                    <div id="predictionDisplay" style="font-size: 3rem; font-weight: bold; text-align: center; color: #888; text-shadow: 2px 2px 4px rgba(0,0,0,0.5);">WAITING</div>
                </div>
            </div>

            <div class="panel log-panel">
                <h3 style="margin-top:0;">Raw Output</h3>
                <div id="console"></div>
            </div>
        </div>
    </div>
    <script>
        const wsProtocol = window.location.protocol === "https:" ? "wss:" : "ws:";
        const wsUrl = wsProtocol + "//" + window.location.host + "/ws";
        let ws;
        
        const statusEl = document.getElementById('status');
        const consoleEl = document.getElementById('console');
        
        Chart.defaults.color = '#888';
        Chart.defaults.borderColor = '#333';

        const gCtx = document.getElementById('gateChart').getContext('2d');
        const gateChart = new Chart(gCtx, {
            type: 'bar',
            data: {
                labels: Array.from({length: 14}, (_, i) => `${i}`),
                datasets: [
                    { label: 'Moving', data: Array(14).fill(0), backgroundColor: 'rgba(54, 162, 235, 0.7)' },
                    { label: 'Stationary', data: Array(14).fill(0), backgroundColor: 'rgba(255, 99, 132, 0.7)' }
                ]
            },
            options: {
                responsive: true, maintainAspectRatio: false, animation: { duration: 0 },
                scales: { y: { min: 0, max: 100 } }
            }
        });

        let currentState = {
            pred: 'WAITING', conf: 0,
            mvVals: Array(14).fill(0), stVals: Array(14).fill(0)
        };

        const globalHistory = [];
        let isAutoScroll = true;
        let activeDiv = null;

        function connect() {
            ws = new WebSocket(wsUrl);
            ws.onopen = () => { statusEl.innerText = "Connected"; statusEl.className = "connected"; };
            ws.onclose = () => { statusEl.innerText = "Disconnected. Reconnecting..."; statusEl.className = ""; setTimeout(connect, 2000); };
            ws.onmessage = (event) => processLine(event.data);
        }

        function renderState(state) {
            if (!state) return;
            gateChart.data.datasets[0].data = state.mvVals;
            gateChart.data.datasets[1].data = state.stVals;
            gateChart.update();
            
            const displayEl = document.getElementById('predictionDisplay');
            if (state.pred === 'background') displayEl.style.color = '#f44336';
            else if (state.pred === 'human' || state.pred === 'vehicle' || state.pred === 'target') displayEl.style.color = '#4caf50';
            else displayEl.style.color = '#fff';
            
            if (state.pred !== 'WAITING') displayEl.innerText = `${state.pred.toUpperCase()} ${(state.conf*100).toFixed(0)}%`;
            else displayEl.innerText = 'WAITING';
        }

        function updateStateFromLine(line) {
            if (line.includes("pred=")) {
                const predMatch = line.match(/pred=([^,]+)/);
                const confMatch = line.match(/conf=([0-9.]+)/);
                if (predMatch && confMatch) {
                    currentState.pred = predMatch[1];
                    currentState.conf = parseFloat(confMatch[1]);
                }
            }

            if (line.includes(",GATE,")) {
                let mvIdx = line.indexOf("mv:");
                let stIdx = line.indexOf("st:");
                if (mvIdx !== -1 && stIdx !== -1) {
                    const mvPart = line.substring(mvIdx + 3, stIdx - 1);
                    const stPart = line.substring(stIdx + 3);
                    currentState.mvVals = mvPart.split(",").map(Number);
                    currentState.stVals = stPart.split(",").map(Number);
                }
            }
        }

        function processLine(line) {
            updateStateFromLine(line);
            const snapshot = JSON.parse(JSON.stringify(currentState));
            globalHistory.push(snapshot);
            const idx = globalHistory.length - 1;

            const div = document.createElement('div');
            div.innerText = line;
            div.dataset.idx = idx;

            if (line.includes("features=") || line.includes("FEAT,")) div.className = "log-feat";
            if (line.includes("pred=")) div.className = "log-pred";
            if (line.includes(",GATE,")) div.className = "log-feat";

            consoleEl.appendChild(div);
            // Increased to 2000 to allow a good amount of scrolling
            if (consoleEl.children.length > 2000) consoleEl.removeChild(consoleEl.firstChild);

            if (isAutoScroll) {
                consoleEl.scrollTop = consoleEl.scrollHeight;
                renderState(currentState);
            }
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
            # Just keep connection open, we only send data from server->client
            await websocket.receive_text()
    except WebSocketDisconnect:
        if websocket in serial_clients:
            serial_clients.remove(websocket)

if __name__ == "__main__":
    port_arg = sys.argv[1] if len(sys.argv) > 1 else find_serial_port()
    baudrate_arg = int(sys.argv[2]) if len(sys.argv) > 2 else 115200
    output_file_arg = sys.argv[3] if len(sys.argv) > 3 else None
    
    if port_arg is None:
        print("❌ Could not find Arduino port automatically.")
        print("\nUsage: python3 capture_inference_log.py <port> [baudrate] [output_file]")
        sys.exit(1)

    # Setup logs directory
    os.makedirs("logs", exist_ok=True)
    if output_file_arg is None:
        ts = datetime.now().strftime("%Y%m%d_%H%M%S")
        output_file_arg = os.path.join("logs", f"arduino_log_{ts}.txt")

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
