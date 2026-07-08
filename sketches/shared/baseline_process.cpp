// baseline_process.cpp
#include "baseline_process.h"
#include "baseline_stats.h"
#include "config.h"
#include "baseline.h"
#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

bool processBaseline() {
  Serial.println("Phase 3: Processing with Outlier Rejection");
  Serial.println("  Using Median Absolute Deviation (MAD) method");
  Serial.println();

  if (!baseline_raw_moving || !baseline_raw_stationary) {
    Serial.println("ERROR: No raw data available");
    return false;
  }

  uint8_t* gateData = (uint8_t*)malloc(BASELINE_SAMPLES);
  uint8_t* cleanData = (uint8_t*)malloc(BASELINE_SAMPLES);
  if (!gateData || !cleanData) { if (gateData) free(gateData); if (cleanData) free(cleanData); return false; }

  int totalOutliers = 0; int totalSkipped = 0;
  for (int gate = 0; gate < NUM_GATES; gate++) {
    Serial.print("  Gate "); Serial.print(gate); Serial.print(": ");
    for (int s = 0; s < BASELINE_SAMPLES; s++) gateData[s] = baseline_raw_stationary[s * NUM_GATES + gate];
    float median = calculateMedian(gateData, BASELINE_SAMPLES); baseline_median[gate] = median;
    float rawMean = calculateMean(gateData, BASELINE_SAMPLES);
    float rawStd = calculateStd(gateData, BASELINE_SAMPLES, rawMean);
    if (rawStd < SKIP_REJECTION_STD) {
      baseline_mean[gate] = rawMean;
      baseline_std[gate] = max(rawStd, MIN_STD);
      baseline_min[gate] = calculateMin(gateData, BASELINE_SAMPLES);
      baseline_max[gate] = calculateMax(gateData, BASELINE_SAMPLES);
      baseline_95th[gate] = calculatePercentile(gateData, BASELINE_SAMPLES, 95.0);
      Serial.print("mean="); Serial.print(baseline_mean[gate],1); Serial.print(" std="); Serial.print(baseline_std[gate],1);
      Serial.println(" (stable, no rejection needed)");
      totalSkipped++;
      baseline_clean_count[gate] = BASELINE_SAMPLES; baseline_confident[gate] = true; continue;
    }
    float mad = calculateMAD(gateData, BASELINE_SAMPLES, median);
    if (mad < MIN_MAD) mad = MIN_MAD;
    int validCount = countValidSamples(gateData, BASELINE_SAMPLES, median, mad);
    int outlierCount = BASELINE_SAMPLES - validCount; totalOutliers += outlierCount;
    int cleanCount = copyValidSamples(gateData, BASELINE_SAMPLES, median, mad, cleanData);
    if (cleanCount < MIN_CLEAN_SAMPLES) {
      Serial.print("WARNING: Only "); Serial.print(cleanCount); Serial.print(" clean samples for gate "); Serial.print(gate);
      Serial.print(" (recommended "); Serial.print(MIN_CLEAN_SAMPLES); Serial.println("+). Marking low-confidence and continuing.");
      baseline_clean_count[gate] = cleanCount; baseline_confident[gate] = false;
      if (cleanCount > 0) {
        baseline_mean[gate] = calculateMean(cleanData, cleanCount);
        baseline_std[gate] = calculateStd(cleanData, cleanCount, baseline_mean[gate]); if (baseline_std[gate] < MIN_STD) baseline_std[gate] = MIN_STD;
        baseline_min[gate] = calculateMin(cleanData, cleanCount);
        baseline_max[gate] = calculateMax(cleanData, cleanCount);
        baseline_95th[gate] = calculatePercentile(cleanData, cleanCount, 95.0);
      } else {
        baseline_mean[gate] = median; baseline_std[gate] = max(rawStd, MIN_STD);
        baseline_min[gate] = calculateMin(gateData, BASELINE_SAMPLES);
        baseline_max[gate] = calculateMax(gateData, BASELINE_SAMPLES);
        baseline_95th[gate] = calculatePercentile(gateData, BASELINE_SAMPLES, 95.0);
      }
      continue;
    }
    baseline_mean[gate] = calculateMean(cleanData, cleanCount);
    baseline_std[gate] = calculateStd(cleanData, cleanCount, baseline_mean[gate]); if (baseline_std[gate] < MIN_STD) baseline_std[gate] = MIN_STD;
    baseline_min[gate] = calculateMin(cleanData, cleanCount);
    baseline_max[gate] = calculateMax(cleanData, cleanCount);
    baseline_95th[gate] = calculatePercentile(cleanData, cleanCount, 95.0);
    baseline_clean_count[gate] = cleanCount; baseline_confident[gate] = (cleanCount >= MIN_CLEAN_SAMPLES);
    Serial.print("mean="); Serial.print(baseline_mean[gate],1); Serial.print(" std="); Serial.print(baseline_std[gate],1);
    Serial.print(" ("); Serial.print(outlierCount); Serial.println(" outliers removed)");
  }
  free(gateData); free(cleanData);
  Serial.println(); Serial.print("  Gates with stable data (skipped rejection): "); Serial.println(totalSkipped);
  Serial.print("  Total outliers removed: "); Serial.print(totalOutliers); Serial.print(" / "); Serial.print(BASELINE_SAMPLES * NUM_GATES);
  Serial.print(" ("); Serial.print((totalOutliers * 100) / (BASELINE_SAMPLES * NUM_GATES)); Serial.println("%)");
  Serial.println();
  return true;
}

void printBaselineStats() {
  Serial.println("Baseline Statistics:");
  Serial.println("  Gate |  Mean  |  Std   | Median |  Min   |  Max   | 95th % | Conf");
  Serial.println("  -----|--------|--------|--------|--------|--------|--------|------");
  for (int i = 0; i < NUM_GATES; i++) {
    Serial.print("    "); if (i < 10) Serial.print(" "); Serial.print(i); Serial.print(" | ");
    printFixedWidth(baseline_mean[i], 6, 1); Serial.print(" | "); printFixedWidth(baseline_std[i], 6, 1); Serial.print(" | ");
    printFixedWidth(baseline_median[i], 6, 1); Serial.print(" | "); printFixedWidth(baseline_min[i], 6, 1); Serial.print(" | ");
    printFixedWidth(baseline_max[i], 6, 1); Serial.print(" | "); printFixedWidth(baseline_95th[i], 6, 1);
    Serial.print(" |"); int confPercent = (BASELINE_SAMPLES > 0) ? ((baseline_clean_count[i] * 100) / BASELINE_SAMPLES) : 0;
    if (confPercent < 10) Serial.print(" "); Serial.print(confPercent); Serial.println("%");
  }
  Serial.println();
}

void printFixedWidth(float value, int width, int decimals) {
  String str = String(value, decimals);
  while (str.length() < width) str = " " + str;
  Serial.print(str);
}
