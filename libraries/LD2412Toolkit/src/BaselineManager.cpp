#include "BaselineManager.h"

BaselineManager::BaselineManager(int numGates, int samples, int interval,
                                 int warmupMs)
    : _numGates(numGates), _samples(samples), _interval(interval),
      _warmupMs(warmupMs), _valid(false), _timestamp(0), _rawMoving(nullptr),
      _rawStationary(nullptr) {

  // Allocate arrays of pointers for sorted values
  _sortedStationary = new uint8_t *[numGates];
  _sortedMoving = new uint8_t *[numGates];
  _sortedCountStationary = new int[numGates];
  _sortedCountMoving = new int[numGates];

  for (int i = 0; i < numGates; i++) {
    _sortedStationary[i] = nullptr;
    _sortedMoving[i] = nullptr;
    _sortedCountStationary[i] = 0;
    _sortedCountMoving[i] = 0;
  }
}

BaselineManager::~BaselineManager() {
  // Free sorted arrays
  for (int i = 0; i < _numGates; i++) {
    if (_sortedStationary[i]) {
      delete[] _sortedStationary[i];
    }
    if (_sortedMoving[i]) {
      delete[] _sortedMoving[i];
    }
  }
  delete[] _sortedStationary;
  delete[] _sortedCountStationary;
  delete[] _sortedMoving;
  delete[] _sortedCountMoving;

  freeBuffers();
}

void BaselineManager::freeBuffers() {
  if (_rawMoving) {
    free(_rawMoving);
    _rawMoving = nullptr;
  }
  if (_rawStationary) {
    free(_rawStationary);
    _rawStationary = nullptr;
  }
}

bool BaselineManager::collectBlocking(LD2412Sensor &sensor) {
  freeBuffers();
  size_t total = (size_t)_samples * (size_t)_numGates;
  _rawMoving = (uint8_t *)malloc(total);
  _rawStationary = (uint8_t *)malloc(total);

  if (!_rawMoving || !_rawStationary) {
    freeBuffers();
    return false;
  }

  // Warmup
  Serial.println("Phase 1: Sensor Warmup");
  unsigned long start = millis();
  int lastPercent = -1;
  while (millis() - start < (unsigned long)_warmupMs) {
    sensor.readFrame();
    int percent = ((millis() - start) * 100) / _warmupMs;
    if (percent != lastPercent && percent % 10 == 0) {
      Serial.print("  ");
      Serial.print(percent);
      Serial.println("%");
      lastPercent = percent;
    }
    delay(10);
  }
  Serial.println("  Warmup complete!");

  // Collection (with timeout to avoid hanging on disconnected sensors)
  Serial.println("\nPhase 2: Data Collection");
  int collected = 0;
  unsigned long lastSample = millis();
  unsigned long lastFrame = millis(); // Track last successful frame for timeout
  const unsigned long COLLECT_TIMEOUT_MS = 30000; // 30s timeout
  lastPercent = -1;
  while (collected < _samples) {
    // Timeout: if no frames received for COLLECT_TIMEOUT_MS, abort
    if (millis() - lastFrame > COLLECT_TIMEOUT_MS) {
      Serial.println("  TIMEOUT: No frames received — sensor may be disconnected.");
      return false;
    }
    if (sensor.readFrame()) {
      lastFrame = millis();
      if (millis() - lastSample >= (unsigned long)_interval) {
        lastSample = millis();
        const uint8_t *mv = sensor.moving();
        const uint8_t *st = sensor.stationary();

        // Print raw values for debugging
        Serial.print("raw_baseline: ");
        for (int i = 0; i < _numGates; i++) {
          Serial.print(mv[i]);
          Serial.print(",");
        }
        for (int i = 0; i < _numGates; i++) {
          Serial.print(st[i]);
          if (i < _numGates - 1)
            Serial.print(",");
        }
        Serial.println();

        for (int i = 0; i < _numGates; i++) {
          _rawMoving[collected * _numGates + i] = mv[i];
          _rawStationary[collected * _numGates + i] = st[i];
        }
        collected++;

        int percent = (collected * 100) / _samples;
        if (percent != lastPercent && percent % 10 == 0) {
          Serial.print("  ");
          Serial.print(percent);
          Serial.print("% (");
          Serial.print(collected);
          Serial.print("/");
          Serial.print(_samples);
          Serial.println(")");
          lastPercent = percent;
        }
      }
    }
  }
  Serial.println("  Collection complete!");
  return true;
}

void BaselineManager::insertionSort(uint8_t *arr, int count) {
  for (int i = 1; i < count; i++) {
    uint8_t key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

void BaselineManager::sortAndUnique(uint8_t *data, int count,
                                    uint8_t *outSorted, int &outCount) {
  // Copy data to output
  for (int i = 0; i < count; i++) {
    outSorted[i] = data[i];
  }

  // Sort using insertion sort (efficient for small arrays)
  insertionSort(outSorted, count);

  // Remove duplicates
  outCount = 0;
  for (int i = 0; i < count; i++) {
    if (outCount == 0 || outSorted[i] != outSorted[outCount - 1]) {
      outSorted[outCount++] = outSorted[i];
    }
  }
}

bool BaselineManager::process() {
  if (!_rawStationary || !_rawMoving) {
    Serial.println("DEBUG: raw buffers are NULL in process()");
    return false;
  }

  Serial.println("\nPhase 3: Processing (Building Sorted Sets)");

  // Temporary buffer for sorting (reused for each gate)
  uint8_t *tempBuffer = new uint8_t[_samples];
  uint8_t *sortedBuffer = new uint8_t[_samples];

  if (!tempBuffer || !sortedBuffer) {
    if (tempBuffer)
      delete[] tempBuffer;
    if (sortedBuffer)
      delete[] sortedBuffer;
    return false;
  }

  // Process stationary gates
  for (int gate = 0; gate < _numGates; gate++) {
    // Extract gate data
    for (int s = 0; s < _samples; s++) {
      tempBuffer[s] = _rawStationary[s * _numGates + gate];
    }

    // Sort and get unique values
    int uniqueCount = 0;
    sortAndUnique(tempBuffer, _samples, sortedBuffer, uniqueCount);

    // Allocate and store
    if (_sortedStationary[gate]) {
      delete[] _sortedStationary[gate];
    }
    _sortedStationary[gate] = new uint8_t[uniqueCount];
    _sortedCountStationary[gate] = uniqueCount;
    for (int i = 0; i < uniqueCount; i++) {
      _sortedStationary[gate][i] = sortedBuffer[i];
    }

    Serial.print("  Gate ");
    Serial.print(gate);
    Serial.print(" STAT: ");
    Serial.print(uniqueCount);
    Serial.println(" unique values");
  }

  // Process moving gates
  for (int gate = 0; gate < _numGates; gate++) {
    // Extract gate data
    for (int s = 0; s < _samples; s++) {
      tempBuffer[s] = _rawMoving[s * _numGates + gate];
    }

    // Sort and get unique values
    int uniqueCount = 0;
    sortAndUnique(tempBuffer, _samples, sortedBuffer, uniqueCount);

    // Allocate and store
    if (_sortedMoving[gate]) {
      delete[] _sortedMoving[gate];
    }
    _sortedMoving[gate] = new uint8_t[uniqueCount];
    _sortedCountMoving[gate] = uniqueCount;
    for (int i = 0; i < uniqueCount; i++) {
      _sortedMoving[gate][i] = sortedBuffer[i];
    }

    Serial.print("  Gate ");
    Serial.print(gate);
    Serial.print(" MOVE: ");
    Serial.print(uniqueCount);
    Serial.println(" unique values");
  }

  delete[] tempBuffer;
  delete[] sortedBuffer;

  _valid = true;
  _timestamp = millis();
  return true;
}

void BaselineManager::fallbackFromRaw() {
  // Just run normal process - it's simple enough
  process();
}

int BaselineManager::binarySearchClosest(uint8_t *arr, int count,
                                         uint8_t value) {
  if (count == 0)
    return -1;
  if (count == 1)
    return 0;

  int left = 0;
  int right = count - 1;

  // Binary search to find insertion point
  while (left < right) {
    int mid = left + (right - left) / 2;
    if (arr[mid] == value) {
      return mid; // Exact match
    } else if (arr[mid] < value) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }

  // Now left == right, find closest between left and left-1
  if (left == 0) {
    return 0;
  } else if (left >= count) {
    return count - 1;
  } else {
    // Check which neighbor is closer
    int distLeft = abs((int)value - (int)arr[left - 1]);
    int distRight = abs((int)value - (int)arr[left]);
    return (distLeft <= distRight) ? (left - 1) : left;
  }
}

int BaselineManager::distanceToSet(uint8_t rawValue, int gate) {
  if (!_valid || gate < 0 || gate >= _numGates) {
    return 0;
  }

  int count = _sortedCountStationary[gate];
  if (count == 0)
    return 0;

  uint8_t *arr = _sortedStationary[gate];
  int closestIdx = binarySearchClosest(arr, count, rawValue);

  if (closestIdx < 0)
    return 0;

  return abs((int)rawValue - (int)arr[closestIdx]);
}

int BaselineManager::distanceToSetMoving(uint8_t rawValue, int gate) {
  if (!_valid || gate < 0 || gate >= _numGates) {
    return 0;
  }

  int count = _sortedCountMoving[gate];
  if (count == 0)
    return 0;

  uint8_t *arr = _sortedMoving[gate];
  int closestIdx = binarySearchClosest(arr, count, rawValue);

  if (closestIdx < 0)
    return 0;

  return abs((int)rawValue - (int)arr[closestIdx]);
}

void BaselineManager::saveToRAM() {
  // Already in RAM members
}

void BaselineManager::printStats(Stream &out) {
  out.println("\n========================================");
  out.println("Baseline Statistics (Distance-to-Set)");
  out.println("========================================\n");

  out.println("STATIONARY GATES:");
  out.println("  Gate  | Unique | Min | Max ");
  out.println("  ------|--------|-----|-----");
  for (int i = 0; i < _numGates; i++) {
    out.print("    ");
    if (i < 10)
      out.print(" ");
    out.print(i);
    out.print("  |   ");
    if (_sortedCountStationary[i] < 10)
      out.print(" ");
    if (_sortedCountStationary[i] < 100)
      out.print(" ");
    out.print(_sortedCountStationary[i]);
    out.print("  | ");
    if (_sortedCountStationary[i] > 0) {
      if (_sortedStationary[i][0] < 10)
        out.print(" ");
      if (_sortedStationary[i][0] < 100)
        out.print(" ");
      out.print(_sortedStationary[i][0]);
      out.print(" | ");
      if (_sortedStationary[i][_sortedCountStationary[i] - 1] < 10)
        out.print(" ");
      if (_sortedStationary[i][_sortedCountStationary[i] - 1] < 100)
        out.print(" ");
      out.print(_sortedStationary[i][_sortedCountStationary[i] - 1]);
    } else {
      out.print("  - |   -");
    }
    out.println();
  }

  out.println("\nMOVING GATES:");
  out.println("  Gate  | Unique | Min | Max ");
  out.println("  ------|--------|-----|-----");
  for (int i = 0; i < _numGates; i++) {
    out.print("    ");
    if (i < 10)
      out.print(" ");
    out.print(i);
    out.print("  |   ");
    if (_sortedCountMoving[i] < 10)
      out.print(" ");
    if (_sortedCountMoving[i] < 100)
      out.print(" ");
    out.print(_sortedCountMoving[i]);
    out.print("  | ");
    if (_sortedCountMoving[i] > 0) {
      if (_sortedMoving[i][0] < 10)
        out.print(" ");
      if (_sortedMoving[i][0] < 100)
        out.print(" ");
      out.print(_sortedMoving[i][0]);
      out.print(" | ");
      if (_sortedMoving[i][_sortedCountMoving[i] - 1] < 10)
        out.print(" ");
      if (_sortedMoving[i][_sortedCountMoving[i] - 1] < 100)
        out.print(" ");
      out.print(_sortedMoving[i][_sortedCountMoving[i] - 1]);
    } else {
      out.print("  - |   -");
    }
    out.println();
  }
}

void BaselineManager::printBaselineSets(Stream &out) {
  out.println("\n========================================");
  out.println("BASELINE SETS (All Unique Values)");
  out.println("========================================\n");

  out.println("STATIONARY GATES:");
  for (int gate = 0; gate < _numGates; gate++) {
    out.print("  Gate ");
    out.print(gate);
    out.print(" (");
    out.print(_sortedCountStationary[gate]);
    out.print(" values): ");
    for (int i = 0; i < _sortedCountStationary[gate]; i++) {
      out.print(_sortedStationary[gate][i]);
      if (i < _sortedCountStationary[gate] - 1) {
        out.print(", ");
      }
    }
    out.println();
  }

  out.println("\nMOVING GATES:");
  for (int gate = 0; gate < _numGates; gate++) {
    out.print("  Gate ");
    out.print(gate);
    out.print(" (");
    out.print(_sortedCountMoving[gate]);
    out.print(" values): ");
    for (int i = 0; i < _sortedCountMoving[gate]; i++) {
      out.print(_sortedMoving[gate][i]);
      if (i < _sortedCountMoving[gate] - 1) {
        out.print(", ");
      }
    }
    out.println();
  }

  out.println("\n========================================\n");
}
