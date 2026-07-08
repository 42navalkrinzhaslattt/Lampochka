/*
  Naive Bayes Model for Arduino
  
  Memory-optimized Gaussian Naive Bayes implementation
  Uses pre-computed class means, variances, and priors for classification.
  
  Features:
  - Gaussian (Normal) distribution assumption for each feature
  - For multi-class: probability calculation for each class
  - Memory-conscious design for Arduino
*/

#ifndef NAIVE_BAYES_MODEL_H
#define NAIVE_BAYES_MODEL_H

// Model metadata
const int NUM_FEATURES = 46;              // Number of input features
const int NUM_CLASSES = 3;                // Number of classes (nothing, human, vehicle)

// Class names
const char* class_names[] = {"nothing", "human", "vehicle"};

// Class priors (log probabilities) - probability of each class in training data
float class_priors[NUM_CLASSES] = {0.0, 0.0, 0.0};  // Will be filled during training (log values)

// Feature means for each class (for Gaussian distribution)
// mean[class][feature_index]
float feature_means[NUM_CLASSES][NUM_FEATURES] = {
  // Class 0 ("nothing") feature means - will be filled by training script
  {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
  
  // Class 1 ("human") feature means - will be filled by training script
  {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
  
  // Class 2 ("vehicle") feature means - will be filled by training script
  {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
};

// Feature variances for each class (for Gaussian distribution)  
// variance[class][feature_index]
float feature_variances[NUM_CLASSES][NUM_FEATURES] = {
  // Class 0 ("nothing") feature variances - will be filled by training script
  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
  
  // Class 1 ("human") feature variances - will be filled by training script
  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
  
  // Class 2 ("vehicle") feature variances - will be filled by training script
  {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}
};

// Minimum variance to prevent division by zero
const float MIN_VARIANCE = 0.001f;

// Calculate probability density for a Gaussian (normal) distribution
// This is P(feature|class) = (1/sqrt(2*pi*var)) * exp(-0.5 * ((x-mean)^2)/var)
float gaussian_pdf(float x, float mean, float var) {
  if (var < MIN_VARIANCE) var = MIN_VARIANCE;
  
  float diff = x - mean;
  float exponent = -0.5f * (diff * diff) / var;
  // Using log probabilities to avoid underflow
  float log_prob = -0.5f * log(2.0f * M_PI * var) + exponent;
  return log_prob;
}

// Evaluate Naive Bayes for all classes and return the most likely class
String evaluateNaiveBayes(float features[]) {
  float class_scores[NUM_CLASSES];
  
  // For each class, calculate log P(class|features) = log P(class) + sum(log P(feature_i|class))
  for (int c = 0; c < NUM_CLASSES; c++) {
    // Start with log prior probability
    class_scores[c] = class_priors[c];
    
    // Add log probability for each feature
    for (int f = 0; f < NUM_FEATURES; f++) {
      float log_prob = gaussian_pdf(features[f], feature_means[c][f], feature_variances[c][f]);
      class_scores[c] += log_prob;
    }
  }
  
  // Find class with highest probability
  int predicted_class = 0;
  float max_score = class_scores[0];
  for (int c = 1; c < NUM_CLASSES; c++) {
    if (class_scores[c] > max_score) {
      max_score = class_scores[c];
      predicted_class = c;
    }
  }
  
  // Return class name
  if (predicted_class >= 0 && predicted_class < NUM_CLASSES) {
    return String(class_names[predicted_class]);
  }
  
  return "unknown";
}

// Compute log posterior scores for each class (prior + sum of log-likelihoods)
void computeNaiveBayesLogScores(float features[], float out_scores[]) {
  for (int c = 0; c < NUM_CLASSES; c++) {
    float s = class_priors[c];
    for (int f = 0; f < NUM_FEATURES; f++) {
      float log_prob = gaussian_pdf(features[f], feature_means[c][f], feature_variances[c][f]);
      s += log_prob;
    }
    out_scores[c] = s;
  }
}

// Helper functions for feature calculation (duplicated from shared copy)
float calculateMaxEnergy(uint8_t gates[]) {
  float maxVal = 0;
  for (int i = 0; i < NUM_GATES; i++) if (gates[i] > maxVal) maxVal = gates[i];
  return maxVal;
}

float calculateTotalEnergy(uint8_t gates[]) {
  float total = 0;
  for (int i = 0; i < NUM_GATES; i++) total += gates[i];
  return total;
}

float calculateActiveZones(uint8_t gates[]) {
  int count = 0;
  for (int i = 0; i < NUM_GATES; i++) if (gates[i] > ENERGY_THRESHOLD) count++;
  return count;
}

float calculateCentroid(uint8_t gates[]) {
  float weightedSum = 0, totalEnergy = 0;
  for (int i = 0; i < NUM_GATES; i++) { weightedSum += gates[i] * i; totalEnergy += gates[i]; }
  return (totalEnergy > 0) ? weightedSum / totalEnergy : 0;
}

float calculateSpread(uint8_t gates[]) {
  float centroid = calculateCentroid(gates);
  float variance = 0, totalEnergy = 0;
  for (int i = 0; i < NUM_GATES; i++) { variance += gates[i] * (i - centroid) * (i - centroid); totalEnergy += gates[i]; }
  return (totalEnergy > 0) ? sqrt(variance / totalEnergy) : 0;
}

float calculateChangeRate(uint8_t history[][NUM_GATES]) {
  if (HISTORY_SIZE < 2) return 0;
  float totalChange = 0;
  for (int i = 0; i < NUM_GATES; i++) totalChange += abs(history[0][i] - history[HISTORY_SIZE-1][i]);
  return totalChange / (HISTORY_SIZE - 1);
}

float calculateVariance(uint8_t history[][NUM_GATES]) {
  float variance = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    float mean = 0;
    for (int t = 0; t < HISTORY_SIZE; t++) mean += history[t][i];
    mean /= HISTORY_SIZE;
    float gateVar = 0;
    for (int t = 0; t < HISTORY_SIZE; t++) gateVar += (history[t][i] - mean) * (history[t][i] - mean);
    variance += gateVar / HISTORY_SIZE;
  }
  return variance / NUM_GATES;
}

float calculateZoneSwitches() {
  int switches = 0;
  for (int i = 0; i < NUM_GATES; i++) {
    bool wasActive = movingHistory[HISTORY_SIZE-1][i] > ENERGY_THRESHOLD;
    bool isActive = latestMovingGates[i] > ENERGY_THRESHOLD;
    if (wasActive != isActive) switches++;
  }
  return switches;
}

float calculateMotionDuration() {
  int duration = 0;
  for (int t = 0; t < HISTORY_SIZE; t++) {
    bool hasMotion = false;
    for (int i = 0; i < NUM_GATES; i++) { if (movingHistory[t][i] > ENERGY_THRESHOLD) { hasMotion = true; break; } }
    if (hasMotion) duration++;
  }
  return duration;
}

float calculateSequentialActivation() {
  int sequential = 0;
  for (int i = 0; i < NUM_GATES - 1; i++) if (latestMovingGates[i] > ENERGY_THRESHOLD && latestMovingGates[i+1] > ENERGY_THRESHOLD) sequential++;
  return sequential;
}

float calculateZoneCorrelation() {
  float correlation = 0;
  for (int i = 0; i < NUM_GATES; i++) if (latestMovingGates[i] > ENERGY_THRESHOLD && latestStationaryGates[i] > ENERGY_THRESHOLD) correlation++;
  return correlation;
}

#endif