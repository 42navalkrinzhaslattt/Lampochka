#!/usr/bin/env python3
"""
Clean vehicle dataset as requested by user.
Criteria: max_move_energy > 20
Files: logs/08/vehicle1.txt to vehicle4.txt
"""

import os

LOG_DIR = 'logs/08'
FILES_TO_CLEAN = [
    'vehicle1.txt',
    'vehicle2.txt',
    'vehicle3.txt',
    'vehicle4.txt'
]

# Criteria
ENERGY_THRESHOLD = 20.0 # keeps max_move_energy > 20

def parse_features_line(line):
    """
    New 'features=' line format.
    Format: ... features=v0,v1,v2...
    v0 = max_move_energy
    """
    try:
        content = line.split('features=')[1].strip()
        vals = [float(x) for x in content.split(',') if x.strip()]
        if len(vals) > 0: return vals[0]
    except (ValueError, IndexError):
        pass
    return None

def parse_legacy_feat(line):
    """
    Legacy FEAT format.
    Format: [timestamp] label,FEAT,v0,v1,v2...
    v0 = max_move_energy
    """
    try:
        parts = line.split(',')
        feat_idx = -1
        for i, p in enumerate(parts):
            if "FEAT" in p:
                feat_idx = i
                break
        if feat_idx != -1 and len(parts) > feat_idx + 1:
            return float(parts[feat_idx + 1].split(']')[0]) # v0 is 1 index after FEAT
    except (ValueError, IndexError):
        pass
    return None

def clean_file(filename):
    src_path = os.path.join(LOG_DIR, filename)
    name, ext = os.path.splitext(filename)
    dst_path = os.path.join(LOG_DIR, f"{name}_clean{ext}")
    
    if not os.path.exists(src_path):
        print(f"Skipping {filename} (not found)")
        return

    print(f"Cleaning {filename} -> {os.path.basename(dst_path)}...")
    
    with open(src_path, 'r') as f:
        lines = f.readlines()
        
    kept_count = 0
    removed_count = 0
    
    with open(dst_path, 'w') as f_out:
        i = 0
        while i < len(lines):
            line = lines[i]
            
            # --- Check format ---
            tot_mv = None
            if 'features=' in line:
                tot_mv = parse_features_line(line)
            elif ',FEAT,' in line:
                tot_mv = parse_legacy_feat(line)
            
            # If it's a feature or FEAT line
            if tot_mv is not None:
                if tot_mv > ENERGY_THRESHOLD:
                    # If this was preceded by a GATE line natively, 
                    # let's make sure we kept the GATE line previously. 
                    # Wait, the simplest robust way is to buffer non-feature lines 
                    # until we hit a feature line, decide if we keep it, and write the buffer.
                    # But vehicle1-3 don't have GATE lines. Only vehicle4 might.
                    # Let's just write this feature line.
                    f_out.write(line)
                    kept_count += 1
                else:
                    removed_count += 1
            else:
                # Other lines (e.g. headers, pred=, GATE lines)
                # If they are `pred=` following a removed `features=`, ideally we drop them.
                # If they are `GATE` preceding a removed `features=`, ideally we drop them.
                # To be perfectly clean, we can do a local buffer. For the purpose of retraining, 
                # parser only looks at `features=` and `FEAT` anyway. So writing extra lines doesn't affect parser.
                f_out.write(line)

            i += 1

    print(f"  Removed {removed_count} samples (<=20.0).")
    print(f"  Kept {kept_count} samples (>20.0).")

def main():
    for f in FILES_TO_CLEAN:
        clean_file(f)
    print("Done cleaning.")

if __name__ == '__main__':
    main()
