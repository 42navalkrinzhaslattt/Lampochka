// sensor_config.h - shared sensor configuration helpers
#ifndef SHARED_SENSOR_CONFIG_H
#define SHARED_SENSOR_CONFIG_H

void sendConfigCommand(uint8_t* cmd, int cmdLen) {
  const uint8_t header[] = {0xFD, 0xFC, 0xFB, 0xFA};
  sensorSerial.write(header, 4);
  sensorSerial.write(cmd, cmdLen);
  const uint8_t tail[] = {0x04, 0x03, 0x02, 0x01};
  sensorSerial.write(tail, 4);
}

void configureSensor() {
  while (sensorSerial.available()) sensorSerial.read();
  delay(100);

  uint8_t enableConfigCmd[] = {0x04, 0x00, 0xFF, 0x00, 0x01, 0x00};
  sendConfigCommand(enableConfigCmd, sizeof(enableConfigCmd));
  delay(500);

  uint8_t maxGateCmd[] = {
    0x14, 0x00,
    0x60, 0x00,
    0x00, 0x00,
    0x0D, 0x00,
    0x00, 0x00,
    0x0D, 0x00,
    0x00, 0x00,
    0x05, 0x00,
    0x00, 0x00,
    0x00, 0x00
  };
  sendConfigCommand(maxGateCmd, sizeof(maxGateCmd));
  delay(500);

  uint8_t motionSensCmd[] = {
    0x10, 0x00,
    0x03, 0x00,
    0x00,
    15,15,15,15,15,15,
    15,15,15,15,15,15,15,15
  };
  sendConfigCommand(motionSensCmd, sizeof(motionSensCmd));
  delay(500);

  uint8_t stationarySensCmd[] = {
    0x10, 0x00,
    0x04, 0x00,
    0x00,
    20,20,20,20,20,20,
    20,20,20,20,20,20,20,20
  };
  sendConfigCommand(stationarySensCmd, sizeof(stationarySensCmd));
  delay(500);

  uint8_t enableEngineeringCmd[] = {0x02, 0x00, 0x62, 0x00};
  sendConfigCommand(enableEngineeringCmd, sizeof(enableEngineeringCmd));
  delay(500);

  uint8_t endConfigCmd[] = {0x02, 0x00, 0xFE, 0x00};
  sendConfigCommand(endConfigCmd, sizeof(endConfigCmd));
  delay(500);

  while (sensorSerial.available()) sensorSerial.read();
}

#endif
