#include <Arduino.h>
#include "driver/fuzzy_controller.h"

FuzzyController fz;

FuzzySet eNeg(-100, -100, -50, 0);
FuzzySet eZero(-25, 0, 0, 25);
FuzzySet ePos(0, 50, 100, 100);
FuzzySet outLow(0, 0, 25, 50);
FuzzySet outMid(25, 50, 50, 75);
FuzzySet outHigh(50, 75, 100, 100);

void setup() {
  Serial.begin(115200);

  fz.begin(1, 1);
  FuzzySet* inputSets[] = { &eNeg, &eZero, &ePos };
  FuzzySet* outputSets[] = { &outLow, &outMid, &outHigh };
  fz.addInput(1, inputSets, 3);
  fz.addOutput(1, outputSets, 3);
  fz.addRule(1, &eNeg, &outHigh);
  fz.addRule(2, &eZero, &outMid);
  fz.addRule(3, &ePos, &outLow);
}

void loop() {
  float err = random(-100, 100);
  fz.setInputValue(1, err);
  fz.compute();
  float result = fz.getOutputValue(1);
  Serial.print("Error: "); Serial.print(err);
  Serial.print(" -> Output: "); Serial.println(result);
  delay(500);
}
