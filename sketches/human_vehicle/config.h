/*
  Configuration and Global Variables for SVM Detector (LD2412)
  Mirrors the recorder's config to ensure feature parity.
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Baseline mode toggle: 1 = use library BaselineManager, 0 = use manual
#define USE_BASELINE_LIBRARY 1

// Pin definitions
const int LED_PIN = LED_BUILTIN;
#define sensorSerial Serial1

// Sampling parameters
const int NUM_GATES = 14;
const unsigned long SAMPLE_INTERVAL = 200; // 5 Hz
const int HISTORY_SIZE =
    100; // Track last 100 samples (20s at 5Hz) for temporal features
const int ENERGY_THRESHOLD = 10; // Threshold for active zone

// Baseline parameters
const int BASELINE_SAMPLES = 10;             // 10 frames (2s at 5Hz)
const int BASELINE_WARMUP_MS = 5000;         // 5 seconds sensor warmup
const unsigned long BASELINE_INTERVAL = 200; // 5 Hz sampling

// Frame buffer
uint8_t dataBuf[256];
int dataBufLen = 0;

// Timing variables
unsigned long lastSampleTime = 0;

// Latest sensor readings
uint8_t latestMovingGates[NUM_GATES] = {0};
uint8_t latestStationaryGates[NUM_GATES] = {0};

// History buffers for temporal features
uint8_t movingHistory[HISTORY_SIZE][NUM_GATES];
uint8_t stationaryHistory[HISTORY_SIZE][NUM_GATES];
// New feature history buffers (for Hjorth, linearity, etc.)
float totalEnergyHistory[HISTORY_SIZE];
float centroidHistory[HISTORY_SIZE];
int prevPeakIdx = -1;

int historyIndex = 0;
int historyCount = 0;

// Detection tracking (for motionDuration semantics)
unsigned long firstDetectionTime = 0;
bool isDetecting = false;

#endif
