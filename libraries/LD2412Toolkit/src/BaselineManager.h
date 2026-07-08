#ifndef BASELINE_MANAGER_H
#define BASELINE_MANAGER_H

#include "LD2412Sensor.h"
#include <Arduino.h>

// Maximum unique values per gate (uint8_t range is 0-255)
const int MAX_UNIQUE_VALUES = 256;

class BaselineManager {
public:
  BaselineManager(int numGates, int samples, int interval, int warmupMs);
  ~BaselineManager();

  bool collectBlocking(LD2412Sensor &sensor);

  bool process();
  void saveToRAM();
  void freeBuffers();

  // Distance-to-set normalization (stationary gates)
  int distanceToSet(uint8_t rawValue, int gate);

  // Distance-to-set normalization (moving gates)
  int distanceToSetMoving(uint8_t rawValue, int gate);

  // Legacy aliases for compatibility
  int normalize(uint8_t rawValue, int gate) {
    return distanceToSet(rawValue, gate);
  }
  int normalizeWithClamp(uint8_t rawValue, int gate, float k = 0,
                         float zMax = 0) {
    (void)k;
    (void)zMax; // Unused
    return distanceToSet(rawValue, gate);
  }
  int normalizeMoving(uint8_t rawValue, int gate) {
    return distanceToSetMoving(rawValue, gate);
  }
  int normalizeMovingWithClamp(uint8_t rawValue, int gate, float k = 0,
                               float zMax = 0) {
    (void)k;
    (void)zMax; // Unused
    return distanceToSetMoving(rawValue, gate);
  }

  void printStats(Stream &out);
  void printBaselineSets(Stream &out);

  bool valid() const { return _valid; }
  unsigned long timestamp() const { return _timestamp; }

  // Fallback if processing fails
  void fallbackFromRaw();

private:
  int _numGates;
  int _samples;
  int _interval;
  int _warmupMs;

  bool _valid;
  unsigned long _timestamp;

  // Sorted unique baseline values per gate (stationary)
  uint8_t **_sortedStationary; // Array of sorted unique values per gate
  int *_sortedCountStationary; // Count of unique values per gate

  // Sorted unique baseline values per gate (moving)
  uint8_t **_sortedMoving; // Array of sorted unique values per gate
  int *_sortedCountMoving; // Count of unique values per gate

  // Raw buffers (only during collection, freed after processing)
  uint8_t *_rawMoving;
  uint8_t *_rawStationary;

  // Helpers
  int binarySearchClosest(uint8_t *arr, int count, uint8_t value);
  void sortAndUnique(uint8_t *data, int count, uint8_t *outSorted,
                     int &outCount);
  void insertionSort(uint8_t *arr, int count);
};

#endif
