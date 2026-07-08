import os
import numpy as np
from sklearn.svm import LinearSVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.metrics import classification_report, accuracy_score
from sklearn.model_selection import train_test_split

LOG_FILE = "logs/pico_4s_mixed.txt"

FILES_MAP = {
    'logs/current': [
        'human1_clean.txt', 'human2_clean.txt', 'human3_clean.txt', 'human4_clean.txt',
        'background1.txt', 'background2.txt', 'background3.txt', 'background4.txt',
        'human5_clean.txt', 'human6_clean.txt', 'human7_clean.txt',
        'background5.txt',
        'human_running1.txt',
        'human_running2.txt'
    ],
    'logs/08': [
        'vehicle1_clean.txt',
        'vehicle2_clean.txt',
        'vehicle3_clean.txt',
        'vehicle4_clean.txt'
    ]
}

FEATURE_NAMES = [
    "max_move_energy", "total_move_energy", "active_move_zones", "move_centroid", "move_spread",
    "max_stat_energy", "total_stat_energy", "active_stat_zones", "stat_centroid", "stat_spread",
    "move_change_rate", "stat_change_rate", "zone_switches", "motion_duration", "move_variance", "stat_variance",
    "zones_sequential", "zone_correlation", "weighted_peak_stability", "spatial_smoothness_index",
    "centroid_velocity_smoothed", "movement_linearity", "spatial_kurtosis", "spatial_skewness",
    "hjorth_mobility", "hjorth_complexity", "crest_factor", "spectral_entropy", "flicker_index", "flux"
]

def check_target_rule(mv_gates):
    """
    Rule: 2 or 3 (strictly) consecutive gates with moving energy >= 40 = target.
    Everything else = background.
    """
    active_idx = [i for i, v in enumerate(mv_gates) if v >= 40]
    
    if len(active_idx) in [2, 3]:
        if max(active_idx) - min(active_idx) == len(active_idx) - 1:
            return "target"
            
    return "background"

def load_data():
    X = []
    y = []
    
    # 1. Parse multi-sensor mixed log
    last_features = {}
    print(f"Parsing {LOG_FILE}...")
    if os.path.exists(LOG_FILE):
        with open(LOG_FILE, 'r') as f:
            for line in f:
                if ":features=" in line:
                    try:
                        s_idx = int(line.split(":")[0][-1])
                        feat_str = line.split("features=")[1]
                        features = [float(x) for x in feat_str.split(",")]
                        if len(features) == 30:
                            last_features[s_idx] = features
                    except: pass
                elif ",GATE,mv:" in line:
                    try:
                        s_idx = int(line.split(":")[0][-1])
                        if s_idx in last_features:
                            mv_str = line.split("GATE,mv:")[1].split(",st:")[0]
                            mv_gates = [float(x) for x in mv_str.split(",")]
                            label = check_target_rule(mv_gates)
                            X.append(last_features[s_idx])
                            y.append(label)
                            del last_features[s_idx]
                    except: pass
                    
    # 2. Parse older single-sensor logs
    for d, files in FILES_MAP.items():
        for fname in files:
            path = os.path.join(d, fname)
            if not os.path.exists(path): continue
            
            print(f"Parsing {path}...")
            pending_feat = None
            pending_mv = None
            
            with open(path, "r") as f:
                for line in f:
                    if "GATE,mv:" in line:
                        try:
                            mv_str = line.split("GATE,mv:")[1].split(",st:")[0]
                            pending_mv = [float(x) for x in mv_str.split(",")]
                        except: pass
                        
                    elif ",FEAT," in line:
                        try:
                            parts = line.split(",")
                            idx = parts.index("FEAT")
                            fs = [float(x.split("]")[0]) for x in parts[idx+1:]]
                            if len(fs) == 30: pending_feat = fs
                        except: pass
                        
                    elif "features=" in line and "S" not in line:
                        try:
                            fs = [float(x) for x in line.split("features=")[1].split(",")]
                            if len(fs) == 30: pending_feat = fs
                        except: pass
                        
                    if pending_feat is not None and pending_mv is not None:
                        X.append(pending_feat)
                        y.append(check_target_rule(pending_mv))
                        pending_feat = None
                        pending_mv = None

    return np.array(X), np.array(y)

def main():
    X_all, y_all = load_data()
    
    if len(X_all) == 0:
        print("No valid data found.")
        return
        
    classes, counts = np.unique(y_all, return_counts=True)
    print(f"\nTotal Dataset: {len(X_all)} samples")
    for cls, c in zip(classes, counts):
        print(f"  {cls}: {c}")
        
    X_train, X_test, y_train, y_test = train_test_split(
        X_all, y_all, test_size=0.2, stratify=y_all, random_state=42
    )
    
    print(f"\nTraining LinearSVC and RandomForest on {len(X_train)} samples...")
    
    # --- SVM Pipeline ---
    svm_pipe = Pipeline([
        ('scaler', StandardScaler()),
        ('clf', LinearSVC(C=1.0, class_weight='balanced', max_iter=50000, dual=False, random_state=42))
    ])
    svm_pipe.fit(X_train, y_train)
    
    # --- RF Pipeline ---
    # Random forests do not require standard scaling, but it's fine to use or omit. 
    # We will train it without scaling.
    rf_clf = RandomForestClassifier(n_estimators=100, class_weight='balanced', random_state=42)
    rf_clf.fit(X_train, y_train)
    
    # --- Evaluation ---
    print("\n============ LINEAR SVC RESULTS ============")
    svm_pred = svm_pipe.predict(X_test)
    print(classification_report(y_test, svm_pred))
    
    print("============ RANDOM FOREST RESULTS ============")
    rf_pred = rf_clf.predict(X_test)
    print(classification_report(y_test, rf_pred))
    
    # --- Feature Analysis ---
    print("\n============ SVM FEATURE IMPORTANCE (Weights) ============")
    clf_svm = svm_pipe.named_steps['clf']
    # For binary classification, coef_ is 1xN. The positive class is usually the second one alphabetically ('target')
    # Because target > background.
    target_class_idx = list(svm_pipe.classes_).index('target') 
    
    coefs = clf_svm.coef_[0]
    
    # If target is index 0 for some weird reason, flip coefs conceptually, but let's assume index 1 is target
    if target_class_idx == 0:
        coefs = -coefs
        
    top_pos_idx = np.argsort(coefs)[::-1][:10]
    top_neg_idx = np.argsort(coefs)[:10]
    
    print("Top features pushing towards TARGET:")
    for idx in top_pos_idx:
        print(f"  + {FEATURE_NAMES[idx]:<28} : {coefs[idx]:.4f}")
        
    print("\nTop features pushing towards BACKGROUND:")
    for idx in top_neg_idx:
        print(f"  - {FEATURE_NAMES[idx]:<28} : {coefs[idx]:.4f}")
        
    print("\n============ RANDOM FOREST FEATURE IMPORTANCE ============")
    importances = rf_clf.feature_importances_
    rf_top_idx = np.argsort(importances)[::-1][:15]
    for i, idx in enumerate(rf_top_idx):
        print(f"  {(i+1):2d}. {FEATURE_NAMES[idx]:<28} : {importances[idx]:.4f}")

if __name__ == "__main__":
    main()
