#include "inverter_vfd.h"

const float InverterVFD::SCALE_FREQ = 0.01f;

InverterVFD::InverterVFD(HardwareSerial &serial, unsigned long baud)
    : modbusSerial(serial), currentBaud(baud), slaveAddr(1), debug(false) {}

void InverterVFD::begin(uint8_t modbusAddress)
{
  slaveAddr = modbusAddress;
  modbusSerial.begin(currentBaud, SERIAL_8N1);
  delay(20);
}

void InverterVFD::setBaud(unsigned long baud)
{
  currentBaud = baud;
  modbusSerial.updateBaudRate(baud);
}

void InverterVFD::setModbusAddress(uint8_t addr)
{
  slaveAddr = addr;
}

uint16_t InverterVFD::calcCRC(const uint8_t *buf, uint16_t len)
{
  uint16_t crc = 0xFFFF;
  for (uint16_t pos = 0; pos < len; pos++)
  {
    crc ^= buf[pos];
    for (int i = 0; i < 8; i++)
      crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
  }
  return crc;
}

uint16_t InverterVFD::beToU16(const uint8_t *buf)
{
  return (uint16_t(buf[0]) << 8) | buf[1];
}

void InverterVFD::u16ToBe(uint8_t *buf, uint16_t v)
{
  buf[0] = (v >> 8) & 0xFF;
  buf[1] = v & 0xFF;
}

bool InverterVFD::sendRequestAndReceive(const uint8_t *req, uint16_t reqLen,
                                        uint8_t *resp, uint16_t &respLen,
                                        uint16_t timeout_ms)
{
  while (modbusSerial.available())
    modbusSerial.read();
  modbusSerial.write(req, reqLen);
  modbusSerial.flush();
  delayMicroseconds(200);

  uint32_t start = millis();
  uint16_t idx = 0;
  while (millis() - start < timeout_ms)
  {
    while (modbusSerial.available() && idx < respLen)
      resp[idx++] = modbusSerial.read();
    if (idx >= 5)
    {
      uint16_t computed = calcCRC(resp, idx - 2);
      uint16_t received = resp[idx - 2] | (resp[idx - 1] << 8);
      if (computed == received)
      {
        respLen = idx;
        return true;
      }
    }
  }
  respLen = idx;
  return false;
}

bool InverterVFD::readRegister(uint16_t reg, uint16_t &outValue, uint16_t timeout_ms)
{
  uint8_t req[8] = {slaveAddr, 0x03};
  u16ToBe(&req[2], reg);
  u16ToBe(&req[4], 1);
  uint16_t crc = calcCRC(req, 6);
  req[6] = crc & 0xFF;
  req[7] = crc >> 8;

  uint8_t resp[9];
  uint16_t respLen = sizeof(resp);
  if (!sendRequestAndReceive(req, sizeof(req), resp, respLen, timeout_ms))
    return false;
  if (respLen < 7 || resp[0] != slaveAddr || (resp[1] & 0x80))
    return false;
  outValue = beToU16(&resp[3]);
  return true;
}

bool InverterVFD::writeRegister(uint16_t reg, uint16_t value, uint16_t timeout_ms)
{
  uint8_t req[8] = {slaveAddr, 0x06};
  u16ToBe(&req[2], reg);
  u16ToBe(&req[4], value);
  uint16_t crc = calcCRC(req, 6);
  req[6] = crc & 0xFF;
  req[7] = crc >> 8;

  uint8_t resp[8];
  uint16_t respLen = sizeof(resp);
  if (!sendRequestAndReceive(req, sizeof(req), resp, respLen, timeout_ms))
    return false;
  return memcmp(req, resp, 6) == 0;
}

bool InverterVFD::setSpeedHz(float hz)
{
  uint16_t val = (uint16_t)(hz / SCALE_FREQ);
  return writeRegister(REG_FREQ_COMMAND, val);
}

bool InverterVFD::start()
{
  return writeRegister(REG_CONTROLL_COMMAND, 0x0002);
}

bool InverterVFD::stop()
{
  return writeRegister(REG_CONTROLL_COMMAND, 0x0001);
}

bool InverterVFD::readOutputFrequency(float &hz)
{
  uint16_t raw;
  if (!readRegister(REG_OUTPUT_FREQ, raw))
    return false;
  hz = raw * SCALE_FREQ;
  return true;
}

bool InverterVFD::readOutputCurrent(float &ampere)
{
  uint16_t raw;
  if (!readRegister(REG_OUTPUT_CURR, raw))
    return false;
  ampere = raw * 0.01f;
  return true;
}

bool InverterVFD::readOutputVoltage(float &voltage)
{
  uint16_t raw;
  if (!readRegister(REG_OUTPUT_VOLTAGE, raw))
    return false;
  voltage = raw * 0.1f;
  return true;
}

void InverterVFD::setDebug(bool en)
{
  debug = en;
}
