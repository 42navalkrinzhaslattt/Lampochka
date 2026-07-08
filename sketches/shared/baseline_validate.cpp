// baseline_validate.cpp
#include "baseline_validate.h"
#include "config.h"
#include "baseline.h"
#include "sensor_comm.h"
#include <Arduino.h>

bool validateBaseline() {
  Serial.println("Phase 4: Baseline Validation"); Serial.println();
  if (!baseline_valid) { Serial.println("ERROR: No baseline loaded"); return false; }
  bool allPassed = true; int varianceFailures = 0;
  Serial.println("Check 1: Variance Analysis");
  for (int i = 0; i < NUM_GATES; i++) {
    float ratio = (baseline_mean[i] > 0) ? (baseline_std[i] / baseline_mean[i]) : 0;
    if (ratio > MAX_STD_RATIO) { Serial.print("  WARNING: Gate "); Serial.print(i); Serial.print(" has high variance (std/mean = "); Serial.print(ratio,2); Serial.println(")"); varianceFailures++; }
  }
  if (varianceFailures == 0) Serial.println("  PASS: All gates have acceptable variance"); else { Serial.print("  FAIL: "); Serial.print(varianceFailures); Serial.println(" gates with high variance"); allPassed = false; }

  Serial.println(); Serial.println("Check 2: Spatial Continuity"); int spatialFailures = 0;
  for (int i = 0; i < NUM_GATES - 1; i++) {
    float diff = abs(baseline_mean[i] - baseline_mean[i+1]); if (diff > MAX_SPATIAL_DIFF) { Serial.print("  WARNING: Large jump between gate "); Serial.print(i); Serial.print(" and "); Serial.print(i+1); Serial.print(" (diff = "); Serial.print(diff,1); Serial.println(")"); spatialFailures++; }
  }
  if (spatialFailures == 0) Serial.println("  PASS: Spatial distribution looks normal"); else { Serial.print("  WARNING: "); Serial.print(spatialFailures); Serial.println(" spatial discontinuities detected"); }

  Serial.println(); Serial.println("Check 3: Live Detection Test"); Serial.println("  Testing sensor response to movement...");
  Serial.println("  Ready? Press ENTER to start test..."); while (!Serial.available()) delay(100); while (Serial.available()) Serial.read(); Serial.println("  NOW! Move in front of sensor...");
  unsigned long testStart = millis(); int maxActiveGates = 0; int detectionCount = 0;
  while (millis() - testStart < 5000) {
    readFrame(); int activeGates = 0; for (int i = 0; i < NUM_GATES; i++) { float threshold = baseline_mean[i] + 2 * baseline_std[i]; if (latestMovingGates[i] > threshold) activeGates++; }
    if (activeGates > maxActiveGates) maxActiveGates = activeGates; if (activeGates >= MIN_DETECTION_GATES) detectionCount++; delay(50);
  }
  Serial.println("  Test complete!"); Serial.print("  Max active gates: "); Serial.println(maxActiveGates); Serial.print("  Detection frames: "); Serial.print(detectionCount); Serial.println(" / ~100");
  if (maxActiveGates >= MIN_DETECTION_GATES && detectionCount > 10) Serial.println("  PASS: Sensor responds to movement"); else { Serial.println("  FAIL: Sensor did not detect movement"); allPassed = false; }

  Serial.println(); Serial.println("========================================"); if (allPassed) { Serial.println("VALIDATION PASSED ✓"); } else { Serial.println("VALIDATION FAILED ✗"); }
  Serial.println("========================================"); Serial.println(); return allPassed;
}

bool quickValidateBaseline() {
  if (!baseline_valid) return false;
  for (int i = 0; i < NUM_GATES; i++) { if (baseline_mean[i] > 0) { float ratio = baseline_std[i] / baseline_mean[i]; if (ratio > MAX_STD_RATIO) return false; } }
  return true;
}
