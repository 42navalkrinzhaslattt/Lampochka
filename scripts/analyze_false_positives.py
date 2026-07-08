#!/usr/bin/env python3
"""
Analyze False Positives (FP) from the v2 model.
For each FP (Background predicted as Human), find the closest True Positives (Human predicted as Human).

Method:
1. Identify all FPs and TPs.
2. Use scaled feature space (what the model sees) to calculate Manhattan distance.
3. Show RAW values for inspection.
"""

import os
import joblib
import numpy as np
import pandas as pd
from sklearn.metrics import pairwise_distances

MODEL_PATH = 'models/LinearSVC_v4.pkl'
TEST_PATH = 'datasets/training_data_test_v4.csv'

# Columns to display in report (subset of interesting ones)
DISPLAY_COLS = [
    'f58',  # flux
    'f55',  # crest_factor
    'f51',  # spatial_kurtosis
    'f57',  # flicker_index
    'f29',  # max_move_energy
    'f30',  # total_move_energy
    'f33',  # move_spread
    'f47',  # weighted_peak_stability
    'f48',  # spatial_smoothness_index
]

# Map friendly names for display
COL_NAMES = {
    'f58': 'flux',
    'f55': 'crest_fac',
    'f51': 'kurtosis',
    'f57': 'flicker',
    'f29': 'max_mv',
    'f30': 'tot_mv',
    'f33': 'mv_spread',
    'f47': 'stability',
    'f48': 'smooth',
}

def main():
    if not os.path.exists(MODEL_PATH) or not os.path.exists(TEST_PATH):
        print("Model or test data not found.")
        return

    # Load model
    print(f"Loading model {MODEL_PATH}...")
    model_data = joblib.load(MODEL_PATH)
    pipeline = model_data['pipeline']
    feature_cols = model_data['feature_cols']
    
    # Extract scaler and clf
    scaler = pipeline.named_steps['scaler']
    clf = pipeline.named_steps['clf']

    # Load data
    df = pd.read_csv(TEST_PATH)
    X_raw = df[feature_cols].values
    y_true = df['label'].values
    
    # Get model predictions
    X_scaled = scaler.transform(X_raw)
    y_pred = pipeline.predict(X_raw)
    
    # Map string labels to match (assuming 'human' is the positive class)
    # The pipeline might output 0/1 or strings depending on how it was trained.
    # Looking at train_linear_svc_v2.py, it uses LabelEncoder.
    # We need to re-create the mapping.
    le = model_data['label_encoder']
    y_pred_str = le.inverse_transform(y_pred)
    
    # Identify FPs and TPs
    # FP: True=nothing, Pred=human
    fp_mask = (y_true == 'nothing') & (y_pred_str == 'human')
    # TP: True=human, Pred=human
    tp_mask = (y_true == 'human') & (y_pred_str == 'human')
    
    fp_indices = np.where(fp_mask)[0]
    tp_indices = np.where(tp_mask)[0]
    
    print(f"\nFound {len(fp_indices)} False Positives (Background -> Human)")
    print(f"Found {len(tp_indices)} True Positives (Human -> Human)")
    
    if len(fp_indices) == 0:
        print("No False Positives to analyze!")
        return

    # Subsets for distance calc
    X_fp_scaled = X_scaled[fp_indices]
    X_tp_scaled = X_scaled[tp_indices]
    
    # Calculate Manhattan distances (L1) between every FP and all TPs
    # Result shape: (n_fps, n_tps)
    dists = pairwise_distances(X_fp_scaled, X_tp_scaled, metric='manhattan')
    
    # Display Report
    print(f"\n{'='*100}")
    print(f"ANALYSIS OF FALSE POSITIVES")
    print(f"Distance Metric: Manhattan (L1) on Scaled Features")
    print(f"{'='*100}")

# Map CSV rows to Log Files
# Based on clean_dataset.py / augment_logs.py counts:
# 1. human_test_clean.txt: 627 rows
# 2. background_test.txt: 382 rows
HUMAN_COUNT = 627
LOG_DIR = 'logs/05'

def get_raw_log_lines(row_idx):
    """Map CSV row index to raw log lines."""
    if row_idx < HUMAN_COUNT:
        filename = 'human_test_clean.txt'
        sample_idx = row_idx
    else:
        filename = 'background_test.txt'
        sample_idx = row_idx - HUMAN_COUNT
        
    path = os.path.join(LOG_DIR, filename)
    if not os.path.exists(path):
        return f"[File not found: {path}]"
        
    # Read file and find the N-th sample (GATE+FEAT lines)
    # This is a simple parser assuming the file structure is consistent
    with open(path, 'r', errors='ignore') as f:
        lines = f.readlines()
        
    current_sample = 0
    captured_lines = []
    
    i = 0
    while i < len(lines):
        line = lines[i].strip()
        if not line:
            i += 1
            continue
            
        # Check for GATE line
        if ',GATE,' in line:
            if current_sample == sample_idx:
                # Found it! Get this line and the next (FEAT)
                raw_gate = lines[i].strip()
                raw_feat = lines[i+1].strip() if i+1 < len(lines) else "[Missing FEAT line]"
                return f"{filename}:{sample_idx}\n    {raw_gate}\n    {raw_feat}"
            
            current_sample += 1
            # Skip the next line (FEAT) as we just counted this sample
            # But be careful if logic requires strictly checking pairs. 
            # augment_logs.py parses pairs. Let's assume standard pairs.
        
        i += 1
            
    return f"[Sample {sample_idx} not found in {filename}]"

def main():
    if not os.path.exists(MODEL_PATH) or not os.path.exists(TEST_PATH):
        print("Model or test data not found.")
        return

    # Load model
    print(f"Loading model {MODEL_PATH}...")
    model_data = joblib.load(MODEL_PATH)
    pipeline = model_data['pipeline']
    feature_cols = model_data['feature_cols']
    
    # Extract scaler and clf
    scaler = pipeline.named_steps['scaler']
    
    # Load data
    df = pd.read_csv(TEST_PATH)
    X_raw = df[feature_cols].values
    y_true = df['label'].values
    
    # Get model predictions
    # Note: Model was trained on encoded labels, need to ensure consistent mapping
    # The pipeline outputs numeric or string depending on training. 
    # v2 script used LabelEncoder.
    X_scaled = scaler.transform(X_raw)
    y_pred = pipeline.predict(X_raw)
    
    label_encoder = model_data['label_encoder']
    y_pred_str = label_encoder.inverse_transform(y_pred)
    
    # Identify FPs and TPs
    # FP: True=nothing, Pred=human
    fp_mask = (y_true == 'nothing') & (y_pred_str == 'human')
    # TP: True=human, Pred=human
    tp_mask = (y_true == 'human') & (y_pred_str == 'human')
    
    fp_indices = np.where(fp_mask)[0]
    tp_indices = np.where(tp_mask)[0]
    
    print(f"\nFound {len(fp_indices)} False Positives (Background -> Human)")
    print(f"Found {len(tp_indices)} True Positives (Human -> Human)")
    
    if len(fp_indices) == 0:
        print("No False Positives to analyze!")
        return

    # Subsets for distance calc
    X_fp_scaled = X_scaled[fp_indices]
    X_tp_scaled = X_scaled[tp_indices]
    
    # Calculate Manhattan distances
    dists = pairwise_distances(X_fp_scaled, X_tp_scaled, metric='manhattan')
    
    # Sort by distance (most "confusing" first)
    min_dists = np.min(dists, axis=1)
    sorted_indices = np.argsort(min_dists)
    
    # Header for the table
    header_items = [f"{COL_NAMES.get(c, c):>10}" for c in DISPLAY_COLS]
    header_str = "  ".join(header_items)
    
    # OUTPUT FILE
    report_path = os.path.join(LOG_DIR, 'false_positives_report.txt')
    
    with open(report_path, 'w') as f:
        f.write("="*100 + "\n")
        f.write(f"FALSE POSITIVE ANALYSIS REPORT\n")
        f.write(f"Total False Positives: {len(fp_indices)}\n")
        f.write(f"Timestamp: {pd.Timestamp.now()}\n")
        f.write("="*100 + "\n\n")
        
        for i in sorted_indices:
            fp_idx = fp_indices[i]
            original_idx = df.index[fp_idx]
            
            # Find closest TP
            closest_tp_local_idx = np.argmin(dists[i])
            closest_tp_idx = tp_indices[closest_tp_local_idx]
            min_dist = dists[i][closest_tp_local_idx]
            
            f.write(f"FP #{i+1} (Row {original_idx}) | Dist to TP: {min_dist:.2f}\n")
            f.write("-" * 100 + "\n")
            
            # 1. Print Feature Table
            f.write(f"{'TYPE':>12} " + header_str + "\n")
            fp_vals = df.iloc[fp_idx][DISPLAY_COLS].values
            fp_val_str = "  ".join([f"{v:>10.2f}" for v in fp_vals])
            f.write(f"{'FALSE POS':>12} " + fp_val_str + "\n")
            
            tp_vals = df.iloc[closest_tp_idx][DISPLAY_COLS].values
            tp_val_str = "  ".join([f"{v:>10.2f}" for v in tp_vals])
            f.write(f"{'CLOSEST TP':>12} " + tp_val_str + "\n")
            
            # 2. Print Raw Log Lines
            f.write("\n[RAW LOG LINE - FALSE POSITIVE]\n")
            f.write(get_raw_log_lines(original_idx) + "\n")
            
            f.write("\n[RAW LOG LINE - CLOSEST TRUE POSITIVE]\n")
            f.write(get_raw_log_lines(closest_tp_idx) + "\n")
            f.write("."*100 + "\n\n")

    print(f"Report written to: {report_path}")
    print(f"Analyzed {len(fp_indices)} False Positives.")

if __name__ == '__main__':
    main()
