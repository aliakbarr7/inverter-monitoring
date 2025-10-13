#pragma once

#include <Arduino.h>
#include <Fuzzy.h>

class FuzzyController {
public:
  FuzzyController();
  void begin(uint8_t numInputs, uint8_t numOutputs);
  void addInput(uint8_t id, FuzzySet *sets[], uint8_t setCount);
  void addOutput(uint8_t id, FuzzySet *sets[], uint8_t setCount);
  void addRule(uint8_t ruleId, FuzzySet *inputSet, FuzzySet *outputSet);
  bool setInputValue(uint8_t inputId, float value);
  bool compute();
  float getOutputValue(uint8_t outputId);

private:
  Fuzzy fuzzy;
  FuzzyInput **inputs;
  FuzzyOutput **outputs;
  uint8_t inputCount;
  uint8_t outputCount;
};