/*
  Recurrent Neural Network / LSTM Model for Arduino
  
  Memory-optimized implementation with simplified LSTM operations
  Uses pre-computed weights and maintains internal state for sequence processing.
  
  Features:
  - Simplified LSTM for temporal pattern recognition
  - Stateful operation to maintain memory across samples
  - Memory-conscious design for Arduino
*/

#ifndef RNN_LSTM_MODEL_H
#define RNN_LSTM_MODEL_H

// Model architecture
const int INPUT_SIZE = 46;            // Number of input features  
const int HIDDEN_SIZE = 16;           // Size of LSTM hidden state
const int OUTPUT_SIZE = 3;            // Number of output classes (nothing, human, vehicle)

// Class names
const char* class_names[] = {"nothing", "human", "vehicle"};

// LSTM weights (combined for input, forget, and output gates)
// Weights for input gate: [HIDDEN_SIZE][INPUT_SIZE + HIDDEN_SIZE]
float w_input_gate[HIDDEN_SIZE][INPUT_SIZE + HIDDEN_SIZE] = {};  // Will be filled by training script

// Weights for forget gate: [HIDDEN_SIZE][INPUT_SIZE + HIDDEN_SIZE]
float w_forget_gate[HIDDEN_SIZE][INPUT_SIZE + HIDDEN_SIZE] = {};  // Will be filled by training script

// Weights for output gate: [HIDDEN_SIZE][INPUT_SIZE + HIDDEN_SIZE]
float w_output_gate[HIDDEN_SIZE][INPUT_SIZE + HIDDEN_SIZE] = {};  // Will be filled by training script

// Weights for cell state: [HIDDEN_SIZE][INPUT_SIZE + HIDDEN_SIZE]
float w_cell_state[HIDDEN_SIZE][INPUT_SIZE + HIDDEN_SIZE] = {};  // Will be filled by training script

// Input gate biases
float b_input_gate[HIDDEN_SIZE] = {};  // Will be filled by training script

// Forget gate biases
float b_forget_gate[HIDDEN_SIZE] = {};  // Will be filled by training script

// Output gate biases
float b_output_gate[HIDDEN_SIZE] = {};  // Will be filled by training script

// Cell state biases
float b_cell_state[HIDDEN_SIZE] = {};  // Will be filled by training script

// Output weights: [OUTPUT_SIZE][HIDDEN_SIZE]
float w_output[OUTPUT_SIZE][HIDDEN_SIZE] = {};  // Will be filled by training script

// Output biases
float b_output_bias[OUTPUT_SIZE] = {};  // Will be filled by training script

// LSTM internal state
float hidden_state[HIDDEN_SIZE] = {0.0};  // Current hidden state
float cell_state[HIDDEN_SIZE] = {0.0};    // Current cell state
bool lstm_initialized = false;             // Flag to indicate if LSTM has been initialized

// Activation function implementations
float sigmoid(float x) {
  // Use a simplified approximation to avoid expensive exp() calculation
  if (x < -10.0f) return 0.0f;
  if (x > 10.0f) return 1.0f;
  return 1.0f / (1.0f + exp(-x));
}

float tanh_approx(float x) {
  // Simple tanh approximation for computational efficiency
  if (x < -3.0f) return -1.0f;
  if (x > 3.0f) return 1.0f;
  // For -3 to 3, use a polynomial approximation
  float x2 = x * x;
  return x * (27.0f + x2) / (27.0f + 9.0f * x2);
}

float relu(float x) {
  return (x > 0.0f) ? x : 0.0f;
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

// LSTM step operation (simplified for Arduino)
void lstm_step(float* input, float* hidden, float* cell) {
  // Combine input and hidden state for gate calculations
  float combined[INPUT_SIZE + HIDDEN_SIZE];
  for (int i = 0; i < INPUT_SIZE; i++) {
    combined[i] = input[i];
  }
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    combined[INPUT_SIZE + i] = hidden[i];
  }
  
  // Calculate gate values
  float input_gate[HIDDEN_SIZE];
  float forget_gate[HIDDEN_SIZE];
  float output_gate[HIDDEN_SIZE];
  float candidate_cell[HIDDEN_SIZE];
  
  // Input gate
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    float sum = b_input_gate[i];
    for (int j = 0; j < INPUT_SIZE + HIDDEN_SIZE; j++) {
      sum += combined[j] * w_input_gate[i][j];
    }
    input_gate[i] = sigmoid(sum);
  }
  
  // Forget gate
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    float sum = b_forget_gate[i];
    for (int j = 0; j < INPUT_SIZE + HIDDEN_SIZE; j++) {
      sum += combined[j] * w_forget_gate[i][j];
    }
    forget_gate[i] = sigmoid(sum);
  }
  
  // Output gate
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    float sum = b_output_gate[i];
    for (int j = 0; j < INPUT_SIZE + HIDDEN_SIZE; j++) {
      sum += combined[j] * w_output_gate[i][j];
    }
    output_gate[i] = sigmoid(sum);
  }
  
  // Candidate cell state
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    float sum = b_cell_state[i];
    for (int j = 0; j < INPUT_SIZE + HIDDEN_SIZE; j++) {
      sum += combined[j] * w_cell_state[i][j];
    }
    candidate_cell[i] = tanh_approx(sum);
  }
  
  // Update cell state
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    cell[i] = forget_gate[i] * cell[i] + input_gate[i] * candidate_cell[i];
  }
  
  // Update hidden state
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    hidden[i] = output_gate[i] * tanh_approx(cell[i]);
  }
}

// Reset LSTM state
void reset_lstm_state() {
  for (int i = 0; i < HIDDEN_SIZE; i++) {
    hidden_state[i] = 0.0;
    cell_state[i] = 0.0;
  }
  lstm_initialized = true;
}

// Evaluate RNN/LSTM
String evaluateRNN(float features[]) {
  // Initialize states if not already done
  if (!lstm_initialized) {
    reset_lstm_state();
  }
  
  // Perform one LSTM step
  lstm_step(features, hidden_state, cell_state);
  
  // Calculate output from hidden state
  float output[OUTPUT_SIZE];
  for (int i = 0; i < OUTPUT_SIZE; i++) {
    float sum = b_output_bias[i];
    for (int j = 0; j < HIDDEN_SIZE; j++) {
      sum += hidden_state[j] * w_output[i][j];
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