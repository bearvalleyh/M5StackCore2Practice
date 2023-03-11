#include <Arduino.h>
#include <M5Core2.h>

void setup() {
  M5.begin();
}
void loop() {
  M5.Axp.SetLed(1);
  delay(1000);
  M5.Axp.SetLed(0);
  delay(1000);
}
