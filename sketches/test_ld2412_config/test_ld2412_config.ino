/*
  LD2412 Raw Protocol Configuration
  
  This sends raw UART commands directly to configure the sensor
  without relying on the MyLD2410 library parsing.
*/

#define sensorSerial Serial1
const int LED_PIN = LED_BUILTIN;

// Protocol constants
const uint8_t CONFIG_HEADER[] = {0xFD, 0xFC, 0xFB, 0xFA};
const uint8_t CONFIG_TAIL[] = {0x04, 0x03, 0x02, 0x01};
const uint8_t DATA_HEADER[] = {0xF4, 0xF3, 0xF2, 0xF1};

uint8_t rxBuf[128];
int rxIndex = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000);
  
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println("========================================");
  Serial.println("LD2412 Raw Protocol Configuration");
  Serial.println("========================================");
  Serial.println();
  
  sensorSerial.begin(115200);
  delay(1000);
  
  // Step 1: Enable configuration mode
  Serial.println("Step 1: Enabling configuration mode...");
  uint8_t enableConfigCmd[] = {0x04, 0x00, 0xFF, 0x00, 0x01, 0x00};
  sendConfigCommand("Enable Config", enableConfigCmd, 6);
  delay(500);
  
  // Step 2: Set max detection gates to 13 (about 10m range - 14 gates x 0.75m)
  Serial.println("\nStep 2: Setting max detection range...");
  // Command 0x60: maxGate parameters
  // Format: [length_low, length_high, cmd_low, cmd_high, word1, word2, ...]
  uint8_t maxGateCmd[] = {
    0x14, 0x00,              // Length: 20 bytes
    0x60, 0x00,              // Command: 0x0060 (set max gate)
    0x00, 0x00,              // Word 1: reserved
    0x0D, 0x00,              // Word 2: max moving gate = 13 (0-13 = 14 gates)
    0x00, 0x00,              // Word 3: reserved  
    0x0D, 0x00,              // Word 4: max stationary gate = 13 (0-13 = 14 gates)
    0x00, 0x00,              // Word 5: reserved
    0x05, 0x00,              // Word 6: no-one timeout = 5 seconds
    0x00, 0x00,              // Word 7: reserved
    0x00, 0x00               // Word 8: reserved
  };
  sendConfigCommand("Set Max Gates", maxGateCmd, sizeof(maxGateCmd));
  delay(500);
  
  // Step 3: Set sensitivity for all gates at once (commands 0x0003 and 0x0004)
  Serial.println("\nStep 3: Setting gate sensitivities...");
  
  // Lower = more sensitive. Try very low values: 10-20
  uint8_t movingSensitivity = 15;    // Very sensitive to motion
  uint8_t stationarySensitivity = 20;  // Very sensitive to stationary
  
  // Motion sensitivity configuration (command 0x0003) - 14 bytes for 14 gates
  uint8_t motionSensCmd[] = {
    0x10, 0x00,              // Length: 16 bytes (14 bytes data + 2 for command)
    0x03, 0x00,              // Command: 0x0003 (motion sensitivity config)
    0x00,                    // Gate 0 (0-0.75m) - not used but still set
    movingSensitivity,       // Gate 1 (0.75-1.5m)
    movingSensitivity,       // Gate 2 (1.5-2.25m)
    movingSensitivity,       // Gate 3 (2.25-3m)
    movingSensitivity,       // Gate 4 (3-3.75m)
    movingSensitivity,       // Gate 5 (3.75-4.5m)
    movingSensitivity,       // Gate 6 (4.5-5.25m)
    movingSensitivity,       // Gate 7 (5.25-6m)
    movingSensitivity,       // Gate 8 (6-6.75m)
    movingSensitivity,       // Gate 9
    movingSensitivity,       // Gate 10
    movingSensitivity,       // Gate 11
    movingSensitivity,       // Gate 12
    movingSensitivity        // Gate 13
  };
  Serial.print("  Setting motion sensitivity to ");
  Serial.print(movingSensitivity);
  Serial.print(" for all gates... ");
  sendConfigCommand(NULL, motionSensCmd, sizeof(motionSensCmd));
  delay(500);
  
  // Stationary sensitivity configuration (command 0x0004) - 14 bytes for 14 gates
  uint8_t stationarySensCmd[] = {
    0x10, 0x00,              // Length: 16 bytes (14 bytes data + 2 for command)
    0x04, 0x00,              // Command: 0x0004 (stationary sensitivity config)
    0x00,                    // Gate 0
    stationarySensitivity,   // Gate 1
    stationarySensitivity,   // Gate 2
    stationarySensitivity,   // Gate 3
    stationarySensitivity,   // Gate 4
    stationarySensitivity,   // Gate 5
    stationarySensitivity,   // Gate 6
    stationarySensitivity,   // Gate 7
    stationarySensitivity,   // Gate 8
    stationarySensitivity,   // Gate 9
    stationarySensitivity,   // Gate 10
    stationarySensitivity,   // Gate 11
    stationarySensitivity,   // Gate 12
    stationarySensitivity    // Gate 13
  };
  Serial.print("  Setting stationary sensitivity to ");
  Serial.print(stationarySensitivity);
  Serial.print(" for all gates... ");
  sendConfigCommand(NULL, stationarySensCmd, sizeof(stationarySensCmd));
  delay(500);
  
  Serial.println("  ✓ Sensitivity configuration sent");
  
  // Step 3b: Verify sensitivity settings by reading them back
  Serial.println("\nStep 3b: Verifying sensitivity settings...");
  Serial.println("Re-entering config mode...");
  uint8_t enableConfigCmd2[] = {0x04, 0x00, 0xFF, 0x00, 0x01, 0x00};
  sendConfigCommand("Enable Config", enableConfigCmd2, 6);
  delay(500);
  
  // Query motion sensitivity (command 0x0013)
  Serial.println("Querying motion sensitivity...");
  uint8_t queryMotionCmd[] = {0x02, 0x00, 0x13, 0x00};
  // sendConfigCommandWithResponse("Query Motion Sensitivity", queryMotionCmd, 4);
  delay(500);
  
  // Query stationary sensitivity (command 0x0014)
  Serial.println("Querying stationary sensitivity...");
  uint8_t queryStationaryCmd[] = {0x02, 0x00, 0x14, 0x00};
  // sendConfigCommandWithResponse("Query Stationary Sensitivity", queryStationaryCmd, 4);
  delay(500);
  
  // Step 4: Enable engineering (enhanced) mode for gate data
  Serial.println("\nStep 4: Enabling engineering mode...");
  uint8_t enableEngineeringCmd[] = {0x02, 0x00, 0x62, 0x00};
  sendConfigCommand("Enable Engineering", enableEngineeringCmd, 4);
  delay(500);
  
  // Step 5: Exit configuration mode (saves settings)
  Serial.println("\nStep 5: Exiting configuration mode (saving settings)...");
  uint8_t exitConfigCmd[] = {0x02, 0x00, 0xFE, 0x00};
  sendConfigCommand("Exit Config", exitConfigCmd, 4);
  delay(1000);
  
  Serial.println("\n========================================");
  Serial.println("Configuration complete!");
  Serial.println("========================================");
  Serial.println();
  Serial.println("Testing data output (30 seconds)...");
  Serial.println("Sensor should now output data continuously!");
  Serial.println();
}

unsigned long testStart = 0;
int dataFrames = 0;
int configFrames = 0;
uint8_t dataBuf[256];
int dataBufLen = 0;

void loop() {
  if (testStart == 0) {
    testStart = millis();
  }
  
  // Read any incoming data
  while (sensorSerial.available()) {
    uint8_t b = sensorSerial.read();
    
    // Check for data frame header (F4 F3 F2 F1)
    static uint8_t headerBuf[4] = {0};
    headerBuf[0] = headerBuf[1];
    headerBuf[1] = headerBuf[2];
    headerBuf[2] = headerBuf[3];
    headerBuf[3] = b;
    
    if (headerBuf[0] == 0xF4 && headerBuf[1] == 0xF3 && 
        headerBuf[2] == 0xF2 && headerBuf[3] == 0xF1) {
      dataFrames++;
      digitalWrite(LED_PIN, HIGH);
      
      // Read the rest of the frame
      dataBufLen = 0;
      unsigned long timeout = millis() + 100;
      while (millis() < timeout && dataBufLen < 256) {
        if (sensorSerial.available()) {
          dataBuf[dataBufLen++] = sensorSerial.read();
          timeout = millis() + 50; // Reset timeout on each byte
        }
      }
      
      // Parse and display every 5th frame with more detail
      if (dataFrames % 5 == 6) {
        Serial.println();
        Serial.print("[");
        Serial.print(dataFrames);
        Serial.print("] Len:");
        Serial.print(dataBufLen);
        Serial.print(" | ");
        
        if (dataBufLen >= 11) {
          // Official LD2412 data frame format (Table 11):
          // [0-1]: length (little endian)
          // [2]: data type (0x01=engineering, 0x02=basic)
          // [3]: head (0xAA)
          // [4]: target state (0=none, 1=moving, 2=stationary, 3=both)
          // [5-6]: moving target distance (cm, little endian)
          // [7]: moving target energy
          // [8-9]: stationary target distance (cm, little endian)
          // [10]: stationary target energy
          // [11]: end (0x55)
          // [12]: check (0x00)
          // Engineering mode adds: [13-14]: max gates, then gate energies
          
          uint8_t dataType = dataBuf[2];
          uint8_t head = dataBuf[3];
          uint8_t targetState = dataBuf[4];
          uint16_t movingDist = dataBuf[5] | (dataBuf[6] << 8);
          uint8_t movingEnergy = dataBuf[7];
          uint16_t stationaryDist = dataBuf[8] | (dataBuf[9] << 8);
          uint8_t stationaryEnergy = dataBuf[10];
          
          Serial.print("Type:");
          Serial.print(dataType == 0x01 ? "Eng" : "Basic");
          Serial.print(" | State:");
          Serial.print(targetState);
          Serial.print(" | Mov:");
          Serial.print(movingEnergy);
          Serial.print("@");
          Serial.print(movingDist);
          Serial.print("cm | Sta:");
          Serial.print(stationaryEnergy);
          Serial.print("@");
          Serial.print(stationaryDist);
          Serial.print("cm");
          
          // Show ALL gate energies if in engineering mode (type 0x01)
          if (dataType == 0x01 && dataBufLen >= 15) {
            // Engineering mode format (Table 13):
            // [11]: end marker (0x55)
            // [12]: check (0x00)  
            // [13]: max moving distance gate number
            // [14]: max stationary distance gate number
            // [15...]: moving gate 0 to N energy values
            // [...]: stationary gate 0 to N energy values
            
            // We configured 14 gates (0-13 = 14 gates total, 10.5m range)
            const int CONFIGURED_GATES = 14;
            
            if (dataBufLen >= 15 + CONFIGURED_GATES * 2) {
              uint8_t maxMovGate = dataBuf[13];
              uint8_t maxStaGate = dataBuf[14];
              
              Serial.println();
              Serial.print("    MaxGates: Mov=");
              Serial.print(maxMovGate);
              Serial.print(" Sta=");
              Serial.print(maxStaGate);
              
              // Moving gate energies (gates 0-13)
              Serial.println();
              Serial.print("    MovGates[0-13]: ");
              for (int i = 0; i < CONFIGURED_GATES; i++) {
                Serial.print(dataBuf[15 + i]);
                Serial.print(" ");
              }
              
              // Stationary gate energies (gates 0-13)
              Serial.println();
              Serial.print("    StaGates[0-13]: ");
              int staStart = 15 + CONFIGURED_GATES;
              for (int i = 0; i < CONFIGURED_GATES; i++) {
                Serial.print(dataBuf[staStart + i]);
                Serial.print(" ");
              }
            }
          }
        } else {
          Serial.print("Frame too short!");
        }
      } else {
        Serial.print(".");
      }
    }
    
    // Check for config frame header (FD FC FB FA)
    if (headerBuf[0] == 0xFD && headerBuf[1] == 0xFC && 
        headerBuf[2] == 0xFB && headerBuf[3] == 0xFA) {
      configFrames++;
    }
  }
  
  digitalWrite(LED_PIN, LOW);
  
  // Report after 30 seconds
  if (millis() - testStart > 30000) {
    Serial.println();
    Serial.println();
    Serial.println("========================================");
    Serial.println("Test Results:");
    Serial.print("  Data frames received: ");
    Serial.println(dataFrames);
    Serial.print("  Config responses: ");
    Serial.println(configFrames);
    Serial.println();
    
    if (dataFrames > 100) {
      Serial.println("✓✓✓ SUCCESS! ✓✓✓");
      Serial.println("Sensor is configured and working autonomously!");
      Serial.println();
      Serial.println("You can now:");
      Serial.println("1. Power cycle the sensor");
      Serial.println("2. Upload the data logging sketch");
      Serial.println("3. Collect training data");
    } else if (dataFrames > 0) {
      Serial.println("⚠ Partial success");
      Serial.println("Sensor is sending some data, but rate is low.");
      Serial.println("Try power cycling the sensor.");
    } else {
      Serial.println("✗ Configuration may have failed");
      Serial.println("Sensor is not outputting data frames.");
      Serial.println();
      Serial.println("Possible issues:");
      Serial.println("1. Sensor requires Bluetooth app configuration first");
      Serial.println("2. Different protocol variant");
      Serial.println("3. Sensor needs factory reset");
    }
    Serial.println("========================================");
    
    // Continue monitoring
    while (true) {
      delay(1000);
    }
  }
  
  delay(10);
}

void sendConfigCommand(const char* label, uint8_t* command, int len) {
  // Clear receive buffer
  while (sensorSerial.available()) {
    sensorSerial.read();
  }
  
  // Send: HEADER + COMMAND + TAIL
  sensorSerial.write(CONFIG_HEADER, 4);
  sensorSerial.write(command, len);
  sensorSerial.write(CONFIG_TAIL, 4);
  sensorSerial.flush();
  
  if (label) {
    Serial.print("  ");
    Serial.print(label);
    Serial.print("... ");
  }
  
  // Wait for ACK response
  unsigned long timeout = millis() + 2000;
  bool gotAck = false;
  uint8_t ackBuf[4] = {0};
  
  while (millis() < timeout) {
    if (sensorSerial.available()) {
      uint8_t b = sensorSerial.read();
      
      ackBuf[0] = ackBuf[1];
      ackBuf[1] = ackBuf[2];
      ackBuf[2] = ackBuf[3];
      ackBuf[3] = b;
      
      // Check for config response header (FD FC FB FA)
      if (ackBuf[0] == 0xFD && ackBuf[1] == 0xFC && 
          ackBuf[2] == 0xFB && ackBuf[3] == 0xFA) {
        gotAck = true;
        
        // Read the rest of the response
        delay(50);
        while (sensorSerial.available()) {
          sensorSerial.read();  // Drain the buffer
        }
        break;
      }
    }
    delay(10);
  }
  
  if (label) {
    if (gotAck) {
      Serial.println("✓ ACK");
    } else {
      Serial.println("⚠ No ACK (may still work)");
    }
  }
}

void sendConfigCommandWithResponse(const char* label, uint8_t* command, int len) {
  // Clear receive buffer
  while (sensorSerial.available()) {
    sensorSerial.read();
  }
  
  // Send: HEADER + COMMAND + TAIL
  sensorSerial.write(CONFIG_HEADER, 4);
  sensorSerial.write(command, len);
  sensorSerial.write(CONFIG_TAIL, 4);
  sensorSerial.flush();
  
  if (label) {
    Serial.print("  ");
    Serial.print(label);
    Serial.print("... ");
  }
  
  // Wait for response and parse it
  unsigned long timeout = millis() + 2000;
  uint8_t responseBuf[64];
  int responseLen = 0;
  bool gotResponse = false;
  uint8_t ackBuf[4] = {0};
  
  while (millis() < timeout) {
    if (sensorSerial.available()) {
      uint8_t b = sensorSerial.read();
      
      ackBuf[0] = ackBuf[1];
      ackBuf[1] = ackBuf[2];
      ackBuf[2] = ackBuf[3];
      ackBuf[3] = b;
      
      // Check for config response header (FD FC FB FA)
      if (ackBuf[0] == 0xFD && ackBuf[1] == 0xFC && 
          ackBuf[2] == 0xFB && ackBuf[3] == 0xFA) {
        gotResponse = true;
        responseBuf[0] = 0xFD;
        responseBuf[1] = 0xFC;
        responseBuf[2] = 0xFB;
        responseBuf[3] = 0xFA;
        responseLen = 4;
        
        // Read the rest of the response
        delay(100);
        while (sensorSerial.available() && responseLen < 64) {
          responseBuf[responseLen++] = sensorSerial.read();
        }
        break;
      }
    }
    delay(10);
  }
  
  if (gotResponse && responseLen >= 10) {
    // Parse response: FD FC FB FA [length_low] [length_high] [cmd_low] [cmd_high] [status_low] [status_high] [data...] 04 03 02 01
    uint16_t dataLength = responseBuf[4] | (responseBuf[5] << 8);
    uint16_t cmdWord = responseBuf[6] | (responseBuf[7] << 8);
    uint16_t status = responseBuf[8] | (responseBuf[9] << 8);
    
    Serial.print("✓ Response: Cmd=0x");
    Serial.print(cmdWord, HEX);
    Serial.print(" Status=");
    Serial.print(status == 0 ? "OK" : "FAIL");
    
    // For sensitivity query commands (0x0013, 0x0014), display the 14 gate values
    if ((cmdWord == 0x0113 || cmdWord == 0x0114) && responseLen >= 24 && status == 0) {
      Serial.println();
      Serial.print("    Gates[0-8]: ");
      for (int i = 0; i < 9 && (10 + i) < responseLen; i++) {
        Serial.print(responseBuf[10 + i]);
        Serial.print(" ");
      }
    }
    Serial.println();
  } else if (label) {
    Serial.println("⚠ No response");
   }
}
