/*
  SVM Detector with Vehicle Class (LD2412) — Multi-Sensor Pico 2W

  Uses LinearSVC model trained with 30 derived features.
  3-class: background, human, vehicle.
  Both "human" and "vehicle" classes light up the LED.

  4 LD2412 sensors on Raspberry Pi Pico 2W:
    Sensor 0: Serial1   (GP0/GP1)
    Sensor 1: SerialPIO (GP14/GP15)
    Sensor 2: SerialPIO (GP21/GP22)
    Sensor 3: SerialPIO (GP16/GP17)
*/

#include "config.h"
#include <LD2412Toolkit.h>

// PIO serial port definitions
SerialPIO sensorSerial1(14, 15);
SerialPIO sensorSerial2(21, 22);
SerialPIO sensorSerial3(16, 17);

// Sensor objects (one per physical sensor)
LD2412Sensor* sensors[NUM_SENSORS];

// Baseline managers (one per sensor)
#if USE_BASELINE_LIBRARY
BaselineManager* baselines[NUM_SENSORS];
#endif

// Include RF in a namespace to avoid collisions
namespace RF {
#include "RandomForest.h"
const int NUM_FEATURES = 30;
}

// Forward decls
void syncLatestFromSensor(int idx);
void updateHistory(int idx, BaselineManager &bl);
void buildFeatureVector(int idx, BaselineManager &bl, float features[], int maxFeatures);

// Detection thresholds
float TARGET_THRESHOLD = 0.50f;

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
  Serial.println("LD2412 SVM Detector — 4 Sensors");
  Serial.println("Pico 2 W");
  Serial.println("============================================");

  // Initialize all serial ports
  Serial1.setTX(0);
  Serial1.setRX(1);
  Serial1.begin(115200);

  sensorSerial1.begin(115200);
  sensorSerial2.begin(115200);
  sensorSerial3.begin(115200);

  delay(1000);

  // Create sensor objects
  sensors[0] = new LD2412Sensor(Serial1, NUM_GATES);
  sensors[1] = new LD2412Sensor(sensorSerial1, NUM_GATES);
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

  // Quick comm test for each sensor
  Serial.println("\nTesting sensor communication...");
  for (int i = 0; i < NUM_SENSORS; i++) {
    unsigned long testStart = millis();
    int frames = 0;
    while (millis() - testStart < 3000) {
      if (sensors[i]->readFrame())
        frames++;
      delay(25);
    }
    Serial.print("  Sensor ");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(frames);
    Serial.println(frames < 3 ? " frames ⚠ CHECK WIRING" : " frames ✓");
  }

  // Baseline calibration for each sensor
  Serial.println();
#if USE_BASELINE_LIBRARY
  for (int i = 0; i < NUM_SENSORS; i++) {
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
    } else if (!baselines[i]->process()) {
      Serial.print("  Sensor ");
      Serial.print(i);
      Serial.println(": Baseline processing failed — using raw fallback.");
      baselines[i]->fallbackFromRaw();
    } else {
      Serial.print("  Sensor ");
      Serial.print(i);
      Serial.println(": Baseline ready ✓");
      baselines[i]->printStats(Serial);
    }
  }
#endif

  // Class indices are not needed for binary Random Forest

  // Count active sensors
  int activeSensors = 0;
  for (int i = 0; i < NUM_SENSORS; i++)
    if (sensorOk[i]) activeSensors++;

  Serial.println("\n============================================");
  Serial.print("Active sensors: ");
  Serial.print(activeSensors);
  Serial.print("/");
  Serial.println(NUM_SENSORS);
  Serial.print("Target threshold:  "); Serial.println(TARGET_THRESHOLD, 2);
  Serial.println("Monitoring...");
  Serial.println("============================================");

  lastSampleTime = millis();
}

void loop() {
  // Read frames from all active sensors continuously (non-blocking)
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (!sensorOk[i]) continue;
    if (sensors[i]->readFrame()) {
      syncLatestFromSensor(i);
    }
  }

  // Inference at 5 Hz
  if (millis() - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = millis();

    bool anyLedOn = false;

    for (int s = 0; s < NUM_SENSORS; s++) {
      if (!sensorOk[s]) continue;
#if USE_BASELINE_LIBRARY
      if (!baselines[s] || !baselines[s]->valid())
        continue;

      updateHistory(s, *baselines[s]);

      // Measure feature extraction time
      unsigned long t0 = micros();
      float features[RF::NUM_FEATURES];
      buildFeatureVector(s, *baselines[s], features, RF::NUM_FEATURES);
      unsigned long t1 = micros();

      // Feature logging (CSV-style)
      Serial.print("S");
      Serial.print(s);
      Serial.print(":features=");
      for (int i = 0; i < RF::NUM_FEATURES; i++) {
        if (i > 0)
          Serial.print(',');
        Serial.print(features[i], 4);
      }
      Serial.println();

      // Measure inference time
      double d_features[RF::NUM_FEATURES];
      for (int i = 0; i < RF::NUM_FEATURES; i++) d_features[i] = features[i];
      
      double output[2];
      RF::score(d_features, output);
      unsigned long t2 = micros();

      float target_prob = output[1];
      bool sensorDetected = (target_prob >= TARGET_THRESHOLD);
      if (sensorDetected) anyLedOn = true;

      const char* pred_class = sensorDetected ? "target" : "background";

      // Telemetry (tagged by sensor index)
      Serial.print("S");
      Serial.print(s);
      Serial.print(":pred=");
      Serial.print(pred_class);
      Serial.print(",conf=");
      Serial.print(target_prob, 3);

      Serial.print(",t_feat_us=");
      Serial.print(t1 - t0);
      Serial.print(",t_infer_us=");
      Serial.print(t2 - t1);

      Serial.print(",target_p=");
      Serial.print(target_prob, 3);
      Serial.print(",detected=");
      Serial.print(sensorDetected ? 1 : 0);

      // Gate Telemetry
      Serial.print(",GATE,mv:");
      for (int i = 0; i < NUM_GATES; i++) {
        Serial.print(latestMovingGates[s][i]);
        if (i < NUM_GATES - 1)
          Serial.print(",");
      }
      Serial.print(",st:");
      for (int i = 0; i < NUM_GATES; i++) {
        Serial.print(latestStationaryGates[s][i]);
        if (i < NUM_GATES - 1)
          Serial.print(",");
      }
      Serial.println();
#endif
    }

    // LED on if ANY sensor detects human/vehicle/target
    digitalWrite(LED_PIN, anyLedOn ? HIGH : LOW);
  }
}

void syncLatestFromSensor(int idx) {
  const uint8_t *mv = sensors[idx]->moving();
  const uint8_t *st = sensors[idx]->stationary();
  for (int i = 0; i < NUM_GATES; i++) {
    latestMovingGates[idx][i] = mv ? mv[i] : 0;
    latestStationaryGates[idx][i] = st ? st[i] : 0;
  }
}
