import os
import glob
import numpy as np
from sklearn.svm import LinearSVC
from sklearn.preprocessing import StandardScaler, LabelEncoder
from sklearn.pipeline import Pipeline
from sklearn.metrics import confusion_matrix

# Configuration
LOG_DIR = 'logs/06'
TRAIN_FILES = ['human1_clean.txt', 'human2_clean.txt', 'background1.txt', 'background2.txt']
TEST_FILES = ['human3_clean.txt', 'human4_clean.txt', 'background3.txt', 'background4.txt']

FEATURE_NAMES = [
    "max_move_energy", "total_move_energy", "active_move_zones", "move_centroid", "move_spread",
    "max_stat_energy", "total_stat_energy", "active_stat_zones", "stat_centroid", "stat_spread",
    "move_change_rate", "stat_change_rate", "zone_switches", "motion_duration", "move_variance", "stat_variance",
    "zones_sequential", "zone_correlation", "weighted_peak_stability", "spatial_smoothness_index",
    "centroid_velocity_smoothed", "movement_linearity", "spatial_kurtosis", "spatial_skewness",
    "hjorth_mobility", "hjorth_complexity", "crest_factor", "spectral_entropy", "flicker_index", "flux"
]

def parse_log_file_with_metadata(filepath, true_label):
    features = []
    labels = []
    metadata = [] # stores (filename, line_idx, raw_line_content)
    
    print(f"Parsing {filepath} (Label: {true_label})...")
    
    with open(filepath, 'r') as f:
        for line_idx, line in enumerate(f):
            if ",FEAT," in line:
                try:
                    parts = line.strip().split(',')
                    feat_idx = -1
                    for i, p in enumerate(parts):
                        if "FEAT" in p:
                            feat_idx = i
                            break
                    
                    if feat_idx == -1:
                        continue
                        
                    vals = parts[feat_idx+1:]
                    clean_vals = []
                    for v in vals:
                        v = v.split(']')[0].strip()
                        clean_vals.append(float(v))
                        
                    if len(clean_vals) == 30:
                        features.append(clean_vals)
                        labels.append(true_label)
                        metadata.append((os.path.basename(filepath), line_idx + 1, line.strip()))
                        
                except Exception as e:
                    continue
                    
    return np.array(features), np.array(labels), metadata

def main():
    # 1. Load Training Data (Metadata not needed for training set, but needed for consistency if I reused func)
    print("Loading Training Data...")
    X_train_list = []
    y_train_list = []
    
    for filename in TRAIN_FILES:
        path = os.path.join(LOG_DIR, filename)
        label = "human" if "human" in filename else "background"
        X, y, _ = parse_log_file_with_metadata(path, label)
        if len(X) > 0:
            X_train_list.append(X)
            y_train_list.append(y)
            
    if not X_train_list:
        print("No training data found!")
        return

    X_train = np.vstack(X_train_list)
    y_train = np.concatenate(y_train_list)
    
    # 2. Load Test Data (Need metadata here!)
    print("\nLoading Test Data...")
    X_test_list = []
    y_test_list = []
    metadata_test_list = []
    
    for filename in TEST_FILES:
        path = os.path.join(LOG_DIR, filename)
        label = "human" if "human" in filename else "background"
        X, y, meta = parse_log_file_with_metadata(path, label)
        if len(X) > 0:
            X_test_list.append(X)
            y_test_list.append(y)
            metadata_test_list.extend(meta)

    if not X_test_list:
        print("No test data found!")
        return
        
    X_test = np.vstack(X_test_list)
    y_test = np.concatenate(y_test_list)
    
    # 3. Train Model
    print("\nTraining LinearSVC...")
    le = LabelEncoder()
    y_train_enc = le.fit_transform(y_train)
    y_test_enc = le.transform(y_test)
    
    # Correct order: background=0, human=1 usually, but let's check
    # sorted(unique) -> background, human -> index 0, 1
    class_names = le.classes_
    print(f"Classes: {class_names}")
    
    pipe = Pipeline([
        ('scaler', StandardScaler()),
        ('clf', LinearSVC(C=1.0, class_weight='balanced', max_iter=10000, dual=True))
    ])
    
    pipe.fit(X_train, y_train_enc)
    
    # 4. Predict and Find Failures
    print("\nPredicting...")
    y_pred = pipe.predict(X_test)
    
    print("Finding Misclassifications...")
    misclassified_indices = np.where(y_test_enc != y_pred)[0]
    
    print(f"\nFound {len(misclassified_indices)} misclassified samples.")
    
    if len(misclassified_indices) == 0:
        return
        
    print("\n" + "="*80)
    print("MISCLASSIFICATION REPORT")
    print("="*80)
    
    # Get feature weights to highlight important ones?
    # Or just print the key features: move_variance, etc.
    # From previous training, top features were: move_variance, flicker_index, etc.
    key_features_idx = [FEATURE_NAMES.index("move_variance"), 
                        FEATURE_NAMES.index("active_stat_zones"), 
                        FEATURE_NAMES.index("stat_variance")]
    
    for idx in misclassified_indices:
        actual_label = class_names[y_test_enc[idx]]
        pred_label = class_names[y_pred[idx]]
        filename, line_num, raw_content = metadata_test_list[idx]
        features = X_test[idx]
        
        type_str = "FALSE POSITIVE" if pred_label == "human" else "FALSE NEGATIVE"
        
        print(f"\n[{type_str}] File: {filename} | Line: {line_num}")
        print(f"Expected: {actual_label} | Predicted: {pred_label}")
        print(f"Key Features:")
        print(f"  move_variance:     {features[FEATURE_NAMES.index('move_variance')]:.4f}")
        print(f"  active_stat_zones: {features[FEATURE_NAMES.index('active_stat_zones')]:.1f}")
        print(f"  stat_variance:     {features[FEATURE_NAMES.index('stat_variance')]:.4f}")
        print("-" * 40)
        print(f"Raw Line: {raw_content[:150]}..." if len(raw_content) > 150 else f"Raw Line: {raw_content}") 

if __name__ == "__main__":
    main()
