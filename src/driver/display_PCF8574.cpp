#include "display_PCF8574.h"

DisplayPCF8574::DisplayPCF8574(uint8_t addr, uint8_t c, uint8_t r)
  : lcd(addr, c, r), cols(c), rows(r), labels(nullptr), values(nullptr), count(0), precision(1), lastUpdate(0) {}

void DisplayPCF8574::begin() {
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void DisplayPCF8574::clear() {
  lcd.clear();
}

void DisplayPCF8574::setCursor(uint8_t col, uint8_t row) {
  lcd.setCursor(col, row);
}

void DisplayPCF8574::print(const String &text) {
  lcd.print(text);
}

void DisplayPCF8574::printAt(uint8_t col, uint8_t row, const String &text) {
  lcd.setCursor(col, row);
  lcd.print(text);
}

void DisplayPCF8574::printCenter(uint8_t row, const String &text) {
  int pos = (cols - text.length()) / 2;
  if (pos < 0) pos = 0;
  lcd.setCursor(pos, row);
  lcd.print(text);
}

void DisplayPCF8574::setBacklight(bool state) {
  state ? lcd.backlight() : lcd.noBacklight();
}

void DisplayPCF8574::setData(const String labels[], float *vals, uint8_t cnt, uint8_t prec) {
  labels ? this->labels = labels : this->labels = nullptr;
  this->values = vals;
  this->count = cnt;
  this->precision = prec;
}

void DisplayPCF8574::update() {
  if (!labels || !values) return;

  unsigned long now = millis();
  if (now - lastUpdate < 500) return;

  lastUpdate = now;
  lcd.clear();

  char buf[16];
  for (uint8_t i = 0; i < count && i < rows; i++) {
    lcd.setCursor(0, i);
    lcd.print(labels[i]);
    lcd.print(": ");
    dtostrf(values[i], 0, precision, buf);
    lcd.print(buf);
  }
}

