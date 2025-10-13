#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class DisplayPCF8574 {
public:
  DisplayPCF8574(uint8_t addr = 0x27, uint8_t cols = 16, uint8_t rows = 2);
  void begin();
  void clear();
  void setCursor(uint8_t col, uint8_t row);
  void print(const String &text);
  void printAt(uint8_t col, uint8_t row, const String &text);
  void printCenter(uint8_t row, const String &text);
  void setBacklight(bool state);
  void setData(const String labels[], float *values, uint8_t count, uint8_t precision = 1);
  void update();

private:
  LiquidCrystal_I2C lcd;
  uint8_t cols, rows;
  const String *labels;
  float *values;
  uint8_t count;
  uint8_t precision;
  unsigned long lastUpdate;
};