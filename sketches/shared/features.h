// features.h - declarations for feature helpers
#ifndef SHARED_FEATURES_H
#define SHARED_FEATURES_H

#include <Arduino.h>
#include "../record_data_ld2412_csv/config.h"

// Update internal history buffers from latest sensor readings
void updateHistory();

// CSV logger helpers (no-op for detectors, kept for compatibility)
void outputSpatialFeatures();
void outputTemporalFeatures();
void outputPatternFeatures();

// Functions used by detectors (implementations live in features.cpp)
float calculateMaxEnergy(uint8_t gates[]);
float calculateTotalEnergy(uint8_t gates[]);
float calculateActiveZones(uint8_t gates[]);
float calculateCentroid(uint8_t gates[]);
float calculateSpread(uint8_t gates[]);
float calculateChangeRate(uint8_t history[][NUM_GATES]);
float calculateVariance(uint8_t history[][NUM_GATES]);
float calculateZoneSwitches();
float calculateMotionDuration();
float calculateSequentialActivation();
float calculateZoneCorrelation();

#endif // SHARED_FEATURES_H
