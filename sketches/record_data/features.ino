/*
  Feature Calculations

  Functions for calculating derived features from raw sensor data.
  Includes spatial, temporal, and pattern features for ML training.
*/

const float ACTIVE_ZONE_THRESHOLD = 1.0f;

void updateHistory(BaselineManager &bl) {
  // Store current NORMALIZED readings in circular buffer
  for (int i = 0; i < NUM_GATES; i++) {
    int normMove = normalizeMovingValue(bl, latestMovingGates[i], i);
    int normStat = normalizeValue(bl, latestStationaryGates[i], i);

    // Clamp to uint8_t range (0-255) for storage
    if (normMove > 255)
      normMove = 255;
    if (normStat > 255)
      normStat = 255;

    movingHistory[historyIndex][i] = (uint8_t)normMove;
    stationaryHistory[historyIndex][i] = (uint8_t)normStat;
  }

  // Calculate and store metrics for new features (Energy & Centroid)
  float frameEnergy = 0;
  float frameCentroid = 0;
  float weightSum = 0;

  for (int i = 0; i < NUM_GATES; i++) {
    float val = (float)movingHistory[historyIndex][i];
    frameEnergy += val;
    frameCentroid += i * val;
    weightSum += val;
  }
  if (weightSum > 0)
    frameCentroid /= weightSum;

  totalEnergyHistory[historyIndex] = frameEnergy;
  centroidHistory[historyIndex] = frameCentroid;

  historyIndex = (historyIndex + 1) % HISTORY_SIZE;
  if (historyCount < HISTORY_SIZE)
    historyCount++;

  // Update detection tracking using NORMALIZED values
  bool currentlyDetecting = false;
  for (int i = 0; i < NUM_GATES; i++) {
    float normMove = normalizeMovingValue(bl, latestMovingGates[i], i);
    float normStat = normalizeValue(bl, latestStationaryGates[i], i);

    if (normMove > ACTIVE_ZONE_THRESHOLD || normStat > ACTIVE_ZONE_THRESHOLD) {
      currentlyDetecting = true;
      break;
    }
  }

  if (currentlyDetecting && !isDetecting) {
    firstDetectionTime = millis();
    isDetecting = true;
  } else if (!currentlyDetecting) {
    isDetecting = false;
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

void outputSpatialFeatures(BaselineManager &bl) {
  // Moving spatial features
  float maxMoveEnergy = 0;
  float totalMoveEnergy = 0;
  int activeMoveZones = 0;
  float moveCentroid = 0;
  float weightSum = 0;

  for (int i = 0; i < NUM_GATES; i++) {
    // Use normalized value for MOVING baseline
    float energy = normalizeMovingValue(bl, latestMovingGates[i], i);
    if (energy < 0)
      energy = 0; // Clamp

    if (energy > maxMoveEnergy)
      maxMoveEnergy = energy;
    totalMoveEnergy += energy;

    // Use normalized threshold
    if (energy > ACTIVE_ZONE_THRESHOLD)
      activeMoveZones++;

    // Centroid calculation (position weighted by energy)
    moveCentroid += i * energy;
    weightSum += energy;
  }
  if (weightSum > 0)
    moveCentroid /= weightSum;

  // Moving spread (variance from centroid)
  float moveSpread = 0;
  if (weightSum > 0) {
    for (int i = 0; i < NUM_GATES; i++) {
      float energy = normalizeMovingValue(bl, latestMovingGates[i], i);
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
    float energy = normalizeValue(bl, latestStationaryGates[i], i);
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
      float energy = normalizeValue(bl, latestStationaryGates[i], i);
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

void outputTemporalFeatures(BaselineManager &bl) {
  float moveChangeRate = 0;
  float statChangeRate = 0;
  int zoneSwitches = 0;
  float motionDuration = 0;
  float moveVariance = 0;
  float statVariance = 0;

  if (historyCount >= 2) {
    // Get previous sample index
    int prevIndex = (historyIndex - 2 + HISTORY_SIZE) % HISTORY_SIZE;

    // Calculate energy change rate (total energy difference)
    // NOTE: movingHistory is now NORMALIZED. We must normalize "curr" too.
    float prevMoveTotal = 0, currMoveTotal = 0;
    float prevStatTotal = 0, currStatTotal = 0;

    for (int i = 0; i < NUM_GATES; i++) {
      prevMoveTotal += movingHistory[prevIndex][i]; // Already normalized
      currMoveTotal += normalizeMovingValue(bl, latestMovingGates[i], i);

      prevStatTotal += stationaryHistory[prevIndex][i]; // Already normalized
      currStatTotal += normalizeValue(bl, latestStationaryGates[i], i);
    }

    moveChangeRate = currMoveTotal - prevMoveTotal;
    statChangeRate = currStatTotal - prevStatTotal;

    // Count zone switches (active zone changed between samples)
    int prevActiveZone = -1, currActiveZone = -1;
    float maxEnergy = 0;

    for (int i = 0; i < NUM_GATES; i++) {
      if (movingHistory[prevIndex][i] > maxEnergy) {
        maxEnergy = movingHistory[prevIndex][i];
        prevActiveZone = i;
      }
    }

    maxEnergy = 0;
    for (int i = 0; i < NUM_GATES; i++) {
      float normVal = normalizeMovingValue(bl, latestMovingGates[i], i);
      if (normVal > maxEnergy) {
        maxEnergy = normVal;
        currActiveZone = i;
      }
    }

    if (prevActiveZone != currActiveZone && currActiveZone >= 0) {
      zoneSwitches = 1;
    }
  }

  // Motion duration (seconds since first detection)
  if (isDetecting && firstDetectionTime > 0) {
    motionDuration = (millis() - firstDetectionTime) / 1000.0;
  }

  // Calculate variance over history (temporal stability of NORMALIZED signal)
  if (historyCount >= 5) {
    float moveMean = 0, statMean = 0;

    // Calculate mean
    for (int t = 0; t < historyCount; t++) {
      for (int i = 0; i < NUM_GATES; i++) {
        moveMean += movingHistory[t][i];
        statMean += stationaryHistory[t][i];
      }
    }
    moveMean /= (historyCount * NUM_GATES);
    statMean /= (historyCount * NUM_GATES);

    // Calculate variance
    for (int t = 0; t < historyCount; t++) {
      for (int i = 0; i < NUM_GATES; i++) {
        float moveDiff = movingHistory[t][i] - moveMean;
        float statDiff = stationaryHistory[t][i] - statMean;
        moveVariance += moveDiff * moveDiff;
        statVariance += statDiff * statDiff;
      }
    }
    moveVariance /= (historyCount * NUM_GATES);
    statVariance /= (historyCount * NUM_GATES);
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

void outputPatternFeatures(BaselineManager &bl) {
  // Check if zones activate sequentially (progressive detection)
  bool zonesSequential = false;
  int firstActive = -1, lastActive = -1;

  for (int i = 0; i < NUM_GATES; i++) {
    if (normalizeMovingValue(bl, latestMovingGates[i], i) >
        ACTIVE_ZONE_THRESHOLD) {
      if (firstActive == -1)
        firstActive = i;
      lastActive = i;
    }
  }

  if (firstActive >= 0 && lastActive > firstActive) {
    // Check if all zones between first and last are active (sequential pattern)
    zonesSequential = true;
    for (int i = firstActive; i <= lastActive; i++) {
      if (normalizeMovingValue(bl, latestMovingGates[i], i) <=
          ACTIVE_ZONE_THRESHOLD) {
        zonesSequential = false;
        break;
      }
    }
  }

  // Calculate correlation between adjacent zones
  float zoneCorrelation = 0;
  int correlationCount = 0;

  for (int i = 0; i < NUM_GATES - 1; i++) {
    float val1 = normalizeMovingValue(bl, latestMovingGates[i], i);
    float val2 = normalizeMovingValue(bl, latestMovingGates[i + 1], i + 1);

    // Simple correlation: if both zones active or both inactive
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
float getWeightedPeakStability(const uint8_t *gates, int &outMaxIdx) {
  float maxE = 0;
  int maxIdx = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    if (gates[i] > maxE) {
      maxE = gates[i];
      maxIdx = i;
    }
  }
  outMaxIdx = maxIdx;

  if (prevPeakIdx == -1)
    return 0.0f;

  // log(1 + E)
  float logE = log(1.0f + maxE);
  return abs(maxIdx - prevPeakIdx) * logE;
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
float getCentroidVelocity() {
  if (historyCount < 5)
    return 0.0f;
  float num = 0;
  for (int i = 0; i < 5; i++) {
    int idx = (historyIndex - 5 + i + HISTORY_SIZE) % HISTORY_SIZE;
    float c = centroidHistory[idx];
    float w = (float)(i - 2);
    num += w * c;
  }
  return num / 10.0f;
}

// D. Movement Linearity (R^2)
float getMovementLinearity() {
  if (historyCount < 5)
    return 0.0f;
  int N = (historyCount < 10) ? historyCount : 10;

  float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
  float y_vals[10];

  for (int i = 0; i < N; i++) {
    int idx = (historyIndex - N + i + HISTORY_SIZE) % HISTORY_SIZE;
    float y = centroidHistory[idx];
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
float getHjorthMobility() {
  if (historyCount < 3)
    return 0.0f;
  int N = (historyCount < 20) ? historyCount : 20;

  float signal[20];
  float diff1[19];
  float meanSig = 0;

  for (int i = 0; i < N; i++) {
    int idx = (historyIndex - N + i + HISTORY_SIZE) % HISTORY_SIZE;
    signal[i] = totalEnergyHistory[idx];
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
float getHjorthComplexity() {
  if (historyCount < 4)
    return 0.0f;
  int N = (historyCount < 20) ? historyCount : 20;

  float signal[20];
  float diff1[19];
  float diff2[18];

  float meanSig = 0;
  for (int i = 0; i < N; i++) {
    int idx = (historyIndex - N + i + HISTORY_SIZE) % HISTORY_SIZE;
    signal[i] = totalEnergyHistory[idx];
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
float getFlickerIndex() {
  if (historyCount < 3)
    return 0.0f;
  int N = (historyCount < 20) ? historyCount : 20;

  float vals[20];
  float mean = 0;
  for (int i = 0; i < N; i++) {
    int idx = (historyIndex - N + i + HISTORY_SIZE) % HISTORY_SIZE;
    vals[i] = totalEnergyHistory[idx];
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
float getFlux(const uint8_t *gates) {
  if (historyCount < 1)
    return 0.0f;
  int prevIdx = (historyIndex - 2 + HISTORY_SIZE) % HISTORY_SIZE;
  // Wait, current frame is NOT in history yet?
  // UpdateHistory is called BEFORE outputFeatures in main loop.
  // So current frame IS at 'historyIndex - 1'.
  // Previous frame is 'historyIndex - 2'.

  prevIdx = (historyIndex - 2 + HISTORY_SIZE) % HISTORY_SIZE;
  float f = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    float curr = (float)gates[i];
    float prev = (float)movingHistory[prevIdx][i];
    f += abs(curr - prev);
  }
  return f;
}

void outputNewFeatures(BaselineManager &bl) {
  uint8_t normMov[NUM_GATES];
  for (int i = 0; i < NUM_GATES; i++) {
    normMov[i] = (uint8_t)constrain(
        normalizeMovingValue(bl, latestMovingGates[i], i), 0, 255);
  }

  int maxIdx;
  float stability = getWeightedPeakStability(normMov, maxIdx);
  prevPeakIdx = maxIdx;

  float smoothness = getSpatialSmoothness(normMov);
  float velocity = getCentroidVelocity();
  float linearity = getMovementLinearity();
  float kurtosis = getSpatialKurtosis(normMov);
  float skewness = getSpatialSkewness(normMov);
  float mobility = getHjorthMobility();
  float complexity = getHjorthComplexity();
  float crest = getCrestFactor(normMov);
  float entropy = getSpectralEntropy(normMov);
  float flicker = getFlickerIndex();
  float fluxVal = getFlux(normMov);

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
