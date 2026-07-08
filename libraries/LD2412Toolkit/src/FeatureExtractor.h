#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include <Arduino.h>

class FeatureExtractor {
public:
  static float calculateMaxEnergy(const uint8_t *gates, int numGates);
  static float calculateTotalEnergy(const uint8_t *gates, int numGates);
  static float calculateActiveZones(const uint8_t *gates, int numGates,
                                    int threshold = 10);
  static float calculateCentroid(const uint8_t *gates, int numGates);
  static float calculateSpread(const uint8_t *gates, int numGates,
                               float centroid);
};

#endif
