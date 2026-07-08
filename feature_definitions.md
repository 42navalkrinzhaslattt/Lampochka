# Feature Definitions for LD2412 Human Detection

This document outlines the derived features used to train the SVM model. All features are calculated based on **Normalized Distance-to-Set** values, not raw sensor energy. 

## 1. Input Data: Normalized Distance
*   **Source:** `BaselineManager::distanceToSet()`
*   **Logic:** The raw energy value of each gate (0-255) is compared to a sorted set of historical background values for that specific gate.
*   **Value:** `|CurrentValue - ClosestBackgroundValue|`
*   **Meaning:** 
    *   `0`: Indistinguishable from background.
    *   `>1`: Distinctly different (potential signal).

---

## 2. Spatial Features (Shape & Distribution)
These features analyze the distribution of energy across the 14 range gates (0.75m to 6.0m) at a single moment in time.

### Energy Metrics
*   **`max_move_energy` / `max_stat_energy`**
    *   **Def:** The highest single normalized value across all 14 gates.
    *   **Significance:** Peak signal strength. Humans usually create a strong peak (>15-20).
*   **`total_move_energy` / `total_stat_energy`**
    *   **Def:** Sum of normalized values across all 14 gates.
    *   **Significance:** Overall intensity. Rain/wind might create high total energy but low peak energy (distributed).
*   **`active_move_zones` / `active_stat_zones`**
    *   **Def:** Count of gates with Normalized Value > `1.0`.
    *   **Significance:** Size of the target.
        *   **Human:** typically 2-4 zones (focused).
        *   **Noise:** often 0-1 (sparkle) or >8 (wide interference).

### Shape Metrics
*   **`move_centroid` / `stat_centroid`**
    *   **Def:** Weighted average of gate indices (Center of Gravity).
    *   **Formula:** `Sum(GateIndex * Energy) / TotalEnergy`
    *   **Significance:** Average distance of the target. Usage of `GateIndex` (0-13) represents distance in meters, not importance weight.
*   **`move_spread` / `stat_spread`**
    *   **Def:** Variance of the energy distribution around the centroid.
    *   **Formula:** `Sum( Energy * (GateIndex - Centroid)^2 ) / TotalEnergy`
    *   **Significance:** Width/Focus of the target.
        *   **Human:** Low spread (< 4.0). A focused "clump" of reflection.
        *   **Noise:** High spread (> 10.0). Background noise often triggers gates far apart (e.g., Gate 0 and Gate 13), mathematically resulting in values up to ~49.

---

## 3. Pattern Features (Texture)
These features analyze the relationship between adjacent gates to distinguish solid objects from random noise.

*   **`zone_correlation`** (Most Important Feature)
    *   **Def:** Measure of "smoothness" between neighbors.
    *   **Logic:** Increments if Gate `i` and Gate `i+1` are both Active or both Inactive.
    *   **Significance:** 
        *   **High (near 1.0):** Solid object. Gates 4, 5, 6 are all active (smooth block).
        *   **Low:** Spiky noise. Gate 4 active, Gate 5 inactive, Gate 6 active (salt-and-pepper pattern).
*   **`zones_sequential`**
    *   **Def:** Binary (0 or 1). True if all active gates form a single contiguous block (no gaps).
    *   **Significance:** Humans are solid constraints objects. Gaps in detection usually indicate noise.

---

## 4. Temporal Features (Behavior over Time)
These features analyze changes over a sliding window (1.0 second / 5 frames).

*   **`motion_duration`**
    *   **Def:** Seconds elapsed since any gate first exceeded the threshold. Resets to 0 when all gates return to baseline.
    *   **Significance:** Persistence. Humans typically exist for >0.5s. Electrical noise is often transient (<0.2s).
*   **`move_variance` / `stat_variance`**
    *   **Def:** Variance of the signal intensity over the last 5 frames.
    *   **Significance:** Stability.
        *   **Human:** Smooth motion (Lower high-frequency variance).
        *   **Noise:** Jittery/Flickering (High temporal variance).
*   **`move_change_rate` / `stat_change_rate`**
    *   **Def:** Difference in Total Energy between Now and 2 frames ago (0.4s delta).
    *   **Significance:** Acceleration of signal. Detects sudden entry/exit.
*   **`zone_switches`**
    *   **Def:** Binary (0 or 1). True if the "strongest" gate index changed compared to 2 frames ago.
    *   **Significance:** Movement across space. Humans walk (changing distance). Static interference stays in one place.

---

## 5. NEW Physics-Based Features (v2)
These features were added to improve noise rejection and human movement detection.

### A. Stability & Smoothness (Replace zone_switches/zone_correlation)
*   **`weighted_peak_stability`** (f47, replaces zone_switches)
    *   **Formula:** `|I_max(t) - I_max(t-2)| × log(1 + E_max(t))`
    *   **Logic:** How far did the peak move, weighted by how strong the signal is.
    *   **Significance:** Low = static (background or still human), High = moving human or sudden noise.

*   **`spatial_smoothness_index`** (f48, replaces zone_correlation)
    *   **Formula:** `Σ|E_i - E_{i+1}| / (Σ E_i + ε)`
    *   **Logic:** Total Variation normalized by Total Energy.
    *   **Significance:** ~0 = flat background, Low = smooth human, High = spiky noise.

### B. Kinematics (Trajectory Analysis)
*   **`centroid_velocity_smoothed`** (f49)
    *   **Logic:** Linear regression slope of centroid over last 5 frames (Savitzky-Golay-like).
    *   **Significance:** Positive = approaching, Negative = departing, ~0 = stationary.

*   **`movement_linearity`** (f50)
    *   **Formula:** R² (coefficient of determination) of centroid over last 10 frames.
    *   **Significance:** ~1.0 = straight-line motion (human walking), ~0 = random jitter (noise/insects).

### C. Statistical Shape (Peakedness & Asymmetry)
*   **`spatial_kurtosis`** (f51)
    *   **Formula:** `(Σ(E_i - μ)⁴ / N) / σ⁴ - 3`
    *   **Significance:** High = energy concentrated in peak (sharp human reflection), Low = flat/uniform.

*   **`spatial_skewness`** (f52)
    *   **Formula:** `(Σ(E_i - μ)³ / N) / σ³`
    *   **Significance:** Asymmetric signals (human edge) vs symmetric noise.

### D. Time-Series Texture (Signal Dynamics)
*   **`hjorth_mobility`** (f53)
    *   **Formula:** `√(Var(d¹E) / Var(E))`
    *   **Significance:** Estimates mean frequency. Low = slow changes (breathing), High = fast flicker (noise).

*   **`hjorth_complexity`** (f54)
    *   **Formula:** `Mobility(d¹E) / Mobility(E)`
    *   **Significance:** Similarity to pure sine wave. Detects "rhythm" of the signal.

### E. Signal Quality Metrics
*   **`crest_factor`** (f55)
    *   **Formula:** `MaxEnergy / MeanEnergy`
    *   **Significance:** High = distinct peak (solid human), Low = muddy/distributed (rain/wind).

*   **`spectral_entropy`** (f56)
    *   **Formula:** `-Σ p_i × log₂(p_i)` where `p_i = E_i / ΣE`
    *   **Significance:** Low = energy in few gates (human), High = energy spread everywhere (noise).

*   **`flicker_index`** (f57)
    *   **Logic:** Zero-crossing count of total energy around its mean over last 20 frames.
    *   **Significance:** High = rapid on/off flickering (50Hz hum, sparks), Low = stable presence.

*   **`flux`** (f58)
    *   **Formula:** `Σ|E_i(t) - E_i(t-1)|` for all gates
    *   **Significance:** Total inter-frame change. 0 = static, moderate = walking human, extreme = sudden burst.

