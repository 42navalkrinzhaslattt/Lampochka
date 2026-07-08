/*
  Configuration and Global Variables for Multi-Sensor SVM Detector (LD2412)
  Raspberry Pi Pico 2W — 4 sensors
  
  Pin mapping:
    Sensor 0: Serial1  (GP0 TX, GP1 RX)   — UART0
    Sensor 1: Serial2  (GP4 TX, GP5 RX)   — UART1
    Sensor 2: SerialPIO (GP10 TX, GP11 RX)
    Sensor 3: SerialPIO (GP14 TX, GP15 RX)
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <Adafruit_TinyUSB.h>
#include <Arduino.h>
#include <SerialPIO.h>

// Baseline mode toggle: 1 = use library BaselineManager, 0 = use manual
#define USE_BASELINE_LIBRARY 1

// Number of sensors
const int NUM_SENSORS = 4;

// Pin definitions
const int LED_PIN = LED_BUILTIN;

// PIO-based serial ports (declared here, defined in .ino)
extern SerialPIO sensorSerial2;
extern SerialPIO sensorSerial3;

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

// Timing variables
unsigned long lastSampleTime = 0;

// Per-sensor state
uint8_t latestMovingGates[NUM_SENSORS][NUM_GATES] = {{0}};
uint8_t latestStationaryGates[NUM_SENSORS][NUM_GATES] = {{0}};

// Per-sensor history buffers for temporal features
uint8_t movingHistory[NUM_SENSORS][HISTORY_SIZE][NUM_GATES];
uint8_t stationaryHistory[NUM_SENSORS][HISTORY_SIZE][NUM_GATES];

// Per-sensor feature history buffers (for Hjorth, linearity, etc.)
float totalEnergyHistory[NUM_SENSORS][HISTORY_SIZE];
float centroidHistory[NUM_SENSORS][HISTORY_SIZE];
int prevPeakIdx[NUM_SENSORS] = {-1, -1, -1, -1};

int historyIndex[NUM_SENSORS] = {0};
int historyCount[NUM_SENSORS] = {0};

// Per-sensor detection tracking (for motionDuration semantics)
unsigned long firstDetectionTime[NUM_SENSORS] = {0};
bool isDetecting[NUM_SENSORS] = {false};

// Per-sensor active flag (false if comm test or baseline failed)
bool sensorOk[NUM_SENSORS] = {true, true, true, true};

#endif
