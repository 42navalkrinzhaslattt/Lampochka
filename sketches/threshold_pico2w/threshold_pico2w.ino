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

// Forward decls
void syncLatestFromSensor(int idx);

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



  // Count active sensors
  int activeSensors = 0;
  for (int i = 0; i < NUM_SENSORS; i++)
    if (sensorOk[i]) activeSensors++;

  Serial.println("\n============================================");
  Serial.print("Active sensors: ");
  Serial.print(activeSensors);
  Serial.print("/");
  Serial.println(NUM_SENSORS);

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

      // --- Threshold Logic ---
      int activeCount = 0;
      int minActive = 99;
      int maxActive = -1;

      for (int i = 0; i < NUM_GATES; i++) {
        float energy = baselines[s]->distanceToSetMoving(latestMovingGates[s][i], i);
        if (energy >= 20.0f) {
           activeCount++;
           if (i < minActive) minActive = i;
           if (i > maxActive) maxActive = i;
        }
      }

      String pred = "background";
      bool targetDetected = false;

      if (activeCount > 3) {
          pred = "burst";
      } else if (activeCount == 2 || activeCount == 3) {
          // Check if strictly consecutive
          if ((maxActive - minActive) == (activeCount - 1)) {
              pred = "target";
              targetDetected = true;
          }
      }

      if (targetDetected) {
          anyLedOn = true;
      }
      
      // Telemetry (tagged by sensor index)
      Serial.print("S");
      Serial.print(s);
      Serial.print(":pred=");
      Serial.print(pred);
      Serial.print(",conf=1.000"); // Fixed confidence for compatibility

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
