# Lampochka

Embedded mmWave radar experiments for detecting humans, vehicles and rule-based targets with HLK-LD2412 sensors: raw gate energies are calibrated against the empty scene, turned into a 30-value feature vector and classified on the microcontroller itself. The repository combines Arduino/Pico sketches, a reusable LD2412 toolkit library, Python training/export scripts, captured logs and notebook experiments.

**Stack:** C++ (Arduino, RP2040 / Pico 2W) · Python · scikit-learn · FastAPI    **Context:** Lampochka, hardware startup I co-founded (2024–2025)

## What this project does

Lampochka reads LD2412 engineering-mode gate-energy frames, calibrates a local background baseline, converts raw moving/stationary gate values into environment-relative features, and runs embedded classifiers on microcontrollers.

Current embedded paths include:

- **Single-sensor Arduino/MKR sketches** for data collection and LinearSVC inference.
- **Raspberry Pi Pico 2W multi-sensor sketches** for four LD2412 sensors using hardware UART plus `SerialPIO`.
- **LinearSVC models** exported to C/C++ headers for human / vehicle classification.
- **Random Forest and threshold-based detectors** for compact target/background experiments.
- **Python utilities** for serial capture, live web dashboards, simulation playback, dataset cleaning, training, model export, and error analysis.

## Repository layout

```text
.
├── libraries/LD2412Toolkit/     # Reusable Arduino library for LD2412 parsing, calibration, features
├── sketches/                    # Arduino sketches for recorders, detectors, Pico 2W variants, tests
│   ├── record_data/             # Single-sensor CSV / FEAT logger
│   ├── record_data_pico2w/      # Four-sensor Pico 2W logger
│   ├── human/                   # Human-only LinearSVC detector
│   ├── human_vehicle/           # Single-sensor human/vehicle LinearSVC detector
│   ├── human_vehicle_pico2w/    # Four-sensor Pico 2W LinearSVC detector
│   ├── rf_pico2w/               # Four-sensor Random Forest detector
│   ├── threshold_pico2w/        # Four-sensor threshold-rule detector
│   ├── test_ld2412_config/      # Raw protocol/configuration test sketch
│   └── shared/                  # Shared feature/baseline/model source snapshots
├── scripts/                     # Python training, export, capture, simulation, and analysis tools
├── notebooks/                   # ML experiment notebooks
├── logs/                        # Captured training/evaluation logs
├── models/                      # Saved scikit-learn models
├── BASELINE_SYSTEM.md           # Older baseline-system design notes
└── feature_definitions.md       # Detailed feature documentation
```

## Hardware

### Single-sensor sketches

The single-sensor sketches target an Arduino-style board with `Serial1`; comments in `record_data` document the original MKR WiFi 1010 wiring:

| Board | LD2412 |
| --- | --- |
| RX / `Serial1` RX | TX |
| TX / `Serial1` TX | RX |
| GND | GND |
| 5V | VCC |

### Pico 2W multi-sensor sketches

Most Pico 2W sketches use four LD2412 sensors:

| Sensor | Serial port | Pins in current sketches |
| --- | --- | --- |
| 0 | `Serial1` | GP0 / GP1 |
| 1 | `Serial2` or `SerialPIO` | GP4 / GP5 or GP14 / GP15, depending on sketch |
| 2 | `SerialPIO` | GP10 / GP11 or GP21 / GP22, depending on sketch |
| 3 | `SerialPIO` | GP14 / GP15 or GP16 / GP17, depending on sketch |

Check the target sketch's `config.h` before wiring; the pin map differs between `human_vehicle_pico2w`, `record_data_pico2w`, `threshold_pico2w`, and `rf_pico2w`.

## Arduino setup

1. Install the Arduino IDE or `arduino-cli`.
2. Install the board package for your target board:
   - Arduino SAMD boards for MKR-style targets.
   - Raspberry Pi Pico/RP2040 Arduino core for Pico 2W targets.
3. Make `libraries/LD2412Toolkit` visible to Arduino:
   - set this repository as your Arduino sketchbook, or
   - copy/symlink `libraries/LD2412Toolkit` into your Arduino `libraries` directory.
4. For Pico 2W sketches, ensure the Pico core provides/installs `SerialPIO` and TinyUSB support.
5. Open the sketch directory you want to build, for example `sketches/record_data/record_data.ino`.
6. Select the board and port, compile, and upload.

## LD2412 sensor configuration

`LD2412Sensor::configure()` sends raw UART configuration commands to:

- enter configuration mode,
- set moving and stationary max gates to gate 13 (`NUM_GATES = 14`),
- set motion sensitivity to 15 for each gate,
- set stationary sensitivity to 20 for each gate,
- enable engineering mode so gate energies are present in the stream,
- exit configuration mode.

The frame parser then watches for LD2412 data frames, validates payload length, and extracts 14 moving plus 14 stationary gate-energy values.

## Baseline and normalization model

The current `BaselineManager` implementation uses a **distance-to-set baseline**:

1. During calibration, it warms up the sensor and samples raw moving/stationary gate values.
2. For each gate, it sorts the collected values and stores the unique background values.
3. At runtime, each raw value is converted to:

```text
distance = abs(current_value - closest_value_seen_during_baseline)
```

This makes `0` mean “indistinguishable from the calibrated empty environment” and larger values mean “different from background.” The feature extractor and embedded models are built around these distance-to-set values.

> Note: `BASELINE_SYSTEM.md` contains earlier design notes for a MAD/z-score/EEPROM baseline workflow. The code in `libraries/LD2412Toolkit` and the active sketches currently use the distance-to-set RAM baseline path.

Default active-sketch calibration values are intentionally short for iteration:

- warmup: `BASELINE_WARMUP_MS = 5000` ms,
- samples: `BASELINE_SAMPLES = 10`,
- sample interval: `BASELINE_INTERVAL = 200` ms.

For production use, increase these values in the sketch `config.h` and calibrate with the area empty.

## Feature vector

Most ML sketches build a 30-feature vector at 5 Hz:

| Group | Count | Examples |
| --- | ---: | --- |
| Spatial | 10 | max/total moving energy, active moving zones, centroid, spread, same for stationary gates |
| Temporal | 6 | move/stat change rates, zone switches, motion duration, moving/stationary variance |
| Pattern | 2 | sequential active zones, adjacent-zone correlation |
| Physics-inspired | 12 | weighted peak stability, spatial smoothness, centroid velocity, movement linearity, kurtosis, skewness, Hjorth mobility/complexity, crest factor, spectral entropy, flicker index, flux |

See `feature_definitions.md` and the `features.ino` file inside each sketch for the exact order used by training and embedded inference.

## Main sketches

| Sketch | Purpose |
| --- | --- |
| `sketches/record_data` | Single-sensor logger that prints gate distances and `FEAT` rows for training. Supports serial commands such as `HELP`, `CALIBRATE`, `STATUS`, `VALIDATE`, `CLEAR`, and `NORMALIZE`. |
| `sketches/record_data_pico2w` | Four-sensor Pico 2W logger with per-sensor telemetry. |
| `sketches/human` | Single-sensor LinearSVC detector for human/background classification. |
| `sketches/human_vehicle` | Single-sensor LinearSVC detector for background/human/vehicle/target-style classes, depending on exported model header. |
| `sketches/human_vehicle_pico2w` | Four-sensor Pico 2W LinearSVC detector; LED turns on if any active sensor exceeds the configured class threshold. |
| `sketches/rf_pico2w` | Four-sensor Random Forest target/background detector exported from Python. |
| `sketches/threshold_pico2w` | Four-sensor detector using a simple consecutive-active-gate rule. |
| `sketches/test_ld2412_config` | Low-level LD2412 protocol/configuration test and frame dump utility. |

## Serial tools and web dashboards

Run Python scripts from the repository root.

### Capture single-sensor inference

```bash
python3 scripts/capture_inference_log.py /dev/cu.usbmodem14201 115200
```

This logs serial output and serves a FastAPI/WebSocket dashboard.

### Capture Pico 2W multi-sensor inference

```bash
python3 scripts/capture_inference_pico2w.py /dev/cu.usbmodem14201 115200
```

The script toggles DTR/RTS for Pico USB CDC, writes a timestamped log, and serves a four-sensor live dashboard.

### Simple serial capture

```bash
python3 scripts/capture_inference_simple.py /dev/cu.usbmodem14201 115200
```

### Replay logs in the browser

```bash
python3 scripts/sim_server.py logs/07/human_running1_clean.txt
python3 scripts/sim_server_pico2w.py logs/pico_4s_mixed.txt
```

## Training and exporting models

Python dependencies used across scripts include:

```bash
pip install numpy pandas scikit-learn joblib pyserial fastapi uvicorn m2cgen
```

Some one-off diagnostic scripts contain absolute paths from the original development machine. The main train/export/capture scripts are easiest to run from the repository root.

### Train the LinearSVC model

```bash
python3 scripts/train_running_v7.py
```

This loads configured files from `logs/current` and `logs/08`, trains a balanced `LinearSVC` pipeline with a `StandardScaler`, prints metrics, and saves:

```text
models/linear_svc_model.pkl
```

### Export LinearSVC to an Arduino header

```bash
python3 scripts/export_linear_svc_to_header.py \
  --model-path models/linear_svc_model.pkl \
  --out-path sketches/human_vehicle/svm_model.h
```

For a Pico 2W SVM build, change `--out-path` to `sketches/human_vehicle_pico2w/svm_model.h`.

### Train/export Random Forest target detector

```bash
python3 scripts/export_rf_to_c.py
```

This trains the RF path from `scripts/train_threshold_models.py` data loading logic and writes:

```text
sketches/rf_pico2w/RandomForest.h
```

## Data collection workflow

1. Flash `record_data` or `record_data_pico2w`.
2. Open Serial Monitor at 115200 baud.
3. Ensure the detection area is empty during baseline calibration.
4. Record background, human, running-human, vehicle, rain/noise, or other target sessions.
5. Save logs under `logs/<session>/`.
6. Clean or filter logs with utilities such as `scripts/clean_data.py`.
7. Update `FILES_MAP` in the training script if you add or rename files.
8. Retrain and export the model header.
9. Flash the detector sketch and validate on live serial telemetry.

## Troubleshooting

- **No frames received:** verify TX/RX are crossed, power is stable, baud is 115200, and the sketch's selected UART pins match wiring.
- **Pico serial output missing:** use a capture script that sets DTR/RTS, or reconnect the board after opening the monitor.
- **High false positives:** collect a longer/cleaner empty baseline, add representative background/noise logs, and retrain.
- **Poor transfer between locations:** recalibrate baseline after moving or rotating the sensor.
- **Model/header mismatch:** ensure the sketch's `features.ino` emits features in the same 30-value order used by the Python training script.

## Project notes

- `logs/` and `models/` are part of the experiment history and are useful for retraining and analysis. The logs (plain-text serial captures plus plots in `logs/06/plots`) make up almost all of the repository's size (about 65 MB checked out; the largest, `logs/pico_4s_mixed.txt`, is 6.9 MB).
- `.venv/`, `.DS_Store`, Python caches, notebook checkpoints, and local editor files are intentionally ignored by Git.
- The project is optimized for fast embedded iteration; review calibration durations, thresholds, and pin maps before deployment.
