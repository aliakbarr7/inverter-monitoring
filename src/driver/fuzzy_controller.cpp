#include "fuzzy_controller.h"

FuzzyController::FuzzyController() : inputs(nullptr), outputs(nullptr), inputCount(0), outputCount(0) {}

void FuzzyController::begin(uint8_t numInputs, uint8_t numOutputs) {
  inputCount = numInputs;
  outputCount = numOutputs;

  inputs = new FuzzyInput*[inputCount];
  outputs = new FuzzyOutput*[outputCount];

  for (uint8_t i = 0; i < inputCount; i++) {
    inputs[i] = new FuzzyInput(i + 1);
    fuzzy.addFuzzyInput(inputs[i]);
  }
  for (uint8_t i = 0; i < outputCount; i++) {
    outputs[i] = new FuzzyOutput(i + 1);
    fuzzy.addFuzzyOutput(outputs[i]);
  }
}

void FuzzyController::addInput(uint8_t id, FuzzySet *sets[], uint8_t setCount) {
  if (id == 0 || id > inputCount) return;
  for (uint8_t i = 0; i < setCount; i++) inputs[id - 1]->addFuzzySet(sets[i]);
}

void FuzzyController::addOutput(uint8_t id, FuzzySet *sets[], uint8_t setCount) {
  if (id == 0 || id > outputCount) return;
  for (uint8_t i = 0; i < setCount; i++) outputs[id - 1]->addFuzzySet(sets[i]);
}

void FuzzyController::addRule(uint8_t ruleId, FuzzySet *inputSet, FuzzySet *outputSet) {
  FuzzyRuleAntecedent *ant = new FuzzyRuleAntecedent();
  ant->joinSingle(inputSet);
  FuzzyRuleConsequent *con = new FuzzyRuleConsequent();
  con->addOutput(outputSet);
  fuzzy.addFuzzyRule(new FuzzyRule(ruleId, ant, con));
}

bool FuzzyController::setInputValue(uint8_t inputId, float value) {
  if (inputId == 0 || inputId > inputCount) return false;
  return fuzzy.setInput(inputId, value);
}

bool FuzzyController::compute() {
  fuzzy.fuzzify();
  return true;
}

float FuzzyController::getOutputValue(uint8_t outputId) {
  if (outputId == 0 || outputId > outputCount) return 0;
  return fuzzy.defuzzify(outputId);
}
