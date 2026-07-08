/*
  LD2412 CSV Data Logger — Multi-Sensor Pico 2W

  Hardware:
  - Raspberry Pi Pico 2W
  - 4x HLK-LD2412 mmWave radar sensors (24GHz)

  Pin mapping:
    Sensor 0: Serial1   (GP0/GP1)  — UART0
    Sensor 1: Serial2   (GP4/GP5)  — UART1
    Sensor 2: SerialPIO (GP10/GP11)
    Sensor 3: SerialPIO (GP14/GP15)

  Features:
  - CSV data collection for ML training (46 features per sensor)
  - Baseline calibration system with outlier rejection
  - Environment-normalized features
  - Output tagged by sensor index (S0, S1, S2, S3)

  Commands (via Serial Monitor):
  - CALIBRATE  : Collect new baseline for all sensors
  - VALIDATE   : Test baseline quality
  - STATUS     : Show current baseline
  - CLEAR      : Clear all baselines
  - HELP       : Show command list
*/

#include "config.h"
#include <LD2412Toolkit.h>

// PIO serial port definitions
SerialPIO sensorSerial2(10, 11);
SerialPIO sensorSerial3(14, 15);

// Sensor objects (one per physical sensor)
LD2412Sensor* sensors[NUM_SENSORS];

// Baseline managers (one per sensor)
BaselineManager* baselines[NUM_SENSORS];

// Forward declarations
void handleCommand(String cmd);
void printHelp();
void runBaselineCalibration();
void printStatus();
void testNormalization();
void syncLatestFromSensor(int idx);
void printFixedWidth(float value, int width, int decimals);

void setup() {
  Serial.begin(115200);

  unsigned long start = millis();
  while (!Serial && millis() - start < 5000) {
    delay(10);
  }
  delay(1000); // Extra settle time for macOS USB

  Serial.println("Pico 2W is alive!");
  pinMode(LED_PIN, OUTPUT);

  Serial.println("============================================");
  Serial.println("LD2412 CSV Data Logger — 4 Sensors");
  Serial.println("Pico 2 W");
  Serial.println("============================================");

  // Initialize all serial ports
  Serial1.setTX(0);
  Serial1.setRX(1);
  Serial1.begin(115200);

  Serial2.setTX(4);
  Serial2.setRX(5);
  Serial2.begin(115200);

  sensorSerial2.begin(115200);
  sensorSerial3.begin(115200);

  delay(1000);

  // Create sensor objects
  sensors[0] = new LD2412Sensor(Serial1, NUM_GATES);
  sensors[1] = new LD2412Sensor(Serial2, NUM_GATES);
  sensors[2] = new LD2412Sensor(sensorSerial2, NUM_GATES);
  sensors[3] = new LD2412Sensor(sensorSerial3, NUM_GATES);

  // Configure each sensor
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("\n============ Configuring Sensor ");
    Serial.print(i);
    Serial.println(" ============");
    sensors[i]->configure();
    delay(500);
  }

  // Test communication for each sensor
  Serial.println("\nTesting sensor communication...");
  for (int i = 0; i < NUM_SENSORS; i++) {
    unsigned long testStart = millis();
    int frameCount = 0;
    while (millis() - testStart < 5000) {
      if (sensors[i]->readFrame()) {
        syncLatestFromSensor(i);
        frameCount++;
      }
      delay(50);
    }
    Serial.print("  Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(frameCount);
    Serial.println(frameCount < 3 ? " frames ⚠ CHECK WIRING" : " frames ✓");
  }

  // Print CSV header
  Serial.print("sensor,label");
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

  // Auto-calibrate baselines
  Serial.println();
  Serial.println("========================================");
  Serial.println("Starting automatic baseline calibration for all sensors…");
  Serial.println("========================================");
  runBaselineCalibration();

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

  // Continuously read frames from all active sensors
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (!sensorOk[i]) continue;
    if (sensors[i]->readFrame()) {
      syncLatestFromSensor(i);
    }
  }

  // Output CSV at sample rate
  if (millis() - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = millis();

    // CHANGE THIS LABEL: "human", "nothing", "vehicle", etc.
    String label = "nothing";

    for (int s = 0; s < NUM_SENSORS; s++) {
      if (!sensorOk[s]) continue;
      if (!baselines[s] || !baselines[s]->valid())
        continue;

      // Update history for this sensor
      updateHistory(s, *baselines[s]);

      // LINE 1: Gate distances (moving + stationary)
      Serial.print("S");
      Serial.print(s);
      Serial.print(",");
      Serial.print(label);
      Serial.print(",GATE,mv:");
      for (int i = 0; i < NUM_GATES; i++) {
        if (i > 0)
          Serial.print(",");
        Serial.print(baselines[s]->distanceToSetMoving(latestMovingGates[s][i], i));
      }
      Serial.print(",st:");
      for (int i = 0; i < NUM_GATES; i++) {
        if (i > 0)
          Serial.print(",");
        Serial.print(baselines[s]->distanceToSet(latestStationaryGates[s][i], i));
      }
      Serial.println();

      // LINE 2: Spatial + Temporal + Pattern + Physics features
      Serial.print("S");
      Serial.print(s);
      Serial.print(",");
      Serial.print(label);
      Serial.print(",FEAT");
      outputSpatialFeatures(s, *baselines[s]);
      outputTemporalFeatures(s, *baselines[s]);
      outputPatternFeatures(s, *baselines[s]);
      outputNewFeatures(s, *baselines[s]);

      Serial.println();
    }

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
    for (int i = 0; i < NUM_SENSORS; i++) {
      if (baselines[i] && baselines[i]->valid()) {
        Serial.print("\n--- Sensor ");
        Serial.print(i);
        Serial.println(" ---");
        baselines[i]->printStats(Serial);
      } else {
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.println(": No baseline loaded.");
      }
    }
  } else if (cmd == "STATUS" || cmd == "STAT") {
    printStatus();
  } else if (cmd == "CLEAR") {
    for (int i = 0; i < NUM_SENSORS; i++) {
      if (baselines[i]) {
        baselines[i]->freeBuffers();
        delete baselines[i];
        baselines[i] = nullptr;
      }
    }
    Serial.println("All baselines cleared.");
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
  Serial.println("  CALIBRATE  - Re-run baseline calibration (all sensors)");
  Serial.println("  STATUS     - Show current baseline stats");
  Serial.println("  VALIDATE   - Test baseline quality");
  Serial.println("  CLEAR      - Clear all baselines");
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
  Serial.print("Estimated per-sensor duration: ");
  Serial.print(estWarmupMin);
  Serial.print(" min warmup + ");
  Serial.print(estCollectMin);
  Serial.println(" min collection");
  Serial.println("(All sensors calibrate in parallel...)");

  for (int i = 0; i < NUM_SENSORS; i++) {
    // Free old baseline if exists
    if (baselines[i]) {
      baselines[i]->freeBuffers();
      delete baselines[i];
    }

    Serial.print("\nCalibrating baseline for Sensor ");
    Serial.print(i);
    Serial.println("…");

    baselines[i] = new BaselineManager(NUM_GATES, BASELINE_SAMPLES,
                                       BASELINE_INTERVAL, BASELINE_WARMUP_MS);

    if (!baselines[i]->collectBlocking(*sensors[i])) {
      Serial.print("  Sensor ");
      Serial.print(i);
      Serial.println(": Baseline collection failed — DISABLED.");
      sensorOk[i] = false;
      delete baselines[i];
      baselines[i] = nullptr;
      continue;
    }

    if (!baselines[i]->process()) {
      Serial.print("  Sensor ");
      Serial.print(i);
      Serial.println(": Baseline processing failed — using raw fallback.");
      baselines[i]->fallbackFromRaw();
    } else {
      Serial.print("  Sensor ");
      Serial.print(i);
      Serial.println(": Baseline ready ✓");
    }
    baselines[i]->printStats(Serial);
    baselines[i]->printBaselineSets(Serial);
    baselines[i]->freeBuffers();
  }

  // Count active sensors
  int activeSensors = 0;
  for (int i = 0; i < NUM_SENSORS; i++)
    if (sensorOk[i]) activeSensors++;

  Serial.print("\nActive sensors: ");
  Serial.print(activeSensors);
  Serial.print("/");
  Serial.println(NUM_SENSORS);
  Serial.println("All baseline calibration complete.");
}

void printStatus() {
  Serial.println();
  Serial.println("========================================");
  Serial.println("SYSTEM STATUS");
  Serial.println("========================================");
  Serial.println();

  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(": ");
    if (baselines[i] && baselines[i]->valid()) {
      Serial.println("LOADED ✓");
      unsigned long age_ms = millis() - baselines[i]->timestamp();
      Serial.print("  Age: ");
      Serial.print(age_ms / 60000);
      Serial.println(" minutes");
      baselines[i]->printStats(Serial);
    } else {
      Serial.println("NOT LOADED");
    }
    Serial.println();
  }

  Serial.println("Sensor Configuration:");
  Serial.print("  Sensors: ");
  Serial.println(NUM_SENSORS);
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
  for (int s = 0; s < NUM_SENSORS; s++) {
    if (!baselines[s] || !baselines[s]->valid()) {
      Serial.print("Sensor ");
      Serial.print(s);
      Serial.println(": No baseline loaded!");
      continue;
    }

    Serial.println();
    Serial.print("Sensor ");
    Serial.print(s);
    Serial.println(" — Current Reading (Distance-to-Set):");
    Serial.println("  Gate | Raw Move | Distance | Raw Stat | Distance");
    Serial.println("  -----|----------|----------|----------|----------");

    for (int i = 0; i < NUM_GATES; i++) {
      int distMove = baselines[s]->distanceToSetMoving(latestMovingGates[s][i], i);
      int distStat = baselines[s]->distanceToSet(latestStationaryGates[s][i], i);

      Serial.print("    ");
      if (i < 10)
        Serial.print(" ");
      Serial.print(i);
      Serial.print(" |    ");
      Serial.print(latestMovingGates[s][i]);
      Serial.print("    |    ");
      Serial.print(distMove);
      Serial.print("     |    ");
      Serial.print(latestStationaryGates[s][i]);
      Serial.print("    |    ");
      Serial.print(distStat);
      Serial.println();
    }
  }
  Serial.println();
}

void syncLatestFromSensor(int idx) {
  const uint8_t *mv = sensors[idx]->moving();
  const uint8_t *st = sensors[idx]->stationary();
  for (int i = 0; i < NUM_GATES; i++) {
    latestMovingGates[idx][i] = mv ? mv[i] : 0;
    latestStationaryGates[idx][i] = st ? st[i] : 0;
  }
}

void printFixedWidth(float value, int width, int decimals) {
  String str = String(value, decimals);
  while (str.length() < width)
    str = " " + str;
  Serial.print(str);
}
