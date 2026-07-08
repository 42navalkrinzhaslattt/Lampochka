#include "LD2412Sensor.h"

LD2412Sensor::LD2412Sensor(Stream &serial, int numGates)
    : _serial(serial), _numGates(numGates), _dataBufLen(0), _reading(false) {
  _movingGates = new uint8_t[numGates];
  _stationaryGates = new uint8_t[numGates];
  memset(_movingGates, 0, numGates);
  memset(_stationaryGates, 0, numGates);
  memset(_headerBuf, 0, 4);
}

void LD2412Sensor::configure() {
  while (_serial.available())
    _serial.read();
  delay(100);

  const uint8_t enableConfigCmd[] = {0x04, 0x00, 0xFF, 0x00, 0x01, 0x00};
  sendConfigCommand(enableConfigCmd, sizeof(enableConfigCmd));
  delay(500);

  const uint8_t maxGateCmd[] = {0x14, 0x00, 0x60, 0x00, 0x00, 0x00, 0x0D,
                                0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00,
                                0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
  sendConfigCommand(maxGateCmd, sizeof(maxGateCmd));
  delay(500);

  const uint8_t motionSensCmd[] = {0x10, 0x00, 0x03, 0x00, 0x00, 15, 15,
                                   15,   15,   15,   15,   15,   15, 15,
                                   15,   15,   15,   15,   15};
  sendConfigCommand(motionSensCmd, sizeof(motionSensCmd));
  delay(500);

  const uint8_t stationarySensCmd[] = {0x10, 0x00, 0x04, 0x00, 0x00, 20, 20,
                                       20,   20,   20,   20,   20,   20, 20,
                                       20,   20,   20,   20,   20};
  sendConfigCommand(stationarySensCmd, sizeof(stationarySensCmd));
  delay(500);

  const uint8_t enableEngineeringCmd[] = {0x02, 0x00, 0x62, 0x00};
  sendConfigCommand(enableEngineeringCmd, sizeof(enableEngineeringCmd));
  delay(500);

  const uint8_t endConfigCmd[] = {0x02, 0x00, 0xFE, 0x00};
  sendConfigCommand(endConfigCmd, sizeof(endConfigCmd));
  delay(500);

  while (_serial.available())
    _serial.read();

  // Reset parser state so stale bytes from config ACKs don't
  // interfere with data frame detection
  _reading = false;
  _dataBufLen = 0;
  memset(_headerBuf, 0, 4);
}

void LD2412Sensor::sendConfigCommand(const uint8_t *cmd, int cmdLen) {
  const uint8_t header[] = {0xFD, 0xFC, 0xFB, 0xFA};
  const uint8_t tail[] = {0x04, 0x03, 0x02, 0x01};
  _serial.write(header, 4);
  _serial.write(cmd, cmdLen);
  _serial.write(tail, 4);
}

bool LD2412Sensor::readFrame() {
  // Non-blocking state machine: process available bytes one at a time.
  while (_serial.available()) {
    uint8_t b = _serial.read();

    if (!_reading) {
      // Hunting for data frame header: F4 F3 F2 F1
      _headerBuf[0] = _headerBuf[1];
      _headerBuf[1] = _headerBuf[2];
      _headerBuf[2] = _headerBuf[3];
      _headerBuf[3] = b;

      if (_headerBuf[0] == 0xF4 && _headerBuf[1] == 0xF3 &&
          _headerBuf[2] == 0xF2 && _headerBuf[3] == 0xF1) {
        _reading = true;
        _dataBufLen = 0;
        // Reset tail buffer to prevent immediate premature matching
        memset(_tailBuf, 0, 4);
      }
    } else {
      // Stash byte in payload buffer
      if (_dataBufLen < 256) {
        _dataBuf[_dataBufLen++] = b;
      }

      // Track tail independently using sliding window F8 F7 F6 F5
      _tailBuf[0] = _tailBuf[1];
      _tailBuf[1] = _tailBuf[2];
      _tailBuf[2] = _tailBuf[3];
      _tailBuf[3] = b;

      // Overflow recovery
      if (_dataBufLen >= 128) {
        _reading = false;
        memset(_headerBuf, 0, 4);
        continue;
      }

      // Check if we hit the exact tail sequence
      if (_tailBuf[0] == 0xF8 && _tailBuf[1] == 0xF7 &&
          _tailBuf[2] == 0xF6 && _tailBuf[3] == 0xF5) {
        
        // A complete frame (header...payload...tail) receives the tail bytes 
        // into _dataBuf as well. Let's strip them from the payload length.
        if (_dataBufLen >= 4) {
             _dataBufLen -= 4;
        }

        _reading = false;
        memset(_headerBuf, 0, 4);

        // --- STRICT LENGTH VALIDATION ---
        // The first two bytes of the payload (after header) dictate the length
        // of the intra-frame data (everything between length bytes and tail).
        if (_dataBufLen >= 2) {
           int expectedLen = _dataBuf[0] | (_dataBuf[1] << 8);
           // The length reported by the sensor does NOT include the 2 bytes of the length field itself,
           // so the total payload size we collected (before the tail) should be expectedLen + 2.
           if (_dataBufLen == expectedLen + 2) {
               if (parseFrame()) {
                 return true;
               }
           } else {
               // Length mismatch: likely hit a fake tail inside the payload,
               // or lost bytes due to overrun. Discard this corrupted frame.
               // Serial.println("WARN: Frame length mismatch");
           }
        }
      }
    }
  }
  return false;
}

bool LD2412Sensor::parseFrame() {
  // Frame format (after header F4 F3 F2 F1):
  // Bytes 0-1: Data length (little endian)
  // Byte 2: Data type (0x01 = engineering mode with gate data)
  // Byte 3: Target state
  // Bytes 4-7: Target info
  // Bytes 8-10: Energy info
  // Bytes 11-12: Max gate indices (0x0D = 13 for 14 gates)
  // Bytes 13-26: Moving gate energies (14 values, 0-100)
  // Bytes 27-40: Stationary gate energies (14 values, 0-100)
  // ... rest of frame

  if (_dataBufLen < 41) // Minimum for gate data
    return false;

  uint8_t dataType = _dataBuf[2];

  // Only process long engineering frames (dataType 0x01)
  // Short frames (dataType 0x02) don't have gate data
  if (dataType != 0x01)
    return false;

  // Gate energies start at byte 13
  const int moveStart = 13;
  for (int i = 0; i < _numGates; i++) {
    _movingGates[i] = _dataBuf[moveStart + i];
  }

  const int statStart = moveStart + 14; // byte 27
  for (int i = 0; i < _numGates; i++) {
    _stationaryGates[i] = _dataBuf[statStart + i];
  }

  return true; // Gate data was successfully parsed
}
