import asyncio
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.responses import HTMLResponse
import time
import os

app = FastAPI()
serial_clients = set()
running = True
log_file = "logs/07/human_running1_clean.txt"

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
            <h1>📡 Arduino Monitor (SIMULATION)</h1>
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
            
            div.addEventListener('mouseenter', () => {
                if (!isAutoScroll) {
                    renderState(globalHistory[idx]);
                    if (activeDiv) activeDiv.style.backgroundColor = '';
                    activeDiv = div;
                    div.style.backgroundColor = '#333';
                }
            });

            consoleEl.appendChild(div);
            // Increased to 2000 to allow a good amount of scrolling
            if (consoleEl.children.length > 2000) consoleEl.removeChild(consoleEl.firstChild);

            if (isAutoScroll) {
                consoleEl.scrollTop = consoleEl.scrollHeight;
                renderState(currentState);
            }
        }

        consoleEl.addEventListener('scroll', () => {
            const isAtBottom = consoleEl.scrollHeight - consoleEl.scrollTop - consoleEl.clientHeight < 10;
            if (isAtBottom && !isAutoScroll) {
                isAutoScroll = true;
                if (activeDiv) activeDiv.style.backgroundColor = '';
                statusEl.innerText = "Connected (Live)";
                statusEl.style.backgroundColor = "#4caf50";
                renderState(currentState);
            } else if (!isAtBottom && isAutoScroll) {
                isAutoScroll = false;
                statusEl.innerText = "PAUSED - Scroll to bottom to resume";
                statusEl.style.backgroundColor = "#E91E63";
            }
            
            if (!isAutoScroll) {
                const middle = consoleEl.scrollTop + (consoleEl.clientHeight / 2);
                let closest = null;
                let minDist = Infinity;
                const children = consoleEl.children;
                
                if (children.length > 0) {
                    const avgHeight = consoleEl.scrollHeight / children.length;
                    let guessIdx = Math.floor(middle / avgHeight);
                    guessIdx = Math.max(0, Math.min(children.length - 1, guessIdx));
                    
                    for(let i = Math.max(0, guessIdx - 20); i < Math.min(children.length, guessIdx + 20); i++) {
                        const child = children[i];
                        const childMiddle = child.offsetTop + (child.offsetHeight / 2);
                        const dist = Math.abs(childMiddle - middle);
                        if (dist < minDist) {
                            minDist = dist;
                            closest = child;
                        }
                    }
                }
                
                if (closest && closest !== activeDiv) {
                    if (activeDiv) activeDiv.style.backgroundColor = '';
                    activeDiv = closest;
                    activeDiv.style.backgroundColor = '#333';
                    const idx = closest.dataset.idx;
                    renderState(globalHistory[idx]);
                }
            }
        });

        connect();
    </script>
</body>
</html>
"""

@app.get("/")
def get_ui():
    """Serves the Web UI"""
    return HTMLResponse(HTML_PAGE)

async def stream_logs():
    print(f"Starting simulation of {log_file}")
    with open(log_file, "r") as f:
        for line in f:
            if not getattr(app.state, "running", True):
                break
            
            if serial_clients:
                disconnected = set()
                for client in list(serial_clients):
                    try:
                        await client.send_text(line.strip())
                    except Exception:
                        disconnected.add(client)
                for client in disconnected:
                    if client in serial_clients:
                        serial_clients.remove(client)
            await asyncio.sleep(0.05) # 20 Hz simulation
    print("Simulation finished (end of file).")
@app.on_event("startup")
async def startup_event():
    app.state.running = True
    asyncio.create_task(stream_logs())
    
@app.on_event("shutdown")
async def shutdown_event():
    app.state.running = False

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
    import uvicorn
    print(f"\n🌐 Starting Web Server at http://0.0.0.0:8001")
    uvicorn.run(app, host="0.0.0.0", port=8001, log_level="error")
