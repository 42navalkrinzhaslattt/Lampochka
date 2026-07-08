import glob
import os

search_dirs = ["/Users/lap/Documents/Arduino/lampochka/logs/06", "/Users/lap/Documents/Arduino/lampochka/logs/current"]

found_overall = False

for base_dir in search_dirs:
    for i in range(1, 5):
        filename = f"background{i}.txt"
        file_path = os.path.join(base_dir, filename)
        
        if not os.path.exists(file_path):
            continue
            
        print(f"\nSearching in {file_path} for > 50...")
        found_count = 0
        
        with open(file_path, 'r') as f:
            lines = f.readlines()
            for line_idx, line in enumerate(lines):
                if ",GATE,mv:" in line:
                    try:
                        mv_str = line.split(",GATE,mv:")[1].split(",st:")[0]
                        mv_vals = [int(v) for v in mv_str.split(",")]
                        
                        # Check for adjacent > 70
                        for j in range(len(mv_vals) - 1):
                            if mv_vals[j] > 50 and mv_vals[j+1] > 50:
                                print(f"  Line {line_idx + 1}: {line.strip()}")
                                found_count += 1
                                found_overall = True
                                break # avoid printing same line twice
                    except Exception as e:
                        pass
        print(f"  -> Found {found_count} instances in {file_path}")

if not found_overall:
    print("\nNo instances found with adjacent moving gates > 50 energy.")
