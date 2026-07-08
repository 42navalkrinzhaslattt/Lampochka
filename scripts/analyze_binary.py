import numpy as np
import sys
import os
sys.path.append("/Users/lap/Documents/Arduino/lampochka/scripts")
from train_running_v7 import FILES_MAP, parse_log_file

log_path = "/Users/lap/Documents/Arduino/lampochka/logs/arduino_log_20260220_190524.txt"
X_log, y_log = parse_log_file(log_path, "unknown")

# Get original data without vehicle class to train a purely binary model
X_bin = []
y_bin = []
base_path = "/Users/lap/Documents/Arduino/lampochka"

for log_dir, files in FILES_MAP.items():
    if "08" in log_dir: continue # skip vehicle data
    
    # Load just human/background
    for filename in files:
        if "vehicle" in filename.lower(): continue
        
        path = os.path.join(base_path, log_dir, filename)
        if "human" in filename.lower():
            label = "human"
        else:
            label = "background"
            
        X, y = parse_log_file(path, label)
        if len(X) > 0:
            for row in X:
                X_bin.append(row)
                y_bin.append(label)

X_bin = np.array(X_bin)
y_bin = np.array(y_bin)

from sklearn.svm import LinearSVC
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.pipeline import Pipeline

le = LabelEncoder()
y_enc = le.fit_transform(y_bin)

pipe = Pipeline([
    ("scaler", StandardScaler()),
    ("clf", LinearSVC(C=1.0, class_weight="balanced", max_iter=50000, dual=False))
])
pipe.fit(X_bin, y_enc)

# Now evaluate on the log where it threw false alarms
X_scaled = pipe.named_steps["scaler"].transform(X_log)
y_pred = pipe.named_steps["clf"].predict(X_scaled)
pred_labels = le.inverse_transform(y_pred)

print("\n--- BINARY MODEL (No Vehicle Logic at all) ---")
print(f"Predicted Background: {np.sum(pred_labels == 'background')}")
print(f"Predicted Human: {np.sum(pred_labels == 'human')}")

# find the same low energy false alarms
fa_idx = [i for i, (p, orig_x) in enumerate(zip(pred_labels, X_log)) if p == "human" and orig_x[0] < 30 and orig_x[5] < 30]
print(f"Low Energy 'Human' False Alarms in purely binary model: {len(fa_idx)}")

if len(fa_idx) > 0:
    mean_fa_scaled = np.mean(X_scaled[fa_idx], axis=0)
    
    # LinearSVC for binary has coef_ shape (1, n_features)
    # y=1 is typically human (since 'background'< 'human')
    weights = pipe.named_steps["clf"].coef_[0]
    bias = pipe.named_steps["clf"].intercept_[0]
    
    from train_running_v7 import FEATURE_NAMES
    
    print("\n--- Why Binary Model predicts Human for these false alarms ---")
    contributions = weights * mean_fa_scaled
    sorted_idx = np.argsort(contributions)[::-1]
    
    print(f"Bias: {bias:.4f}")
    print("Features driving it toward Human (Positive contributions):")
    for i in sorted_idx[:10]:
        print(f"  {FEATURE_NAMES[i]}: Effect={contributions[i]:+.4f} (Weight: {weights[i]:.4f}, Scaled Val: {mean_fa_scaled[i]:.2f})")
        
