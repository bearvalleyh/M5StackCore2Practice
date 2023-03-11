#include <Arduino.h>
#include <M5Core2.h>

void setup() {
  M5.Lcd.begin();  //Initialize M5Core2
  M5.Lcd.fillScreen(RED);
  delay(1000);
  M5.Lcd.clear();    //Clear the content displayed on the screen.
}

void loop() {
}

