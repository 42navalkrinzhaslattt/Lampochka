// baseline_collect.cpp
#include "baseline_collect.h"
#include "config.h"
#include "baseline.h"
#include "sensor_comm.h"
#include <Arduino.h>

bool startBaselineCollection() {
  Serial.println("\n========================================");
  Serial.println("BASELINE COLLECTION");
  Serial.println("========================================");
  Serial.println();
  Serial.println("IMPORTANT: Area must be COMPLETELY CLEAR");
  Serial.println("  - No people or animals");
  Serial.println("  - Calm conditions (no wind if outdoor)");
  Serial.print("  - Stable environment for "); Serial.print(BASELINE_SAMPLES); Serial.println(" samples at 1 Hz");
  Serial.println(); Serial.println("Auto-start: Beginning collection now...");
  Serial.print("Allocating memory... "); if (!allocateBaselineBuffers()) { Serial.println("FAILED! Not enough memory."); return false; } Serial.println("OK");

  Serial.println(); Serial.println("Phase 1: Sensor Warmup (30 seconds)"); Serial.println("  Allowing sensor to stabilize...");
  unsigned long warmupStart = millis(); int lastPercent = -1;
  while (millis() - warmupStart < BASELINE_WARMUP_MS) { readFrame(); int percent = ((millis() - warmupStart) * 100) / BASELINE_WARMUP_MS; if (percent != lastPercent && percent % 10 == 0) { Serial.print("  "); Serial.print(percent); Serial.println("%"); lastPercent = percent; } delay(50); }
  Serial.println("  Warmup complete!"); Serial.println(); Serial.println("Phase 2: Data Collection (1 minute)"); Serial.print("  Collecting "); Serial.print(BASELINE_SAMPLES); Serial.println(" samples at 1 Hz...");

  baseline_collecting = true; baseline_sample_count = 0;
  unsigned long lastSample = millis(); lastPercent = -1;
  while (baseline_sample_count < BASELINE_SAMPLES) {
    readFrame();
    if (millis() - lastSample >= BASELINE_INTERVAL) {
      lastSample = millis();
      for (int i = 0; i < NUM_GATES; i++) {
        baseline_raw_moving[baseline_sample_count * NUM_GATES + i] = latestMovingGates[i];
        baseline_raw_stationary[baseline_sample_count * NUM_GATES + i] = latestStationaryGates[i];
      }
      baseline_sample_count++;
      int percent = (baseline_sample_count * 100) / BASELINE_SAMPLES; if (percent != lastPercent && percent % 10 == 0) { Serial.print("  "); Serial.print(percent); Serial.print("% ("); Serial.print(baseline_sample_count); Serial.print("/"); Serial.print(BASELINE_SAMPLES); Serial.println(")"); lastPercent = percent; }
    }
  }
  baseline_collecting = false; Serial.println("  Collection complete!"); Serial.println(); return true;
}

void showProgressBar(int current, int total, int barWidth) {
  float progress = (float)current / total; int pos = barWidth * progress;
  Serial.print("  ["); for (int i = 0; i < barWidth; i++) { if (i < pos) Serial.print("="); else if (i == pos) Serial.print(">"); else Serial.print(" "); } Serial.print("] "); Serial.print((int)(progress * 100)); Serial.print("% ("); Serial.print(current); Serial.print("/"); Serial.print(total); Serial.println(")");
}

void cancelBaselineCollection() { if (baseline_collecting) { baseline_collecting = false; freeBaselineBuffers(); Serial.println("Collection cancelled."); } }
