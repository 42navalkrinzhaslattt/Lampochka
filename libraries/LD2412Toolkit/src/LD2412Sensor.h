#ifndef LD2412_SENSOR_H
#define LD2412_SENSOR_H

#include <Arduino.h>

class LD2412Sensor {
public:
  LD2412Sensor(Stream &serial, int numGates = 14);

  void configure();
  bool readFrame();

  const uint8_t *moving() const { return _movingGates; }
  const uint8_t *stationary() const { return _stationaryGates; }

  int getNumGates() const { return _numGates; }

private:
  Stream &_serial;
  int _numGates;

  uint8_t *_movingGates;
  uint8_t *_stationaryGates;

  uint8_t _headerBuf[4];
  uint8_t _tailBuf[4];
  uint8_t _dataBuf[256];
  int _dataBufLen;
  bool _reading; // true = inside a data frame, collecting bytes

  bool parseFrame();
  void sendConfigCommand(const uint8_t *cmd, int cmdLen);
};

#endif
