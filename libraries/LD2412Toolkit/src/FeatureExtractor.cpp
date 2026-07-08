#include "FeatureExtractor.h"

float FeatureExtractor::calculateMaxEnergy(const uint8_t *gates, int numGates) {
  uint8_t maxVal = 0;
  for (int i = 0; i < numGates; i++)
    if (gates[i] > maxVal)
      maxVal = gates[i];
  return (float)maxVal;
}

float FeatureExtractor::calculateTotalEnergy(const uint8_t *gates,
                                             int numGates) {
  float sum = 0;
  for (int i = 0; i < numGates; i++)
    sum += gates[i];
  return sum;
}

float FeatureExtractor::calculateActiveZones(const uint8_t *gates, int numGates,
                                             int threshold) {
  int count = 0;
  for (int i = 0; i < numGates; i++)
    if (gates[i] > threshold)
      count++;
  return (float)count;
}

float FeatureExtractor::calculateCentroid(const uint8_t *gates, int numGates) {
  float weightedSum = 0;
  float totalEnergy = 0;
  for (int i = 0; i < numGates; i++) {
    weightedSum += i * gates[i];
    totalEnergy += gates[i];
  }
  return (totalEnergy > 0) ? (weightedSum / totalEnergy) : 0;
}

float FeatureExtractor::calculateSpread(const uint8_t *gates, int numGates,
                                        float centroid) {
  float weightedVar = 0;
  float totalEnergy = 0;
  for (int i = 0; i < numGates; i++) {
    float diff = i - centroid;
    weightedVar += (diff * diff) * gates[i];
    totalEnergy += gates[i];
  }
  return (totalEnergy > 0) ? sqrt(weightedVar / totalEnergy) : 0;
}
