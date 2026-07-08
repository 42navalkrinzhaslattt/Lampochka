/*
  Feature Calculations for Multi-Sensor CSV Data Logger

  Functions for calculating derived features from raw sensor data.
  Includes spatial, temporal, and pattern features for ML training.
  Adapted for per-sensor state (sensor index passed to all functions).
*/

const float ACTIVE_ZONE_THRESHOLD = 1.0f;

void updateHistory(int s, BaselineManager &bl) {
  // Store current NORMALIZED readings in circular buffer
  for (int i = 0; i < NUM_GATES; i++) {
    int normMove = normalizeMovingValue(bl, latestMovingGates[s][i], i);
    int normStat = normalizeValue(bl, latestStationaryGates[s][i], i);

    // Clamp to uint8_t range (0-255) for storage
    if (normMove > 255)
      normMove = 255;
    if (normStat > 255)
      normStat = 255;

    movingHistory[s][historyIndex[s]][i] = (uint8_t)normMove;
    stationaryHistory[s][historyIndex[s]][i] = (uint8_t)normStat;
  }

  // Calculate and store metrics for new features (Energy & Centroid)
  float frameEnergy = 0;
  float frameCentroid = 0;
  float weightSum = 0;

  for (int i = 0; i < NUM_GATES; i++) {
    float val = (float)movingHistory[s][historyIndex[s]][i];
    frameEnergy += val;
    frameCentroid += i * val;
    weightSum += val;
  }
  if (weightSum > 0)
    frameCentroid /= weightSum;

  totalEnergyHistory[s][historyIndex[s]] = frameEnergy;
  centroidHistory[s][historyIndex[s]] = frameCentroid;

  historyIndex[s] = (historyIndex[s] + 1) % HISTORY_SIZE;
  if (historyCount[s] < HISTORY_SIZE)
    historyCount[s]++;

  // Update detection tracking using NORMALIZED values
  bool currentlyDetecting = false;
  for (int i = 0; i < NUM_GATES; i++) {
    float normMove = normalizeMovingValue(bl, latestMovingGates[s][i], i);
    float normStat = normalizeValue(bl, latestStationaryGates[s][i], i);

    if (normMove > ACTIVE_ZONE_THRESHOLD || normStat > ACTIVE_ZONE_THRESHOLD) {
      currentlyDetecting = true;
      break;
    }
  }

  if (currentlyDetecting && !isDetecting[s]) {
    firstDetectionTime[s] = millis();
    isDetecting[s] = true;
  } else if (!currentlyDetecting) {
    isDetecting[s] = false;
  }
}

// Helper to get distance-to-set for stationary gates
int normalizeValue(BaselineManager &bl, uint8_t rawValue, int gate) {
  return bl.distanceToSet(rawValue, gate);
}

// Helper to get distance-to-set for moving gates
int normalizeMovingValue(BaselineManager &bl, uint8_t rawValue, int gate) {
  return bl.distanceToSetMoving(rawValue, gate);
}

void outputSpatialFeatures(int s, BaselineManager &bl) {
  // Moving spatial features
  float maxMoveEnergy = 0;
  float totalMoveEnergy = 0;
  int activeMoveZones = 0;
  float moveCentroid = 0;
  float weightSum = 0;

  for (int i = 0; i < NUM_GATES; i++) {
    float energy = normalizeMovingValue(bl, latestMovingGates[s][i], i);
    if (energy < 0)
      energy = 0;

    if (energy > maxMoveEnergy)
      maxMoveEnergy = energy;
    totalMoveEnergy += energy;

    if (energy > ACTIVE_ZONE_THRESHOLD)
      activeMoveZones++;

    moveCentroid += i * energy;
    weightSum += energy;
  }
  if (weightSum > 0)
    moveCentroid /= weightSum;

  // Moving spread
  float moveSpread = 0;
  if (weightSum > 0) {
    for (int i = 0; i < NUM_GATES; i++) {
      float energy = normalizeMovingValue(bl, latestMovingGates[s][i], i);
      if (energy < 0)
        energy = 0;
      float diff = i - moveCentroid;
      moveSpread += diff * diff * energy;
    }
    moveSpread /= weightSum;
  }

  // Stationary spatial features
  float maxStatEnergy = 0;
  float totalStatEnergy = 0;
  int activeStatZones = 0;
  float statCentroid = 0;
  weightSum = 0;

  for (int i = 0; i < NUM_GATES; i++) {
    float energy = normalizeValue(bl, latestStationaryGates[s][i], i);
    if (energy < 0)
      energy = 0;

    if (energy > maxStatEnergy)
      maxStatEnergy = energy;
    totalStatEnergy += energy;
    if (energy > ACTIVE_ZONE_THRESHOLD)
      activeStatZones++;

    statCentroid += i * energy;
    weightSum += energy;
  }
  if (weightSum > 0)
    statCentroid /= weightSum;

  // Stationary spread
  float statSpread = 0;
  if (weightSum > 0) {
    for (int i = 0; i < NUM_GATES; i++) {
      float energy = normalizeValue(bl, latestStationaryGates[s][i], i);
      if (energy < 0)
        energy = 0;
      float diff = i - statCentroid;
      statSpread += diff * diff * energy;
    }
    statSpread /= weightSum;
  }

  // Output spatial features
  Serial.print(",");
  Serial.print(maxMoveEnergy, 1);
  Serial.print(",");
  Serial.print(totalMoveEnergy, 1);
  Serial.print(",");
  Serial.print(activeMoveZones);
  Serial.print(",");
  Serial.print(moveCentroid, 2);
  Serial.print(",");
  Serial.print(moveSpread, 2);

  Serial.print(",");
  Serial.print(maxStatEnergy, 1);
  Serial.print(",");
  Serial.print(totalStatEnergy, 1);
  Serial.print(",");
  Serial.print(activeStatZones);
  Serial.print(",");
  Serial.print(statCentroid, 2);
  Serial.print(",");
  Serial.print(statSpread, 2);
}

void outputTemporalFeatures(int s, BaselineManager &bl) {
  float moveChangeRate = 0;
  float statChangeRate = 0;
  int zoneSwitches = 0;
  float motionDuration = 0;
  float moveVariance = 0;
  float statVariance = 0;

  if (historyCount[s] >= 2) {
    int prevIndex = (historyIndex[s] - 2 + HISTORY_SIZE) % HISTORY_SIZE;

    float prevMoveTotal = 0, currMoveTotal = 0;
    float prevStatTotal = 0, currStatTotal = 0;

    for (int i = 0; i < NUM_GATES; i++) {
      prevMoveTotal += movingHistory[s][prevIndex][i];
      currMoveTotal += normalizeMovingValue(bl, latestMovingGates[s][i], i);

      prevStatTotal += stationaryHistory[s][prevIndex][i];
      currStatTotal += normalizeValue(bl, latestStationaryGates[s][i], i);
    }

    moveChangeRate = currMoveTotal - prevMoveTotal;
    statChangeRate = currStatTotal - prevStatTotal;

    // Zone switches
    int prevActiveZone = -1, currActiveZone = -1;
    float maxEnergy = 0;

    for (int i = 0; i < NUM_GATES; i++) {
      if (movingHistory[s][prevIndex][i] > maxEnergy) {
        maxEnergy = movingHistory[s][prevIndex][i];
        prevActiveZone = i;
      }
    }

    maxEnergy = 0;
    for (int i = 0; i < NUM_GATES; i++) {
      float normVal = normalizeMovingValue(bl, latestMovingGates[s][i], i);
      if (normVal > maxEnergy) {
        maxEnergy = normVal;
        currActiveZone = i;
      }
    }

    if (prevActiveZone != currActiveZone && currActiveZone >= 0) {
      zoneSwitches = 1;
    }
  }

  if (isDetecting[s] && firstDetectionTime[s] > 0) {
    motionDuration = (millis() - firstDetectionTime[s]) / 1000.0;
  }

  if (historyCount[s] >= 5) {
    float moveMean = 0, statMean = 0;

    for (int t = 0; t < historyCount[s]; t++) {
      for (int i = 0; i < NUM_GATES; i++) {
        moveMean += movingHistory[s][t][i];
        statMean += stationaryHistory[s][t][i];
      }
    }
    moveMean /= (historyCount[s] * NUM_GATES);
    statMean /= (historyCount[s] * NUM_GATES);

    for (int t = 0; t < historyCount[s]; t++) {
      for (int i = 0; i < NUM_GATES; i++) {
        float moveDiff = movingHistory[s][t][i] - moveMean;
        float statDiff = stationaryHistory[s][t][i] - statMean;
        moveVariance += moveDiff * moveDiff;
        statVariance += statDiff * statDiff;
      }
    }
    moveVariance /= (historyCount[s] * NUM_GATES);
    statVariance /= (historyCount[s] * NUM_GATES);
  }

  // Output temporal features
  Serial.print(",");
  Serial.print(moveChangeRate, 1);
  Serial.print(",");
  Serial.print(statChangeRate, 1);
  Serial.print(",");
  Serial.print(zoneSwitches);
  Serial.print(",");
  Serial.print(motionDuration, 2);
  Serial.print(",");
  Serial.print(moveVariance, 2);
  Serial.print(",");
  Serial.print(statVariance, 2);
}

void outputPatternFeatures(int s, BaselineManager &bl) {
  bool zonesSequential = false;
  int firstActive = -1, lastActive = -1;

  for (int i = 0; i < NUM_GATES; i++) {
    if (normalizeMovingValue(bl, latestMovingGates[s][i], i) >
        ACTIVE_ZONE_THRESHOLD) {
      if (firstActive == -1)
        firstActive = i;
      lastActive = i;
    }
  }

  if (firstActive >= 0 && lastActive > firstActive) {
    zonesSequential = true;
    for (int i = firstActive; i <= lastActive; i++) {
      if (normalizeMovingValue(bl, latestMovingGates[s][i], i) <=
          ACTIVE_ZONE_THRESHOLD) {
        zonesSequential = false;
        break;
      }
    }
  }

  float zoneCorrelation = 0;
  int correlationCount = 0;

  for (int i = 0; i < NUM_GATES - 1; i++) {
    float val1 = normalizeMovingValue(bl, latestMovingGates[s][i], i);
    float val2 = normalizeMovingValue(bl, latestMovingGates[s][i + 1], i + 1);

    if ((val1 > ACTIVE_ZONE_THRESHOLD && val2 > ACTIVE_ZONE_THRESHOLD) ||
        (val1 <= ACTIVE_ZONE_THRESHOLD && val2 <= ACTIVE_ZONE_THRESHOLD)) {
      zoneCorrelation += 1.0;
    }
    correlationCount++;
  }

  if (correlationCount > 0) {
    zoneCorrelation /= correlationCount;
  }

  // Output pattern features
  Serial.print(",");
  Serial.print(zonesSequential ? 1 : 0);
  Serial.print(",");
  Serial.print(zoneCorrelation, 2);
}

// ==========================================
// NEW FEATURES (v2/v3/v4 Model)
// ==========================================

// A. Weighted Peak Stability
float getWeightedPeakStability(int s, const uint8_t *gates, int &outMaxIdx) {
  float maxE = 0;
  int maxIdx = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    if (gates[i] > maxE) {
      maxE = gates[i];
      maxIdx = i;
    }
  }
  outMaxIdx = maxIdx;

  if (prevPeakIdx[s] == -1)
    return 0.0f;

  float logE = log(1.0f + maxE);
  return abs(maxIdx - prevPeakIdx[s]) * logE;
}

// B. Spatial Smoothness Index
float getSpatialSmoothness(const uint8_t *gates) {
  float totalVar = 0;
  float totalE = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    totalE += gates[i];
    if (i < NUM_GATES - 1) {
      totalVar += abs((float)gates[i] - (float)gates[i + 1]);
    }
  }
  return totalVar / (totalE + 1e-9f);
}

// C. Centroid Velocity (Smoothed)
float getCentroidVelocity(int s) {
  if (historyCount[s] < 5)
    return 0.0f;
  float num = 0;
  for (int i = 0; i < 5; i++) {
    int idx = (historyIndex[s] - 5 + i + HISTORY_SIZE) % HISTORY_SIZE;
    float c = centroidHistory[s][idx];
    float w = (float)(i - 2);
    num += w * c;
  }
  return num / 10.0f;
}

// D. Movement Linearity (R^2)
float getMovementLinearity(int s) {
  if (historyCount[s] < 5)
    return 0.0f;
  int N = (historyCount[s] < 10) ? historyCount[s] : 10;

  float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
  float y_vals[10];

  for (int i = 0; i < N; i++) {
    int idx = (historyIndex[s] - N + i + HISTORY_SIZE) % HISTORY_SIZE;
    float y = centroidHistory[s][idx];
    y_vals[i] = y;
    float x = (float)i;

    sumX += x;
    sumY += y;
    sumXY += x * y;
    sumX2 += x * x;
  }

  float denom = N * sumX2 - sumX * sumX;
  if (abs(denom) < 1e-9f)
    return 0.0f;

  float slope = (N * sumXY - sumX * sumY) / denom;
  float intercept = (sumY - slope * sumX) / N;

  float ss_res = 0;
  float ss_tot = 0;
  float meanY = sumY / N;

  for (int i = 0; i < N; i++) {
    float pred = slope * i + intercept;
    float y = y_vals[i];
    ss_res += (y - pred) * (y - pred);
    ss_tot += (y - meanY) * (y - meanY);
  }

  if (ss_tot < 1e-9f)
    return 1.0f;
  return 1.0f - (ss_res / ss_tot);
}

// E. Spatial Kurtosis
float getSpatialKurtosis(const uint8_t *gates) {
  float vals[NUM_GATES];
  float mean = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    vals[i] = (float)gates[i];
    mean += vals[i];
  }
  mean /= NUM_GATES;

  float var = 0;
  float m4 = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    float diff = vals[i] - mean;
    var += diff * diff;
    m4 += diff * diff * diff * diff;
  }
  var /= NUM_GATES;
  m4 /= NUM_GATES;

  if (var < 1e-9f)
    return -3.0f;
  return (m4 / (var * var)) - 3.0f;
}

// F. Spatial Skewness
float getSpatialSkewness(const uint8_t *gates) {
  float vals[NUM_GATES];
  float mean = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    vals[i] = (float)gates[i];
    mean += vals[i];
  }
  mean /= NUM_GATES;

  float var = 0;
  float m3 = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    float diff = vals[i] - mean;
    var += diff * diff;
    m3 += diff * diff * diff;
  }
  var /= NUM_GATES;
  m3 /= NUM_GATES;

  float stdDev = sqrt(var);
  if (stdDev < 1e-9f)
    return 0.0f;
  return m3 / (stdDev * stdDev * stdDev);
}

// G. Hjorth Mobility
float getHjorthMobility(int s) {
  if (historyCount[s] < 3)
    return 0.0f;
  int N = (historyCount[s] < 20) ? historyCount[s] : 20;

  float signal[20];
  float diff1[19];
  float meanSig = 0;

  for (int i = 0; i < N; i++) {
    int idx = (historyIndex[s] - N + i + HISTORY_SIZE) % HISTORY_SIZE;
    signal[i] = totalEnergyHistory[s][idx];
    meanSig += signal[i];
  }
  meanSig /= N;

  float varSig = 0;
  for (int i = 0; i < N; i++) {
    float d = signal[i] - meanSig;
    varSig += d * d;
  }
  varSig /= N;

  float meanDiff = 0;
  for (int i = 0; i < N - 1; i++) {
    diff1[i] = signal[i + 1] - signal[i];
    meanDiff += diff1[i];
  }
  meanDiff /= (N - 1);

  float varDiff = 0;
  for (int i = 0; i < N - 1; i++) {
    float d = diff1[i] - meanDiff;
    varDiff += d * d;
  }
  varDiff /= (N - 1);

  if (varSig < 1e-9f)
    return 0.0f;
  return sqrt(varDiff / varSig);
}

// H. Hjorth Complexity
float getHjorthComplexity(int s) {
  if (historyCount[s] < 4)
    return 0.0f;
  int N = (historyCount[s] < 20) ? historyCount[s] : 20;

  float signal[20];
  float diff1[19];
  float diff2[18];

  float meanSig = 0;
  for (int i = 0; i < N; i++) {
    int idx = (historyIndex[s] - N + i + HISTORY_SIZE) % HISTORY_SIZE;
    signal[i] = totalEnergyHistory[s][idx];
    meanSig += signal[i];
  }
  meanSig /= N;
  float varSig = 0;
  for (int i = 0; i < N; i++)
    varSig += pow(signal[i] - meanSig, 2);
  varSig /= N;

  float meanD1 = 0;
  for (int i = 0; i < N - 1; i++) {
    diff1[i] = signal[i + 1] - signal[i];
    meanD1 += diff1[i];
  }
  meanD1 /= (N - 1);
  float varD1 = 0;
  for (int i = 0; i < N - 1; i++)
    varD1 += pow(diff1[i] - meanD1, 2);
  varD1 /= (N - 1);

  float meanD2 = 0;
  for (int i = 0; i < N - 2; i++) {
    diff2[i] = diff1[i + 1] - diff1[i];
    meanD2 += diff2[i];
  }
  meanD2 /= (N - 2);
  float varD2 = 0;
  for (int i = 0; i < N - 2; i++)
    varD2 += pow(diff2[i] - meanD2, 2);
  varD2 /= (N - 2);

  if (varSig < 1e-9f || varD1 < 1e-9f)
    return 0.0f;

  float mobSig = sqrt(varD1 / varSig);
  float mobD1 = sqrt(varD2 / varD1);

  return mobD1 / (mobSig + 1e-9f);
}

// I. Crest Factor
float getCrestFactor(const uint8_t *gates) {
  float maxE = 0;
  float sumE = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    float v = (float)gates[i];
    if (v > maxE)
      maxE = v;
    sumE += v;
  }
  float meanE = sumE / NUM_GATES;
  return maxE / (meanE + 1e-9f);
}

// J. Spectral Entropy
float getSpectralEntropy(const uint8_t *gates) {
  float sumE = 0;
  for (int i = 0; i < NUM_GATES; i++)
    sumE += (float)gates[i];
  if (sumE < 1e-9f)
    return 0.0f;

  float entropy = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    float p = (float)gates[i] / sumE;
    if (p > 0) {
      entropy -= p * (log(p) / log(2.0f));
    }
  }
  return entropy;
}

// K. Flicker Index
float getFlickerIndex(int s) {
  if (historyCount[s] < 3)
    return 0.0f;
  int N = (historyCount[s] < 20) ? historyCount[s] : 20;

  float vals[20];
  float mean = 0;
  for (int i = 0; i < N; i++) {
    int idx = (historyIndex[s] - N + i + HISTORY_SIZE) % HISTORY_SIZE;
    vals[i] = totalEnergyHistory[s][idx];
    mean += vals[i];
  }
  mean /= N;

  int crossings = 0;
  for (int i = 0; i < N - 1; i++) {
    float c1 = vals[i] - mean;
    float c2 = vals[i + 1] - mean;
    if (c1 * c2 < 0)
      crossings++;
  }
  return (float)crossings;
}

// L. Flux
float getFlux(int s, const uint8_t *gates) {
  if (historyCount[s] < 1)
    return 0.0f;
  int prevIdx = (historyIndex[s] - 2 + HISTORY_SIZE) % HISTORY_SIZE;
  float f = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    float curr = (float)gates[i];
    float prev = (float)movingHistory[s][prevIdx][i];
    f += abs(curr - prev);
  }
  return f;
}

void outputNewFeatures(int s, BaselineManager &bl) {
  uint8_t normMov[NUM_GATES];
  for (int i = 0; i < NUM_GATES; i++) {
    normMov[i] = (uint8_t)constrain(
        normalizeMovingValue(bl, latestMovingGates[s][i], i), 0, 255);
  }

  int maxIdx;
  float stability = getWeightedPeakStability(s, normMov, maxIdx);
  prevPeakIdx[s] = maxIdx;

  float smoothness = getSpatialSmoothness(normMov);
  float velocity = getCentroidVelocity(s);
  float linearity = getMovementLinearity(s);
  float kurtosis = getSpatialKurtosis(normMov);
  float skewness = getSpatialSkewness(normMov);
  float mobility = getHjorthMobility(s);
  float complexity = getHjorthComplexity(s);
  float crest = getCrestFactor(normMov);
  float entropy = getSpectralEntropy(normMov);
  float flicker = getFlickerIndex(s);
  float fluxVal = getFlux(s, normMov);

  Serial.print(",");
  Serial.print(stability, 2);
  Serial.print(",");
  Serial.print(smoothness, 2);
  Serial.print(",");
  Serial.print(velocity, 2);
  Serial.print(",");
  Serial.print(linearity, 2);
  Serial.print(",");
  Serial.print(kurtosis, 2);
  Serial.print(",");
  Serial.print(skewness, 2);
  Serial.print(",");
  Serial.print(mobility, 2);
  Serial.print(",");
  Serial.print(complexity, 2);
  Serial.print(",");
  Serial.print(crest, 2);
  Serial.print(",");
  Serial.print(entropy, 2);
  Serial.print(",");
  Serial.print(flicker, 2);
  Serial.print(",");
  Serial.print(fluxVal, 2);
}
