/*
  LD2412 CSV Data Logger with Baseline Collection

  Hardware:
  - Arduino MKR WiFi 1010
  - HLK-LD2412 mmWave radar sensor (24GHz)

  Connection:
  Arduino RX (Pin 13) -- TX LD2412
  Arduino TX (Pin 14) -- RX LD2412
  GND                 -- GND LD2412
  5V                  -- VCC LD2412

  Features:
  - CSV data collection for ML training (46 features)
  - Baseline calibration system with outlier rejection
  - Environment-normalized features

  Commands (via Serial Monitor):
  - CALIBRATE  : Collect new baseline (5 min)
  - LOAD       : Load baseline from EEPROM (Not implemented in library yet,
  placeholder)
  - VALIDATE   : Test baseline quality
  - STATUS     : Show current baseline
  - CLEAR      : Clear baseline
  - HELP       : Show command list
*/

#include "config.h"
#include <LD2412Toolkit.h>

// Instantiate library classes
LD2412Sensor sensor(sensorSerial, NUM_GATES);
BaselineManager baseline(NUM_GATES, BASELINE_SAMPLES, BASELINE_INTERVAL,
                         BASELINE_WARMUP_MS);

// Forward declarations
void handleCommand(String cmd);
void printHelp();
void runBaselineCalibration();
void printStatus();
void testNormalization();
void syncLatestFromSensor();
void printFixedWidth(float value, int width, int decimals);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000)
    ;

  pinMode(LED_PIN, OUTPUT);
  sensorSerial.begin(115200);
  delay(1000);

  // Configure sensor using library
  sensor.configure();

  // Test communication
  unsigned long testStart = millis();
  int frameCount = 0;

  while (millis() - testStart < 5000) {
    if (sensor.readFrame()) {
      syncLatestFromSensor();
      frameCount++;
    }
    delay(50);
  }

  if (frameCount < 10) {
    Serial.println("ERROR: No sensor data. Check wiring.");
    while (true) {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
      delay(500);
    }
  }

  // Print CSV header
  Serial.print("label");
  for (int i = 0; i < NUM_GATES; i++) {
    Serial.print(",move");
    Serial.print(i);
  }
  for (int i = 0; i < NUM_GATES; i++) {
    Serial.print(",stat");
    Serial.print(i);
  }
  Serial.print(",max_move_energy,total_move_energy,active_move_zones,move_"
               "centroid,move_spread");
  Serial.print(",max_stat_energy,total_stat_energy,active_stat_zones,stat_"
               "centroid,stat_spread");
  Serial.print(
      ",move_change_rate,stat_change_rate,zone_switches,motion_duration");
  Serial.print(",move_variance,stat_variance");
  Serial.print(",zones_sequential,zone_correlation");
  Serial.print(",weighted_peak_stability,spatial_smoothness_index,centroid_"
               "velocity_smoothed,movement_linearity");
  Serial.print(
      ",spatial_kurtosis,spatial_skewness,hjorth_mobility,hjorth_complexity");
  Serial.print(",crest_factor,spectral_entropy,flicker_index,flux");
  Serial.println();

  lastSampleTime = millis();

  // Check baseline status
  Serial.println();
  Serial.println("========================================");
  if (baseline.valid()) {
    Serial.println("Baseline: LOADED ✓");
    Serial.println("  Output: Z-score normalized values");
  } else {
    Serial.println("Baseline: NOT calibrated (auto-calibration will run)");
    Serial.println("  Output is RAW until calibration completes");
  }
  Serial.println("========================================");
  Serial.println();

  // Auto-calibrate baseline if none present
  if (!baseline.valid()) {
    Serial.println("Starting automatic baseline calibration…");
    runBaselineCalibration();
    Serial.println("Automatic baseline calibration complete.");
  }

  Serial.println("Ready! Type 'HELP' for commands.");
  Serial.println();
}

void loop() {
  // Check for serial commands
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    cmd.toUpperCase();
    handleCommand(cmd);
  }

  // Continuously read frames
  // Note: Library collection is blocking, so we don't need
  // 'baseline_collecting' flag here
  if (sensor.readFrame()) {
    syncLatestFromSensor();
  }

  // Output CSV at 1 Hz
  if (millis() - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = millis();

    // Update history
    updateHistory(baseline);

    // CHANGE THIS LABEL: "human", "nothing", "vehicle", etc.
    String label = "nothing";

    // LINE 1: Gate distances (moving + stationary)
    Serial.print(label);
    Serial.print(",GATE,mv:");
    for (int i = 0; i < NUM_GATES; i++) {
      if (i > 0)
        Serial.print(",");
      Serial.print(baseline.distanceToSetMoving(latestMovingGates[i], i));
    }
    Serial.print(",st:");
    for (int i = 0; i < NUM_GATES; i++) {
      if (i > 0)
        Serial.print(",");
      Serial.print(baseline.distanceToSet(latestStationaryGates[i], i));
    }
    Serial.println();

    // LINE 2: Spatial + Temporal + Pattern features
    Serial.print(label);
    Serial.print(",FEAT");
    outputSpatialFeatures(baseline);
    outputTemporalFeatures(baseline);
    outputPatternFeatures(baseline);
    outputNewFeatures(baseline);

    Serial.println();
    samplesCollected++;

    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}

void handleCommand(String cmd) {
  if (cmd == "HELP" || cmd == "?") {
    printHelp();
  } else if (cmd == "CALIBRATE" || cmd == "CAL") {
    runBaselineCalibration();
  } else if (cmd == "LOAD") {
    Serial.println("EEPROM load not implemented in library yet.");
  } else if (cmd == "VALIDATE" || cmd == "VAL") {
    if (baseline.valid()) {
      baseline.printStats(Serial);
    } else {
      Serial.println("No baseline loaded. Use 'CALIBRATE' first.");
    }
  } else if (cmd == "STATUS" || cmd == "STAT") {
    printStatus();
  } else if (cmd == "CLEAR") {
    baseline.freeBuffers();
    // Re-init to clear validity
    baseline = BaselineManager(NUM_GATES, BASELINE_SAMPLES, BASELINE_INTERVAL,
                               BASELINE_WARMUP_MS);
    Serial.println("Baseline cleared.");
  } else if (cmd == "NORMALIZE" || cmd == "NORM") {
    testNormalization();
  } else {
    Serial.print("Unknown command: ");
    Serial.println(cmd);
    Serial.println("Type 'HELP' for command list.");
  }
}

void printHelp() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("AVAILABLE COMMANDS");
  Serial.println("========================================");
  Serial.println();
  Serial.println("Baseline Commands:");
  Serial.println("  CALIBRATE  - Re-run baseline calibration");
  Serial.println("  STATUS     - Show current baseline stats");
  Serial.println("  VALIDATE   - Test baseline quality");
  Serial.println("  CLEAR      - Clear baseline");
  Serial.println();
  Serial.println("Utility Commands:");
  Serial.println(
      "  NORMALIZE  - Debug: print normalized values for current frame");
  Serial.println("  HELP       - Show this help");
  Serial.println();
  Serial.println("========================================");
  Serial.println();
}

void runBaselineCalibration() {
  Serial.println();
  Serial.println("Starting baseline calibration (Library Mode)…");
  unsigned long estWarmupMin = BASELINE_WARMUP_MS / 60000;
  unsigned long estCollectMs = BASELINE_SAMPLES * BASELINE_INTERVAL;
  unsigned long estCollectMin = estCollectMs / 60000;
  Serial.print("Estimated duration: ");
  Serial.print(estWarmupMin);
  Serial.print(" min warmup + ");
  Serial.print(estCollectMin);
  Serial.println(" min collection");
  Serial.println("(Collection is blocking, please wait...)");

  if (!baseline.collectBlocking(sensor)) {
    Serial.println("ERROR: Allocation failed during collection.");
    return;
  }

  if (!baseline.process()) {
    Serial.println("Robust processing failed, applying raw fallback!!!");
    baseline.fallbackFromRaw();
  }

  Serial.println();
  Serial.println("Baseline ready.");
  Serial.println();
  baseline.printStats(Serial);
  baseline.printBaselineSets(Serial);
  baseline.freeBuffers();
}

void printStatus() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("SYSTEM STATUS");
  Serial.println("========================================");
  Serial.println();

  Serial.print("Baseline: ");
  if (baseline.valid()) {
    Serial.println("LOADED ✓");
    unsigned long age_ms = millis() - baseline.timestamp();
    Serial.print("  Age: ");
    Serial.print(age_ms / 60000);
    Serial.println(" minutes");
    Serial.println();
    baseline.printStats(Serial);
  } else {
    Serial.println("NOT LOADED");
    Serial.println("  Use 'CALIBRATE' to collect baseline");
  }

  Serial.println("Sensor Configuration:");
  Serial.print("  Gates: ");
  Serial.println(NUM_GATES);
  Serial.print("  Sample Rate: ");
  Serial.print(1000 / SAMPLE_INTERVAL);
  Serial.println(" Hz");
  Serial.print("  Samples Collected: ");
  Serial.println(samplesCollected);
  Serial.println();
  Serial.println("========================================");
  Serial.println();
}

void testNormalization() {
  if (!baseline.valid()) {
    Serial.println("No baseline loaded!");
    return;
  }

  Serial.println();
  Serial.println("Current Reading (Distance-to-Set):");
  Serial.println("  Gate | Raw Move | Distance | Raw Stat | Distance");
  Serial.println("  -----|----------|----------|----------|----------");

  for (int i = 0; i < NUM_GATES; i++) {
    int distMove = baseline.distanceToSetMoving(latestMovingGates[i], i);
    int distStat = baseline.distanceToSet(latestStationaryGates[i], i);

    Serial.print("    ");
    if (i < 10)
      Serial.print(" ");
    Serial.print(i);
    Serial.print(" |    ");
    Serial.print(latestMovingGates[i]);
    Serial.print("    |    ");
    Serial.print(distMove);
    Serial.print("     |    ");
    Serial.print(latestStationaryGates[i]);
    Serial.print("    |    ");
    Serial.print(distStat);
    Serial.println();
  }
  Serial.println();
}

void syncLatestFromSensor() {
  const uint8_t *mv = sensor.moving();
  const uint8_t *st = sensor.stationary();
  for (int i = 0; i < NUM_GATES; i++) {
    latestMovingGates[i] = mv ? mv[i] : 0;
    latestStationaryGates[i] = st ? st[i] : 0;
  }
}

void printFixedWidth(float value, int width, int decimals) {
  String str = String(value, decimals);
  while (str.length() < width)
    str = " " + str;
  Serial.print(str);
}
