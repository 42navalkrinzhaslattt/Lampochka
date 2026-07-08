/*
  1D Convolutional Neural Network Model for Arduino
  
  Memory-optimized 1D-CNN implementation for pattern scanning
  Uses pre-computed convolutional filters and weights for classification.
  
  Features:
  - 1D convolutions for pattern detection
  - Max pooling for feature extraction
  - Dense layer for classification
  - Memory-conscious design for Arduino
*/

#ifndef CNN_1D_MODEL_H
#define CNN_1D_MODEL_H

// Model architecture
const int INPUT_SIZE = 46;            // Number of input features  
const int NUM_CONV_FILTERS = 8;       // Number of convolutional filters
const int FILTER_SIZE = 5;            // Size of each convolutional filter
const int CONV_OUTPUT_SIZE = INPUT_SIZE - FILTER_SIZE + 1;  // 42 after convolution (46-5+1)
const int POOLED_SIZE = CONV_OUTPUT_SIZE / 2;              // 21 after max pooling
const int HIDDEN_SIZE = 16;           // Size of hidden dense layer
const int OUTPUT_SIZE = 3;            // Number of output classes (nothing, human, vehicle)

// Class names
const char* class_names[] = {"nothing", "human", "vehicle"};

// Convolutional filters: [NUM_CONV_FILTERS][FILTER_SIZE]
float conv_filters[NUM_CONV_FILTERS][FILTER_SIZE] = {};  // Will be filled by training script

// Convolutional biases for each filter
float conv_biases[NUM_CONV_FILTERS] = {};  // Will be filled by training script

// Dense layer weights from pooled output to hidden layer
// Shape: [HIDDEN_SIZE][NUM_CONV_FILTERS * POOLED_SIZE]
float w_dense_hidden[HIDDEN_SIZE][NUM_CONV_FILTERS * POOLED_SIZE] = {};  // Will be filled by training script

// Dense layer biases
float b_hidden[HIDDEN_SIZE] = {};  // Will be filled by training script

// Weights from hidden to output layer
// Shape: [OUTPUT_SIZE][HIDDEN_SIZE]
float w_hidden_output[OUTPUT_SIZE][HIDDEN_SIZE] = {};  // Will be filled by training script

// Output biases
float b_output[OUTPUT_SIZE] = {};  // Will be filled by training script

// Activation function implementations
float relu(float x) {
  return (x > 0.0f) ? x : 0.0f;
}

// 1D convolution operation
void conv1d(float* input, int input_len, float* filter, int filter_len, float* output, int output_len, float bias) {
  for (int i = 0; i < output_len; i++) {
    float sum = bias;
    for (int j = 0; j < filter_len; j++) {
      sum += input[i + j] * filter[j];
    }
    output[i] = relu(sum);  // Apply ReLU activation
  }
}

// 1D max pooling operation
void max_pool1d(float* input, int input_len, float* output, int output_len) {
  int pool_size = 2;  // Simple pooling of 2 elements
  for (int i = 0; i < output_len; i++) {
    float max_val = input[i * pool_size];
    for (int j = 1; j < pool_size && (i * pool_size + j) < input_len; j++) {
      if (input[i * pool_size + j] > max_val) {
        max_val = input[i * pool_size + j];
      }
    }
    output[i] = max_val;
  }
}

// Softmax function for output layer
void softmax(float* input, int size, float* output) {
  // Find max to prevent overflow
  float max_val = input[0];
  for (int i = 1; i < size; i++) {
    if (input[i] > max_val) max_val = input[i];
  }
  
  // Calculate exponentials and sum
  float sum = 0.0f;
  for (int i = 0; i < size; i++) {
    float exp_val = exp(input[i] - max_val);
    output[i] = exp_val;
    sum += exp_val;
  }
  
  // Normalize
  for (int i = 0; i < size; i++) {
    output[i] /= sum;
  }
}

// Evaluate 1D CNN
String evaluateCNN(float features[]) {
  // Apply each convolutional filter
  float conv_outputs[NUM_CONV_FILTERS][CONV_OUTPUT_SIZE];
  for (int f = 0; f < NUM_CONV_FILTERS; f++) {
    conv1d(features, INPUT_SIZE, conv_filters[f], FILTER_SIZE, conv_outputs[f], CONV_OUTPUT_SIZE, conv_biases[f]);
  }
  
  // Max pooling for each feature map
  float pooled_outputs[NUM_CONV_FILTERS][POOLED_SIZE];
  for (int f = 0; f < NUM_CONV_FILTERS; f++) {
    max_pool1d(conv_outputs[f], CONV_OUTPUT_SIZE, pooled_outputs[f], POOLED_SIZE);
  }
  
  // Flatten pooled outputs for dense layer
  float flattened[NUM_CONV_FILTERS * POOLED_SIZE];
  int idx = 0;
  for (int f = 0; f < NUM_CONV_FILTERS; f++) {
    for (int p = 0; p < POOLED_SIZE; p++) {
      flattened[idx++] = pooled_outputs[f][p];
    }
  }
  
  // Dense hidden layer
  float hidden[HIDDEN_SIZE];
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    float sum = b_hidden[i];
    for (int j = 0; j < NUM_CONV_FILTERS * POOLED_SIZE; j++) {
      sum += flattened[j] * w_dense_hidden[i][j];
    }
    hidden[i] = relu(sum);
  }
  
  // Output layer
  float output[OUTPUT_SIZE];
  for (int i = 0; i < OUTPUT_SIZE; i++) {
    float sum = b_output[i];
    for (int j = 0; j < HIDDEN_SIZE; j++) {
      sum += hidden[j] * w_hidden_output[i][j];
    }
    output[i] = sum;
  }
  
  // Apply softmax to get probabilities
  float probabilities[OUTPUT_SIZE];
  softmax(output, OUTPUT_SIZE, probabilities);
  
  // Find the class with highest probability
  int predicted_class = 0;
  float max_prob = probabilities[0];
  for (int i = 1; i < OUTPUT_SIZE; i++) {
    if (probabilities[i] > max_prob) {
      max_prob = probabilities[i];
      predicted_class = i;
    }
  }
  
  // Return class name
  if (predicted_class >= 0 && predicted_class < OUTPUT_SIZE) {
    return String(class_names[predicted_class]);
  }
  
  return "unknown";
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