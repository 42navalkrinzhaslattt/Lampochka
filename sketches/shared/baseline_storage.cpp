// baseline_storage.cpp
#include "baseline_storage.h"
#include "config.h"
#include "baseline.h"
#include <Arduino.h>

bool saveBaselineToRAM() {
  Serial.println("Baseline saved to RAM ✓");
  Serial.println("  WARNING: Will be lost on power cycle!");
  baseline_timestamp = millis(); baseline_valid = true; return true;
}

bool loadBaselineFromRAM() {
  if (baseline_valid) {
    Serial.println("Baseline already loaded from this session ✓");
    unsigned long age_ms = millis() - baseline_timestamp;
    if (age_ms < 3600000) { Serial.print("  Age: "); Serial.print(age_ms / 60000); Serial.println(" minutes"); }
    else { Serial.print("  Age: "); Serial.print(age_ms / 3600000); Serial.println(" hours"); }
    return true;
  } else {
    Serial.println("No baseline in memory."); Serial.println("  Use 'CALIBRATE' to collect baseline."); return false;
  }
}

void clearBaselineFromRAM() {
  Serial.print("Clearing baseline from RAM... ");
  for (int i = 0; i < NUM_GATES; i++) { baseline_mean[i] = 0; baseline_std[i] = 0; baseline_median[i] = 0; baseline_min[i] = 0; baseline_max[i] = 0; baseline_95th[i] = 0; }
  baseline_valid = false; baseline_timestamp = 0; Serial.println("OK");
}

bool checkBaselineExists() { return baseline_valid; }
