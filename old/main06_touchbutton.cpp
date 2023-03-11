#include <Arduino.h>
#include <M5Core2.h>

Button lt(0, 0, 160, 120, "left-top");
Button lb(0, 120, 160, 120, "left-bottom");
Button rt(160, 0, 160, 120, "right-top");
Button rb(160, 120, 160, 120, "right-bottom");

void colorButtons(Event& e) {
  Button& b = *e.button;
  M5.Lcd.fillRect(b.x, b.y, b.w, b.h, b.isPressed() ? WHITE : BLACK);
}

void dblTapped(Event& e) {
  Serial.println("--- TOP RIGHT BUTTON WAS DOUBLETAPPED ---");
}



void setup() {
  M5.begin();  //Initialize M5Core2
  int X = M5.Lcd.getCursorX();
  //M5.Lcd.print(X);
  int Y = M5.Lcd.getCursorY();
  //M5.Lcd.print(Y);
  //M5.Lcd.fillScreen(M5.Lcd.alphaBlend(128, 0X00FF00, 0XFF0000));
  //M5.Lcd.drawFastHLine(3, 100, 255, GREEN);
  M5.Lcd.loadFont("filename", SD);
  M5.Lcd.println("");
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(10,20);
  M5.Lcd.println("Loading...");
  M5.Lcd.progressBar(10, 100, 240, 20, 0);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 20);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 40);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 60);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 80);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.progressBar(10, 100, 240, 20, 100);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  delay(500);
  M5.Lcd.fillScreen(WHITE);
  M5.Buttons.addHandler(colorButtons,  E_TOUCH + E_RELEASE);
  rt.addHandler(dblTapped, E_DBLTAP);
  M5.Lcd.unloadFont();
}

void loop() {
  M5.update();
}



