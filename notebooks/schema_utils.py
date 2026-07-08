import re
import os
import numpy as np
import pandas as pd

# Columns commonly present in your logs/datasets
_AGG_CANDIDATES = [
    'max_norm','total_norm','active_zones',
    'max_move_energy','total_move_energy','active_move_zones','move_centroid','move_spread','move_change_rate','motion_duration','move_variance',
    'max_stat_energy','total_stat_energy','active_stat_zones','stat_centroid','stat_spread','stat_change_rate','stat_variance',
    'zone_switches','zones_sequential','zone_correlation'
]

_SESSION_CANDIDATES = ['session_id','session','recording_id','recording','seq_id']
_SORT_CANDIDATES = ['timestep','timestamp','frame','time']

_LABEL_CANDIDATES = ['label','class','target']


def _sorted_numeric_suffix(cols, prefix):
    pat = re.compile(r'^' + re.escape(prefix) + r'(\d+)$')
    matched = []
    for c in cols:
        m = pat.match(c)
        if m:
            matched.append((int(m.group(1)), c))
    return [c for _, c in sorted(matched)]


def detect_feature_columns(df: pd.DataFrame):
    cols = list(df.columns)
    # Priority: normalized zones -> move zones -> stat zones -> numeric fallback
    norm_cols = _sorted_numeric_suffix(cols, 'norm_z')
    move_cols = _sorted_numeric_suffix(cols, 'move')
    stat_cols = _sorted_numeric_suffix(cols, 'stat')

    if norm_cols:
        zone_cols = norm_cols
        zone_source = 'norm_z'
    elif move_cols and stat_cols:
        # Prefer move zones for spatial patterns; stat zones available separately
        zone_cols = move_cols
        zone_source = 'move'
    elif move_cols:
        zone_cols = move_cols
        zone_source = 'move'
    elif stat_cols:
        zone_cols = stat_cols
        zone_source = 'stat'
    else:
        # Fallback: all numeric except obvious non-feature columns
        exclude = set(_SESSION_CANDIDATES + _SORT_CANDIDATES + _LABEL_CANDIDATES)
        numeric_cols = df.select_dtypes(include=['number']).columns.tolist()
        zone_cols = [c for c in numeric_cols if c not in exclude]
        zone_source = 'fallback_numeric'

    agg_cols = [c for c in _AGG_CANDIDATES if c in cols]
    label_col = next((c for c in _LABEL_CANDIDATES if c in cols), None)

    classic_feature_cols = zone_cols + agg_cols
    return {
        'zone_cols': zone_cols,
        'agg_cols': agg_cols,
        'classic_feature_cols': classic_feature_cols,
        'label_col': label_col,
        'zone_source': zone_source,
    }


def build_feature_matrix(df: pd.DataFrame, feature_cols):
    missing = [c for c in feature_cols if c not in df.columns]
    if missing:
        raise ValueError(f"Missing feature columns: {missing}")
    return df[feature_cols].values


def infer_session_sort_columns(df: pd.DataFrame):
    session_col = next((c for c in _SESSION_CANDIDATES if c in df.columns), None)
    sort_col = next((c for c in _SORT_CANDIDATES if c in df.columns), None)
    if session_col is None:
        session_col = '__synthetic_session__'
        df[session_col] = 0
    if sort_col is None:
        sort_col = '__row_index__'
        df[sort_col] = np.arange(len(df))
    return session_col, sort_col


def create_sequences(df: pd.DataFrame, seq_len: int, feature_cols, label_col: str, session_col: str, sort_col: str):
    X_seq = []
    y_seq = []
    for sid, g in df.groupby(session_col):
        g_sorted = g.sort_values(sort_col)
        Xg = g_sorted[feature_cols].values
        yg = g_sorted[label_col].astype(str).values if label_col else None
        if len(Xg) < seq_len:
            continue
        for i in range(len(Xg) - seq_len + 1):
            X_seq.append(Xg[i:i+seq_len])
            if yg is not None:
                y_seq.append(yg[i+seq_len-1])
    X_seq = np.asarray(X_seq)
    y_seq = np.asarray(y_seq) if y_seq else None
    return X_seq, y_seq