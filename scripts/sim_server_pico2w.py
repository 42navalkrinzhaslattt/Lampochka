#!/usr/bin/env python3
"""
Simulation server: replays a Pico 2W log file (4 sensors) through
the same 4-sensor web UI as capture_inference_pico2w.py.

Usage: python3 sim_server_pico2w.py [log_file]
Then open http://localhost:8001
"""

import asyncio
import socket
import sys
import uvicorn
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse

app = FastAPI()
serial_clients = set()

log_file = sys.argv[1] if len(sys.argv) > 1 else "logs/pico_inference_20260315_210448.txt"

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

HTML_PAGE = """
<!DOCTYPE html>
<html>
<head>
    <title>Pico 2W — 4-Sensor Sim</title>
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
        .sensor-title { font-size: 0.95rem; font-weight: 600; }
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

        .chart-wrap { height: 180px; position: relative; }

        .bottom-row { display: flex; gap: 12px; }
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
        <h1>📡 Pico 2W Sim <span>4 Sensors — REPLAY</span></h1>
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
            <h3>Replay Log</h3>
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
        const globalHistory = [];
        let isAutoScroll = true;
        let activeDiv = null;

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

        function renderAllFromSnapshot(snapshot) {
            if (!snapshot) return;
            for (let s = 0; s < 4; s++) {
                if (snapshot[s]) {
                    sensorState[s] = JSON.parse(JSON.stringify(snapshot[s]));
                    renderSensor(s);
                }
            }
        }

        function processLine(line) {
            updateStateFromLine(line);

            const sIdx = parseSensorIndex(line);
            if (sIdx >= 0 && isAutoScroll) renderSensor(sIdx);

            // Snapshot all 4 sensors for history scrubbing
            const snapshot = sensorState.map(s => JSON.parse(JSON.stringify(s)));
            globalHistory.push(snapshot);
            const idx = globalHistory.length - 1;

            const div = document.createElement('div');
            div.innerText = line;
            div.dataset.idx = idx;

            if (sIdx >= 0) div.classList.add('log-s' + sIdx);
            if (line.includes("features=") || line.includes("FEAT,")) div.classList.add('log-feat');
            if (line.includes("pred=")) div.classList.add('log-pred');

            div.addEventListener('mouseenter', () => {
                if (!isAutoScroll) {
                    renderAllFromSnapshot(globalHistory[idx]);
                    if (activeDiv) activeDiv.style.backgroundColor = '';
                    activeDiv = div;
                    div.style.backgroundColor = '#333';
                }
            });

            consoleEl.appendChild(div);
            if (consoleEl.children.length > 2000) consoleEl.removeChild(consoleEl.firstChild);
            if (isAutoScroll) consoleEl.scrollTop = consoleEl.scrollHeight;
        }

        consoleEl.addEventListener('scroll', () => {
            const isAtBottom = consoleEl.scrollHeight - consoleEl.scrollTop - consoleEl.clientHeight < 10;
            if (isAtBottom && !isAutoScroll) {
                isAutoScroll = true;
                if (activeDiv) activeDiv.style.backgroundColor = '';
                statusEl.innerText = "Connected (Live)";
                statusEl.style.backgroundColor = "#4caf50";
            } else if (!isAtBottom && isAutoScroll) {
                isAutoScroll = false;
                statusEl.innerText = "PAUSED - Scroll to bottom to resume";
                statusEl.style.backgroundColor = "#E91E63";
            }

            if (!isAutoScroll) {
                const middle = consoleEl.scrollTop + (consoleEl.clientHeight / 2);
                const children = consoleEl.children;
                if (children.length > 0) {
                    const avgHeight = consoleEl.scrollHeight / children.length;
                    let guessIdx = Math.floor(middle / avgHeight);
                    guessIdx = Math.max(0, Math.min(children.length - 1, guessIdx));
                    let closest = null, minDist = Infinity;
                    for (let i = Math.max(0, guessIdx - 20); i < Math.min(children.length, guessIdx + 20); i++) {
                        const child = children[i];
                        const dist = Math.abs(child.offsetTop + child.offsetHeight / 2 - middle);
                        if (dist < minDist) { minDist = dist; closest = child; }
                    }
                    if (closest && closest !== activeDiv) {
                        if (activeDiv) activeDiv.style.backgroundColor = '';
                        activeDiv = closest;
                        activeDiv.style.backgroundColor = '#333';
                        renderAllFromSnapshot(globalHistory[closest.dataset.idx]);
                    }
                }
            }
        });

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
    return HTMLResponse(HTML_PAGE)

async def stream_logs():
    print(f"▶ Replaying: {log_file}")
    with open(log_file, "r") as f:
        for line in f:
            if not getattr(app.state, "running", True):
                break

            stripped = line.strip()
            if not stripped:
                continue

            if serial_clients:
                disconnected = set()
                for client in list(serial_clients):
                    try:
                        await client.send_text(stripped)
                    except Exception:
                        disconnected.add(client)
                for client in disconnected:
                    if client in serial_clients:
                        serial_clients.remove(client)
            await asyncio.sleep(0.005)  # 200 Hz replay (10x faster)
    print("✓ Replay finished (end of file).")

@app.on_event("startup")
async def startup_event():
    app.state.running = True
    asyncio.create_task(stream_logs())

@app.on_event("shutdown")
async def shutdown_event():
    app.state.running = False

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    serial_clients.add(websocket)
    try:
        while True:
            await websocket.receive_text()
    except WebSocketDisconnect:
        if websocket in serial_clients:
            serial_clients.remove(websocket)

if __name__ == "__main__":
    local_ip = get_local_ip()
    print(f"\n🌐 Starting Sim Server at http://{local_ip}:8001")
    print(f"📱 Connect from your phone at: http://{local_ip}:8001")
    print(f"🏠 Or locally at: http://localhost:8001")
    print(f"📂 Log file: {log_file}\n")
    uvicorn.run(app, host="0.0.0.0", port=8001, log_level="error")
