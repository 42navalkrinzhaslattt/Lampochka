// features.cpp - shared feature implementations
#include "features.h"
#include "config.h"
#include "baseline.h"
#include <Arduino.h>

void updateHistory() {
  for (int i = 0; i < NUM_GATES; i++) { movingHistory[historyIndex][i] = latestMovingGates[i]; stationaryHistory[historyIndex][i] = latestStationaryGates[i]; }
  historyIndex = (historyIndex + 1) % HISTORY_SIZE; if (historyCount < HISTORY_SIZE) historyCount++;
  bool currentlyDetecting = false; for (int i = 0; i < NUM_GATES; i++) { if (latestMovingGates[i] > ENERGY_THRESHOLD || latestStationaryGates[i] > ENERGY_THRESHOLD) { currentlyDetecting = true; break; } }
  if (currentlyDetecting && !isDetecting) { firstDetectionTime = millis(); isDetecting = true; } else if (!currentlyDetecting) isDetecting = false;
}

void outputSpatialFeatures() { /* unused in detectors but kept for CSV logger */ }
void outputTemporalFeatures() { /* unused in detectors but kept for CSV logger */ }
void outputPatternFeatures() { /* unused in detectors but kept for CSV logger */ }

float calculateMaxEnergy(uint8_t gates[]) { float maxVal = 0; for (int i = 0; i < NUM_GATES; i++) if (gates[i] > maxVal) maxVal = gates[i]; return maxVal; }
float calculateTotalEnergy(uint8_t gates[]) { float total = 0; for (int i = 0; i < NUM_GATES; i++) total += gates[i]; return total; }
float calculateActiveZones(uint8_t gates[]) { int count = 0; for (int i = 0; i < NUM_GATES; i++) if (gates[i] > ENERGY_THRESHOLD) count++; return count; }
float calculateCentroid(uint8_t gates[]) { float weightedSum = 0, totalEnergy = 0; for (int i = 0; i < NUM_GATES; i++) { weightedSum += gates[i] * i; totalEnergy += gates[i]; } return (totalEnergy > 0) ? weightedSum / totalEnergy : 0; }
float calculateSpread(uint8_t gates[]) { float centroid = calculateCentroid(gates); float variance = 0, totalEnergy = 0; for (int i = 0; i < NUM_GATES; i++) { variance += gates[i] * (i - centroid) * (i - centroid); totalEnergy += gates[i]; } return (totalEnergy > 0) ? sqrt(variance / totalEnergy) : 0; }
float calculateChangeRate(uint8_t history[][NUM_GATES]) { if (HISTORY_SIZE < 2) return 0; float totalChange = 0; for (int i = 0; i < NUM_GATES; i++) totalChange += abs(history[0][i] - history[HISTORY_SIZE-1][i]); return totalChange / (HISTORY_SIZE - 1); }
float calculateVariance(uint8_t history[][NUM_GATES]) { float variance = 0; for (int i = 0; i < NUM_GATES; i++) { float mean = 0; for (int t = 0; t < HISTORY_SIZE; t++) mean += history[t][i]; mean /= HISTORY_SIZE; float gateVar = 0; for (int t = 0; t < HISTORY_SIZE; t++) gateVar += (history[t][i] - mean) * (history[t][i] - mean); variance += gateVar / HISTORY_SIZE; } return variance / NUM_GATES; }
float calculateZoneSwitches() { int switches = 0; for (int i = 0; i < NUM_GATES; i++) { bool wasActive = movingHistory[HISTORY_SIZE-1][i] > ENERGY_THRESHOLD; bool isActive = latestMovingGates[i] > ENERGY_THRESHOLD; if (wasActive != isActive) switches++; } return switches; }
float calculateMotionDuration() { int duration = 0; for (int t = 0; t < HISTORY_SIZE; t++) { bool hasMotion = false; for (int i = 0; i < NUM_GATES; i++) { if (movingHistory[t][i] > ENERGY_THRESHOLD) { hasMotion = true; break; } } if (hasMotion) duration++; } return duration; }
float calculateSequentialActivation() { int sequential = 0; for (int i = 0; i < NUM_GATES - 1; i++) { if (latestMovingGates[i] > ENERGY_THRESHOLD && latestMovingGates[i+1] > ENERGY_THRESHOLD) sequential++; } return sequential; }
float calculateZoneCorrelation() { float correlation = 0; int correlationCount = 0; for (int i = 0; i < NUM_GATES - 1; i++) { if ((latestMovingGates[i] > ENERGY_THRESHOLD && latestMovingGates[i+1] > ENERGY_THRESHOLD) || (latestMovingGates[i] <= ENERGY_THRESHOLD && latestMovingGates[i+1] <= ENERGY_THRESHOLD)) correlation++; correlationCount++; } if (correlationCount > 0) return correlation; return 0; }
