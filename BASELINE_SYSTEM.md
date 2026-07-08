# Baseline Collection System Documentation

## Overview

The baseline collection system captures the "normal empty environment" reference for your radar sensor. This baseline removes the influence of walls, furniture, and environmental clutter, making your ML model work across different installation locations.

## System Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    BASELINE COLLECTION WORKFLOW              │
└─────────────────────────────────────────────────────────────┘

1. WARM-UP (2 minutes)
   ↓ Sensor stabilization
   
2. COLLECTION (5 minutes, 300 samples @ 1Hz)
   ↓ Store raw gate energies
   
3. PROCESSING (Outlier Rejection)
   ↓ Use MAD method to remove birds/insects/debris
   ↓ Calculate: mean, std, median, min, max, 95th percentile
   
4. VALIDATION
   ↓ Variance check (stability)
   ↓ Spatial continuity check
   ↓ Live detection test
   
5. STORAGE (EEPROM with CRC)
   ↓ Magic number + version + checksum
   
6. RUNTIME USAGE
   → normalized_value = (raw - mean) / std
```

## File Structure

```
record_data_ld2412_csv/
├── record_data_ld2412_csv.ino    Main loop + command handler
├── config.h                       Sensor constants
├── baseline.h                     Baseline data structures
├── baseline_stats.ino             Statistical functions
├── baseline_collect.ino           Collection logic
├── baseline_process.ino           Outlier rejection
├── baseline_storage.ino           EEPROM persistence
├── baseline_validate.ino          Quality checks
├── sensor_config.ino              Sensor configuration
├── sensor_comm.ino                Frame reading/parsing
└── features.ino                   Feature calculations
```

## Commands

### CALIBRATE
Collects a new baseline. Full 5-minute process:
- **Requirements:** Area must be completely clear (no people/animals)
- **Duration:** 7 minutes total (2 min warmup + 5 min collection)
- **Process:** Collects 300 samples, rejects outliers, calculates statistics
- **Output:** Saves to EEPROM automatically

**Usage:**
```
> CALIBRATE
[Follow on-screen prompts]
```

### LOAD
Loads baseline from EEPROM (if it exists):
- Validates magic number
- Checks version compatibility
- Verifies CRC checksum
- Shows age of baseline

**Usage:**
```
> LOAD
Loading baseline from EEPROM... OK
  Loaded 14 gates, 300 samples
  Age: 25 minutes
```

### VALIDATE
Tests baseline quality (three checks):
1. **Variance Check:** std < 0.5 * mean for all gates
2. **Spatial Continuity:** Adjacent gates similar (< 30 difference)
3. **Live Detection Test:** Walk in front, verify detection triggers

**Usage:**
```
> VALIDATE
[Follow on-screen test instructions]
```

### STATUS
Shows current system state:
- Baseline loaded? (yes/no)
- Baseline statistics table
- Sensor configuration
- Samples collected

**Usage:**
```
> STATUS
========================================
SYSTEM STATUS
========================================

Baseline: LOADED ✓

Baseline Statistics:
  Gate |  Mean  |  Std   | Median |  Min   |  Max   | 95th %
  -----|--------|--------|--------|--------|--------|--------
     0 |   45.2 |   3.1  |   45.0 |   38.0 |   52.0 |   50.1
     1 |   42.8 |   2.9  |   43.0 |   36.0 |   49.0 |   47.5
  ...
```

### CLEAR
Clears baseline from EEPROM:
- Invalidates stored baseline
- Clears in-memory arrays
- Requires recalibration before use

**Usage:**
```
> CLEAR
Clearing baseline from EEPROM... OK
```

### NORMALIZE
Tests normalization on current sensor reading:
- Shows raw vs normalized values
- Normalized values in standard deviations from baseline mean
- Useful for debugging

**Usage:**
```
> NORMALIZE
Current Reading (Normalized):
  Gate | Raw Move | Normalized | Raw Stat | Normalized
  -----|----------|------------|----------|------------
     0 |    48    |     0.90   |    95    |     2.15
     1 |    45    |     0.76   |    92    |     1.88
  ...
```

## Outlier Rejection Algorithm

Uses **Median Absolute Deviation (MAD)** for robust outlier detection:

```
For each gate:
  1. Calculate median (robust center)
  2. Calculate MAD = median(|value - median|) * 1.4826
  3. Reject samples where |value - median| > 3 * MAD
  4. Calculate statistics from clean data
```

**Why MAD?**
- More robust than standard deviation
- Not affected by outliers (birds, insects)
- Works well with non-normal distributions

## Validation Criteria

### 1. Variance Check
```
std / mean < 0.5 for all gates
```
**Meaning:** Environment was stable during collection  
**Failure:** High wind, movement, or sensor issues

### 2. Spatial Continuity
```
|mean[i] - mean[i+1]| < 30 for adjacent gates
```
**Meaning:** Smooth spatial distribution  
**Warning:** Large jumps may indicate obstacles

### 3. Live Detection Test
```
Detection triggered in >= 3 gates during 5-second test
```
**Meaning:** Sensor responds to movement  
**Failure:** Bad baseline or sensor malfunction

## Runtime Normalization

After baseline is loaded, normalize sensor readings:

```cpp
float normalized = (raw_value - baseline_mean[gate]) / baseline_std[gate];
```

**Interpretation:**
- `normalized = 0` → Same as baseline
- `normalized = 1` → One std above baseline
- `normalized = 2` → Two stds above baseline (likely detection)
- `normalized = -1` → Below baseline (rare)

**Benefits:**
- Environment-independent features
- ML model works across locations
- Comparable measurements

## EEPROM Storage Format

```
Offset | Size | Content
-------|------|----------------------------------
  0    |  4   | Magic number (0xBEEFCAFE)
  4    |  1   | Version (1)
  5    |  1   | Number of gates (14)
  6    |  2   | Sample count (300)
  8    |  4   | Timestamp (millis)
  12   |  56  | Mean array (14 x 4 bytes)
  68   |  56  | Std array (14 x 4 bytes)
  124  |  56  | Median array (14 x 4 bytes)
  180  |  56  | Min array (14 x 4 bytes)
  236  |  56  | Max array (14 x 4 bytes)
  292  |  56  | 95th percentile (14 x 4 bytes)
  348  |  2   | CRC16 checksum
-------|------|----------------------------------
Total: 350 bytes
```

## Memory Usage

**During Collection:**
- Raw buffers: 300 samples × 14 gates × 2 (moving+stationary) = **8,400 bytes**
- Statistics arrays: 14 gates × 6 stats × 4 bytes = **336 bytes**
- **Total: ~8.7 KB** (freed after collection)

**Runtime (No Collection):**
- Statistics arrays only: **336 bytes**

## Best Practices

### Collection Environment
✅ **DO:**
- Use in final installation location
- Clear area completely
- Stable conditions (no wind)
- Point at detection zone
- Run during quiet time

❌ **DON'T:**
- Collect with obstacles present
- Collect in high wind
- Move during collection
- Use different mounting position

### When to Recalibrate
- **Always:** After moving sensor
- **Recommended:** Weekly for outdoor
- **Optional:** Monthly for indoor stable environments
- **Required:** After validation failure

### Troubleshooting

**"High variance" warnings:**
- Environment not stable (wind, movement)
- Sensor too close to reflective surfaces
- → Recollect in calmer conditions

**"No detection" in live test:**
- Baseline too high (sensor saturated)
- Sensor not responding
- → Check sensor placement, recollect

**"Spatial discontinuities":**
- Obstacle blocking some gates
- Sensor misaligned
- → Check mounting, may be OK

## Integration with ML Training

Use normalized values for training:

```python
# In your training script
normalized_features = (raw_features - baseline_mean) / baseline_std

# Train model on normalized features
model.fit(normalized_features, labels)
```

**Result:** Model works across different sensor installations!

## Example Session

```
> CALIBRATE
========================================
BASELINE COLLECTION
========================================

IMPORTANT: Area must be COMPLETELY CLEAR
Type 'START' to begin or 'CANCEL' to abort:
> START

Allocating memory... OK

Phase 1: Sensor Warmup (2 minutes)
  Allowing sensor to stabilize...
  10%
  20%
  ...
  Warmup complete!

Phase 2: Data Collection (5 minutes)
  Collecting 300 samples at 1 Hz...
  10% (30/300)
  20% (60/300)
  ...
  Collection complete!

Phase 3: Processing with Outlier Rejection
  Gate 0: mean=45.2 std=3.1 (8 outliers removed)
  Gate 1: mean=42.8 std=2.9 (12 outliers removed)
  ...
  
  Total outliers removed: 156 / 4200 (3%)

Baseline Statistics:
  [Table displayed]

Saving baseline to EEPROM... OK
  Stored 350 bytes at address 0

Baseline calibration complete!
Run 'VALIDATE' to test quality.

> VALIDATE
Phase 4: Baseline Validation

Check 1: Variance Analysis
  PASS: All gates have acceptable variance

Check 2: Spatial Continuity
  PASS: Spatial distribution looks normal

Check 3: Live Detection Test
  NOW! Move in front of sensor...
  Test complete!
  Max active gates: 7
  Detection frames: 42 / ~100
  PASS: Sensor responds to movement

========================================
VALIDATION PASSED ✓
Baseline is ready for use!
========================================
```

## Technical Details

**Sample Rate:** 1 Hz (one sample per second)  
**Collection Time:** 5 minutes = 300 samples  
**Outlier Threshold:** 3 × MAD (rejects ~0.3% of normal data)  
**Storage:** Non-volatile EEPROM with CRC protection  
**Normalization:** Z-score (standard score)  

---

**Ready to use! Type `HELP` in Serial Monitor to get started.** 🎯
