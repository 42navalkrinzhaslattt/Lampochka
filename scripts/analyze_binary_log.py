import re
import numpy as np
import sys
sys.path.append("/Users/lap/Documents/Arduino/lampochka/scripts")
from train_running_v7 import FEATURE_NAMES

log_path = "/Users/lap/Documents/Arduino/lampochka/logs/arduino_log_20260221_133915.txt"

with open(log_path, "r") as f:
    lines = f.readlines()

human_feats = []
bg_feats = []

# indices in FEATURE_NAMES
max_move_idx = FEATURE_NAMES.index("max_move_energy")
tot_move_idx = FEATURE_NAMES.index("total_move_energy")
max_stat_idx = FEATURE_NAMES.index("max_stat_energy")
tot_stat_idx = FEATURE_NAMES.index("total_stat_energy")


for i in range(len(lines)):
    if "pred=" in lines[i]:
        feat_line = lines[i-1]
        if "features=" in feat_line:
            feats = [float(x) for x in feat_line.split("features=")[1].strip().split(",")]
            if "pred=human" in lines[i]:
                human_feats.append(feats)
            elif "pred=background" in lines[i]:
                bg_feats.append(feats)

print(f"Total Background (TN?): {len(bg_feats)}")
print(f"Total Human (FP?): {len(human_feats)}")

human_feats = np.array(human_feats)

if len(human_feats) > 0:
    print("\n--- Human (False Alarms) Energy Distribution ---")
    h_max_move = human_feats[:, max_move_idx]
    h_tot_move = human_feats[:, tot_move_idx]
    h_max_stat = human_feats[:, max_stat_idx]
    h_tot_stat = human_feats[:, tot_stat_idx]
    
    print(f"Max Move Energy: Avg={np.mean(h_max_move):.2f}, Min={np.min(h_max_move):.2f}, Max={np.max(h_max_move):.2f}")
    print(f"Total Move Energy: Avg={np.mean(h_tot_move):.2f}, Min={np.min(h_tot_move):.2f}, Max={np.max(h_tot_move):.2f}")
    print(f"Max Stat Energy: Avg={np.mean(h_max_stat):.2f}, Min={np.min(h_max_stat):.2f}, Max={np.max(h_max_stat):.2f}")
    print(f"Total Stat Energy: Avg={np.mean(h_tot_stat):.2f}, Min={np.min(h_tot_stat):.2f}, Max={np.max(h_tot_stat):.2f}")
    
    # Filter specific low energy humans vs high energy humans
    low_energy_mask = (h_max_move < 30) & (h_max_stat < 30)
    low_energy_humans = human_feats[low_energy_mask]
    high_energy_humans = human_feats[~low_energy_mask]
    
    print(f"\nLow Energy Humans (<30 stat, <30 move): {len(low_energy_humans)}")
    print(f"High Energy Humans: {len(high_energy_humans)}")

    if len(high_energy_humans) > 0:
       print("\nChecking the high energy human false alarms:")
       for i, f in enumerate(high_energy_humans[:5]):
          print(f"  Sample {i}: Move={f[max_move_idx]}, Stat={f[max_stat_idx]}, duration={f[FEATURE_NAMES.index('motion_duration')]}, variance={f[FEATURE_NAMES.index('move_variance')]}")

