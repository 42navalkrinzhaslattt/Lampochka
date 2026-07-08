import os
import sys
sys.path.append(os.path.dirname(os.path.abspath(__file__)))
import numpy as np
import m2cgen as m2c
from sklearn.ensemble import RandomForestClassifier
from train_threshold_models import load_data

def main():
    print("Loading datasets...")
    X, y_str = load_data()
    
    # Map to binary integer classification
    y = np.where(y_str == 'target', 1, 0)
    
    # Train constrained RF for embedded architectures
    print("\nTraining embedded-friendly Random Forest (15 trees, depth 8)...")
    rf = RandomForestClassifier(n_estimators=15, max_depth=8, random_state=42, n_jobs=-1)
    rf.fit(X, y)
    
    acc = rf.score(X, y)
    print(f"Training accuracy: {acc:.4f}")
    
    print("\nExporting to C...")
    c_code = m2c.export_to_c(rf)
    
    out_file = "sketches/rf_pico2w/RandomForest.h"
    with open(out_file, "w") as f:
        f.write("#pragma once\n\n")
        f.write("#include <math.h>\n\n")
        f.write(c_code)
    print(f"Exported successfully to {out_file}")

if __name__ == '__main__':
    main()
