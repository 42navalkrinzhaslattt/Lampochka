/*
  Feedforward Neural Network / MLP Model for Arduino
  
  Memory-optimized implementation with fixed-size architecture
  Uses pre-computed weights and biases for classification.
  
  Features:
  - 46 input features -> hidden layer -> 3 output classes
  - ReLU activation for hidden layer, softmax for output
  - Memory-conscious design for Arduino
*/

#ifndef NN_MODEL_H
#define NN_MODEL_H

// Model architecture
const int INPUT_SIZE = 46;            // Number of input features  
const int HIDDEN_SIZE = 16;           // Size of hidden layer (adjustable based on memory)
const int OUTPUT_SIZE = 3;            // Number of output classes (nothing, human, vehicle)

// Class names
const char* class_names[] = {"nothing", "human", "vehicle"};

// Weights and biases for input to hidden layer
// Shape: [HIDDEN_SIZE][INPUT_SIZE]
float w_input_hidden[HIDDEN_SIZE][INPUT_SIZE] = {};  // Will be filled by training script

// Biases for hidden layer
// Shape: [HIDDEN_SIZE]  
float b_hidden[HIDDEN_SIZE] = {};  // Will be filled by training script

// Weights and biases for hidden to output layer
// Shape: [OUTPUT_SIZE][HIDDEN_SIZE]
float w_hidden_output[OUTPUT_SIZE][HIDDEN_SIZE] = {};  // Will be filled by training script

// Biases for output layer
// Shape: [OUTPUT_SIZE]
float b_output[OUTPUT_SIZE] = {};  // Will be filled by training script

// Activation function implementations
float relu(float x) {
  return (x > 0.0f) ? x : 0.0f;
}

float sigmoid(float x) {
  // Use a simplified approximation to avoid expensive exp() calculation
  if (x < -10.0f) return 0.0f;
  if (x > 10.0f) return 1.0f;
  return 1.0f / (1.0f + exp(-x));
}

// Softmax function for output layer (simplified for memory efficiency)
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

// Feedforward through the neural network
String evaluateNN(float features[]) {
  // Hidden layer computation
  float hidden[HIDDEN_SIZE];
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    float sum = b_hidden[i];
    for (int j = 0; j < INPUT_SIZE; j++) {
      sum += features[j] * w_input_hidden[i][j];
    }
    hidden[i] = relu(sum);  // Apply ReLU activation
  }
  
  // Output layer computation
  float output[OUTPUT_SIZE];
  for (int i = 0; i < OUTPUT_SIZE; i++) {
    float sum = b_output[i];
    for (int j = 0; j < HIDDEN_SIZE; j++) {
      sum += hidden[j] * w_hidden_output[i][j];
    }
    output[i] = sum;  // No activation for output before softmax
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