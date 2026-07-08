/*
  SVM Detector (LD2412)

  Uses LinearSVC_v7 model trained on logs/06/ and logs/07/ with 30 derived
  features. Mirrors recorder flow and feature definitions; runs LinearSVC
  inference.
*/

#include "config.h"
#include <LD2412Toolkit.h>

// Instantiate sensor and (optionally) baseline manager
LD2412Sensor sensor(sensorSerial, NUM_GATES);
#if USE_BASELINE_LIBRARY
BaselineManager baseline(NUM_GATES, BASELINE_SAMPLES, BASELINE_INTERVAL,
                         BASELINE_WARMUP_MS);
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
void syncLatestFromSensor();
void updateHistory(BaselineManager &bl);
void buildFeatureVector(BaselineManager &bl, float features[], int maxFeatures);

// Detection threshold for "human" class
float HUMAN_THRESHOLD = 0.50f;
int HUMAN_CLASS_INDEX = -1;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
  }
  pinMode(LED_PIN, OUTPUT);
  sensorSerial.begin(115200);
  delay(600);

  // Configure LD2412
  sensor.configure();

  // Quick comm test
  unsigned long testStart = millis();
  int frames = 0;
  while (millis() - testStart < 3000) {
    if (sensor.readFrame())
      frames++;
    delay(25);
  }
  if (frames < 5) {
    Serial.println("ERROR: No sensor frames received. Check wiring.");
  }

  // Baseline calibration
  Serial.println();
#if USE_BASELINE_LIBRARY
  Serial.println("Calibrating baseline (library)…");
  unsigned long estWarmupMin = BASELINE_WARMUP_MS / 60000;
  unsigned long estCollectMs = BASELINE_SAMPLES * BASELINE_INTERVAL;
  unsigned long estCollectMin = estCollectMs / 60000;
  Serial.print("Estimated: ");
  Serial.print(estWarmupMin);
  Serial.print(" min warmup + ");
  Serial.print(estCollectMin);
  Serial.println(" min collection");

  if (!baseline.collectBlocking(sensor)) {
    Serial.println(
        "Baseline collection failed — proceeding with recorded baseline.");
  } else if (!baseline.process()) {
    Serial.println(
        "Baseline processing failed — proceeding with recorded baseline.");
  } else {
    Serial.println("Baseline ready ✓");
    baseline.printStats(Serial);
  }
#else
  Serial.println("Starting baseline calibration (manual)…");
  if (!startBaselineCollection()) {
    Serial.println("ERROR: Baseline collection failed.");
  } else if (!processBaseline()) {
    Serial.println("ERROR: Baseline processing failed.");
    freeBaselineBuffers();
  } else {
    saveBaselineToRAM();
    Serial.println();
    Serial.println("Baseline ready ✓");
    Serial.println();
    printBaselineStats();
    freeBaselineBuffers();
  }
#endif

  // Cache class index for 'human'
  for (int i = 0; i < SVC::NUM_CLASSES; i++) {
    if (String(SVC::class_names[i]).equalsIgnoreCase("human")) {
      HUMAN_CLASS_INDEX = i;
      break;
    }
  }
  Serial.print("Human threshold: ");
  Serial.println(HUMAN_THRESHOLD, 2);
  lastSampleTime = millis();
}

void loop() {
  // Read frames continuously
  if (sensor.readFrame()) {
    syncLatestFromSensor();
  }

  // Inference at 1 Hz
  if (millis() - lastSampleTime >= SAMPLE_INTERVAL) {
    lastSampleTime = millis();
    updateHistory(baseline);

    // Measure feature extraction time
    unsigned long t0 = micros();
    float features[SVC::NUM_FEATURES];
    buildFeatureVector(baseline, features, SVC::NUM_FEATURES);
    unsigned long t1 = micros();

    // Feature logging (CSV-style, matches dataset order)
    Serial.print("features=");
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

    bool humanDetected =
        (HUMAN_CLASS_INDEX >= 0 && p_svc[HUMAN_CLASS_INDEX] >= HUMAN_THRESHOLD);
    digitalWrite(LED_PIN, humanDetected ? HIGH : LOW);

    // Telemetry
    Serial.print("pred=");
    Serial.print(SVC::class_names[best]);
    Serial.print(",conf=");
    Serial.print(bestVal, 3);

    // Log timing
    Serial.print(",t_feat_us=");
    Serial.print(t1 - t0);
    Serial.print(",t_infer_us=");
    Serial.print(t2 - t1);

    if (HUMAN_CLASS_INDEX >= 0) {
      Serial.print(",human_p=");
      Serial.print(p_svc[HUMAN_CLASS_INDEX], 3);
      Serial.print(",detected=");
      Serial.print(humanDetected ? 1 : 0);
    }

    // Gate Telemetry
    Serial.print(",GATE,mv:");
    for (int i = 0; i < NUM_GATES; i++) {
      Serial.print(latestMovingGates[i]);
      if (i < NUM_GATES - 1)
        Serial.print(",");
    }
    Serial.print(",st:");
    for (int i = 0; i < NUM_GATES; i++) {
      Serial.print(latestStationaryGates[i]);
      if (i < NUM_GATES - 1)
        Serial.print(",");
    }
    Serial.println();
  }
}

void syncLatestFromSensor() {
  const uint8_t *mv = sensor.moving();
  const uint8_t *st = sensor.stationary();
  for (int i = 0; i < NUM_GATES; i++) {
    latestMovingGates[i] = mv ? mv[i] : 0;
    latestStationaryGates[i] = st ? st[i] : 0;
  }
}