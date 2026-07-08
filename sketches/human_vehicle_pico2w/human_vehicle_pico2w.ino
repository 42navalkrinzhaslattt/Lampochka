/*
  SVM Detector with Vehicle Class (LD2412) — Multi-Sensor Pico 2W

  Uses LinearSVC model trained with 30 derived features.
  3-class: background, human, vehicle.
  Both "human" and "vehicle" classes light up the LED.

  4 LD2412 sensors on Raspberry Pi Pico 2W:
    Sensor 0: Serial1   (GP0/GP1)
    Sensor 1: Serial2   (GP4/GP5)
    Sensor 2: SerialPIO (GP10/GP11)
    Sensor 3: SerialPIO (GP14/GP15)
*/

#include "config.h"
#include <LD2412Toolkit.h>

// PIO serial port definitions
SerialPIO sensorSerial2(10, 11);
SerialPIO sensorSerial3(14, 15);

// Sensor objects (one per physical sensor)
LD2412Sensor* sensors[NUM_SENSORS];

// Baseline managers (one per sensor)
#if USE_BASELINE_LIBRARY
BaselineManager* baselines[NUM_SENSORS];
#endif

// Include SVM in a namespace to avoid collisions
namespace SVC {
#include "svm_model.h"
}

// Softmax
void softmax(const float *scores, int n, float *probs) {
  float max_val = scores[0];
  for (int i = 1; i < n; i++)
    if (scores[i] > max_val)
      max_val = scores[i];
  float sum = 0.0f;
  for (int i = 0; i < n; i++) {
    float e = exp(scores[i] - max_val);
    probs[i] = e;
    sum += e;
  }
  if (sum <= 0)
    sum = 1.0f;
  for (int i = 0; i < n; i++)
    probs[i] /= sum;
}

// Forward decls
void syncLatestFromSensor(int idx);
void updateHistory(int idx, BaselineManager &bl);
void buildFeatureVector(int idx, BaselineManager &bl, float features[], int maxFeatures);

// Detection thresholds
float HUMAN_THRESHOLD = 0.50f;
float VEHICLE_THRESHOLD = 0.50f;
float TARGET_THRESHOLD = 0.50f;
int HUMAN_CLASS_INDEX = -1;
int VEHICLE_CLASS_INDEX = -1;
int TARGET_CLASS_INDEX = -1;

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

  // Cache class indices for 'human', 'vehicle', and 'target'
  for (int i = 0; i < SVC::NUM_CLASSES; i++) {
    if (String(SVC::class_names[i]).equalsIgnoreCase("human")) {
      HUMAN_CLASS_INDEX = i;
    }
    if (String(SVC::class_names[i]).equalsIgnoreCase("vehicle")) {
      VEHICLE_CLASS_INDEX = i;
    }
    if (String(SVC::class_names[i]).equalsIgnoreCase("target")) {
      TARGET_CLASS_INDEX = i;
    }
  }

  // Count active sensors
  int activeSensors = 0;
  for (int i = 0; i < NUM_SENSORS; i++)
    if (sensorOk[i]) activeSensors++;

  Serial.println("\n============================================");
  Serial.print("Active sensors: ");
  Serial.print(activeSensors);
  Serial.print("/");
  Serial.println(NUM_SENSORS);
  Serial.print("Human threshold:  "); Serial.println(HUMAN_THRESHOLD, 2);
  Serial.print("Vehicle threshold: "); Serial.println(VEHICLE_THRESHOLD, 2);
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
      float features[SVC::NUM_FEATURES];
      buildFeatureVector(s, *baselines[s], features, SVC::NUM_FEATURES);
      unsigned long t1 = micros();

      // Feature logging (CSV-style)
      Serial.print("S");
      Serial.print(s);
      Serial.print(":features=");
      for (int i = 0; i < SVC::NUM_FEATURES; i++) {
        if (i > 0)
          Serial.print(',');
        Serial.print(features[i], 4);
      }
      Serial.println();

      // Measure inference time
      float svc_scores[SVC::NUM_CLASSES];
      SVC::computeSVMDecisionScores(features, svc_scores);
      float p_svc[SVC::NUM_CLASSES];
      softmax(svc_scores, SVC::NUM_CLASSES, p_svc);
      unsigned long t2 = micros();

      int best = 0;
      float bestVal = p_svc[0];
      for (int i = 1; i < SVC::NUM_CLASSES; i++)
        if (p_svc[i] > bestVal) {
          bestVal = p_svc[i];
          best = i;
        }

      // Check detection for this sensor
      bool humanDetected =
          (HUMAN_CLASS_INDEX >= 0 && p_svc[HUMAN_CLASS_INDEX] >= HUMAN_THRESHOLD);
      bool vehicleDetected = (VEHICLE_CLASS_INDEX >= 0 &&
                              p_svc[VEHICLE_CLASS_INDEX] >= VEHICLE_THRESHOLD);
      bool targetDetected = (TARGET_CLASS_INDEX >= 0 &&
                             p_svc[TARGET_CLASS_INDEX] >= TARGET_THRESHOLD);
      bool sensorDetected = humanDetected || vehicleDetected || targetDetected;

      if (sensorDetected)
        anyLedOn = true;

      // Telemetry (tagged by sensor index)
      Serial.print("S");
      Serial.print(s);
      Serial.print(":pred=");
      Serial.print(SVC::class_names[best]);
      Serial.print(",conf=");
      Serial.print(bestVal, 3);

      Serial.print(",t_feat_us=");
      Serial.print(t1 - t0);
      Serial.print(",t_infer_us=");
      Serial.print(t2 - t1);

      if (HUMAN_CLASS_INDEX >= 0) {
        Serial.print(",human_p=");
        Serial.print(p_svc[HUMAN_CLASS_INDEX], 3);
      }
      if (VEHICLE_CLASS_INDEX >= 0) {
        Serial.print(",vehicle_p=");
        Serial.print(p_svc[VEHICLE_CLASS_INDEX], 3);
      }
      if (TARGET_CLASS_INDEX >= 0) {
        Serial.print(",target_p=");
        Serial.print(p_svc[TARGET_CLASS_INDEX], 3);
      }
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
