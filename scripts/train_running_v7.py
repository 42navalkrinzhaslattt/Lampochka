import os
import numpy as np
import re
from sklearn.svm import LinearSVC
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.pipeline import Pipeline
from sklearn.metrics import accuracy_score, classification_report, confusion_matrix
from sklearn.model_selection import train_test_split
import joblib

# Configuration
# FILES_MAP maps directory -> list of files
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

# Override labels for specific files if needed
# If not in this map, label is inferred from filename ('human', 'vehicle', or 'background')
FILE_LABELS = {
    'human_running1.txt': 'human',
    'human_running2.txt': 'human',
    'vehicle1_clean.txt': 'vehicle',
    'vehicle2_clean.txt': 'vehicle',
    'vehicle3_clean.txt': 'vehicle',
    'vehicle4_clean.txt': 'vehicle'
}

TRAIN_RATIO = 0.8

FEATURE_NAMES = [
    "max_move_energy", "total_move_energy", "active_move_zones", "move_centroid", "move_spread",
    "max_stat_energy", "total_stat_energy", "active_stat_zones", "stat_centroid", "stat_spread",
    "move_change_rate", "stat_change_rate", "zone_switches", "motion_duration", "move_variance", "stat_variance",
    "zones_sequential", "zone_correlation", "weighted_peak_stability", "spatial_smoothness_index",
    "centroid_velocity_smoothed", "movement_linearity", "spatial_kurtosis", "spatial_skewness",
    "hjorth_mobility", "hjorth_complexity", "crest_factor", "spectral_entropy", "flicker_index", "flux"
]

def parse_log_file(filepath, true_label):
    """
    Parse log file and extract features.
    Supports both standard FEAT format and new 'features=' format.
    """
    features = []
    labels = []
    
    print(f"Parsing {filepath} (True Label: {true_label})...")
    
    if not os.path.exists(filepath):
        print(f"Warning: {filepath} not found!")
        return np.array([]), np.array([])

    with open(filepath, 'r') as f:
        for line in f:
            line = line.strip()
            if not line: continue
            
            # 1. Try 'features=' format (running/vehicle logs)
            if "features=" in line:
                try:
                    # Format: [timestamp] features=v1,v2...
                    content = line.split('features=')[1].strip()
                    vals = [float(x) for x in content.split(',')]
                    
                    if len(vals) == 30:
                        features.append(vals)
                        labels.append(true_label)
                except Exception:
                    continue
                    
            # 2. Try standard ',FEAT,' format
            elif ",FEAT," in line:
                try:
                    # Format: [timestamp] label,FEAT,val1,val2...
                    parts = line.split(',')
                    feat_idx = -1
                    for i, p in enumerate(parts):
                        if "FEAT" in p:
                            feat_idx = i
                            break
                    
                    if feat_idx == -1:
                        continue
                        
                    # Values are after FEAT
                    vals = parts[feat_idx+1:]
                    
                    # Clean values
                    clean_vals = []
                    for v in vals:
                        v = v.split(']')[0].strip()
                        clean_vals.append(float(v))
                        
                    if len(clean_vals) == 30:
                        features.append(clean_vals)
                        labels.append(true_label)
                        
                except Exception:
                    continue
                    
    return np.array(features), np.array(labels)

def main():
    import random
    random.seed(42)
    np.random.seed(42)
    
    # 1. Load All Data
    X_all_list = []
    y_all_list = []
    
    print("Loading data...")
    for log_dir, files in FILES_MAP.items():
        for filename in files:
            path = os.path.join(log_dir, filename)
            
            # Determine label
            if filename in FILE_LABELS:
                label = FILE_LABELS[filename]
            else:
                # Fallback inference
                fname_lower = filename.lower()
                if "human" in fname_lower:
                    label = "human"
                elif "vehicle" in fname_lower:
                    label = "vehicle"
                else:
                    label = "background"
                
            X, y = parse_log_file(path, label)
            if len(X) > 0:
                X_all_list.append(X)
                y_all_list.append(y)
                print(f"  -> {filename}: {len(X)} samples.")
            
    if not X_all_list:
        print("No data found!")
        return

    X_all = np.vstack(X_all_list)
    y_all = np.concatenate(y_all_list)
    
    classes, counts = np.unique(y_all, return_counts=True)
    print(f"\nTotal samples: {len(X_all)}")
    for cls, count in zip(classes, counts):
        print(f"  {cls}: {count}")
    
    # 2. Stratified split
    # Use sklearn's train_test_split which handles stratification nicely
    X_train, X_test, y_train, y_test = train_test_split(
        X_all, y_all, test_size=(1.0 - TRAIN_RATIO), stratify=y_all, random_state=42
    )

    print(f"\nTrain set: {len(X_train)}")
    train_classes, train_counts = np.unique(y_train, return_counts=True)
    for c, n in zip(train_classes, train_counts):
        print(f"  {c}: {n}")

    print(f"Test set:  {len(X_test)}")
    
    # 3. Train Model
    print("\nTraining LinearSVC...")
    le = LabelEncoder()
    y_train_enc = le.fit_transform(y_train)
    y_test_enc = le.transform(y_test)
    
    # Check encoding mapping
    print("Class mapping:")
    for i, class_name in enumerate(le.classes_):
        print(f"  {i} -> {class_name}")

    pipe = Pipeline([
        ('scaler', StandardScaler()),
        # multi_class='ovr' is default for LinearSVC. 
        # dual='auto' avoids warning in newer sklearn, but 'dual=True' logic was in v7. 
        # If n_samples > n_features, dual=False is usually preferred. Here we have ~5000 samples, 30 features.
        # Let's use dual=False for speed/convergence, or stick to auto.
        ('clf', LinearSVC(C=1.0, class_weight='balanced', max_iter=50000, dual=False)) 
    ])
    
    pipe.fit(X_train, y_train_enc)
    
    # 4. Evaluate
    print("\nEvaluating...")
    y_pred_train = pipe.predict(X_train)
    y_pred_test = pipe.predict(X_test)
    
    train_acc = accuracy_score(y_train_enc, y_pred_train)
    test_acc = accuracy_score(y_test_enc, y_pred_test)
    
    print(f"Train Accuracy: {train_acc:.4f}")
    print(f"Test Accuracy:  {test_acc:.4f}")
    
    print("\nClassification Report (Test):")
    print(classification_report(y_test_enc, y_pred_test, target_names=le.classes_))
    
    print("Confusion Matrix (Test):")
    print(confusion_matrix(y_test_enc, y_pred_test))
    
    # 5. Analyze Weights
    # For multi-class (One-vs-Rest), coef_ is (n_classes, n_features)
    # If binary, it's (1, n_features).
    clf = pipe.named_steps['clf']
    print(f"\nModel Coefficients shape: {clf.coef_.shape}")
    
    if clf.coef_.ndim == 2:
        # Multi-class case (or binary stored as 1xN)
        # If binary, n_classes=2 but coef is 1xN.
        # If multiclass (3), coef is 3xN.
        
        if len(le.classes_) == 2:
            # Binary case (similar to before)
            print("\nFeature Weights (Top 10):")
            coef = clf.coef_[0]
            indices = np.argsort(np.abs(coef))[::-1]
            for i in range(10):
                idx = indices[i]
                print(f"  {FEATURE_NAMES[idx]}: {coef[idx]:.4f}")
        else:
            # Multi-class: Print top features for EACH class
            # In OvR, each row corresponds to "Class X vs All Others"
            for i, class_name in enumerate(le.classes_):
                print(f"\nTop features for class '{class_name}' (vs others):")
                coef = clf.coef_[i]
                indices = np.argsort(np.abs(coef))[::-1]
                for k in range(5): # Top 5 per class
                    idx = indices[k]
                    print(f"  {FEATURE_NAMES[idx]}: {coef[idx]:.4f}")

    # 6. Save model
    os.makedirs('models', exist_ok=True)
    # Save as v8 to distinguish
    model_path = 'models/linear_svc_model.pkl' 
    joblib.dump({'pipeline': pipe, 'label_encoder': le, 'feature_names': FEATURE_NAMES}, model_path)
    print(f"\nSaved model to {model_path}")
    
if __name__ == "__main__":
    main()
