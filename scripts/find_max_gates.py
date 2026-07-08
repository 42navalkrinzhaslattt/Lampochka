import os

log_file = "/Users/lap/Documents/Arduino/lampochka/logs/09/background1_rain.txt"

if not os.path.exists(log_file):
    print(f"Error: {log_file} does not exist.")
    exit(1)

max_moving = [0] * 14

with open(log_file, 'r') as f:
    for line in f:
        if ",GATE,mv:" in line:
            try:
                mv_str = line.split(",GATE,mv:")[1].split(",st:")[0]
                mv_vals = [int(v) for v in mv_str.split(",")]
                
                for i in range(min(len(mv_vals), 14)):
                    if mv_vals[i] > max_moving[i]:
                        max_moving[i] = mv_vals[i]
            except Exception as e:
                pass

print("Historical Maximums for Moving Gates (logs/09/background1_rain.txt):")
for i, val in enumerate(max_moving):
    print(f"Gate {i:2d}: {val}")
