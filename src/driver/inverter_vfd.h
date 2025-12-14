#pragma once
#include <Arduino.h>

class InverterVFD
{
public:
  InverterVFD(HardwareSerial &serial, unsigned long baud = 9600);
  void begin(uint8_t modbusAddress = 1);
  void setBaud(unsigned long baud);
  void setModbusAddress(uint8_t addr);
  bool readRegister(uint16_t reg, uint16_t &outValue, uint16_t timeout_ms = 200);
  bool writeRegister(uint16_t reg, uint16_t value, uint16_t timeout_ms = 200);
  bool setSpeedHz(float hz);
  bool start();
  bool stop();
  bool readOutputFrequency(float &hz);
  bool readOutputCurrent(float &ampere);
  bool readOutputVoltage(float &voltage);
  void setDebug(bool en);

private:
  HardwareSerial &modbusSerial;
  unsigned long currentBaud;
  uint8_t slaveAddr;
  bool debug;

  uint16_t calcCRC(const uint8_t *buf, uint16_t len);
  bool sendRequestAndReceive(const uint8_t *req, uint16_t reqLen,
                             uint8_t *resp, uint16_t &respLen,
                             uint16_t timeout_ms);
  static uint16_t beToU16(const uint8_t *buf);
  static void u16ToBe(uint8_t *buf, uint16_t v);

  static const uint16_t REG_FREQ_COMMAND = 0x2001;
  static const uint16_t REG_CONTROLL_COMMAND = 0x2000;
  static const uint16_t REG_OUTPUT_FREQ = 0x2103;
  static const uint16_t REG_OUTPUT_CURR = 0x2104;
  static const uint16_t REG_OUTPUT_VOLTAGE = 0x2109;
  static const float SCALE_FREQ;
};
