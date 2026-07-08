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
// Baseline parameters
const int BASELINE_SAMPLES = 10;             // 10 frames (2s at 5Hz)
const int BASELINE_WARMUP_MS = 5000;         // 5 seconds sensor warmup
const unsigned long BASELINE_INTERVAL = 200; // 5 Hz sampling

// Timing variables
unsigned long lastSampleTime = 0;

// Per-sensor state
uint8_t latestMovingGates[NUM_SENSORS][NUM_GATES] = {{0}};
uint8_t latestStationaryGates[NUM_SENSORS][NUM_GATES] = {{0}};

// Per-sensor active flag (false if comm test or baseline failed)
bool sensorOk[NUM_SENSORS] = {true, true, true, true};

#endif
