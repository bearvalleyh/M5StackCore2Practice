#include <Arduino.h>
#include <M5Core2.h>

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
  //M5.Lcd.fillScreen(M5.Lcd.alphaBlend(128, 0X00FF00, 0XFF0000));
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.unloadFont();
}

void loop(){
  M5.update();
  //M5.Lcd.invertDisplay(1);    //Turn on Invert
  //delay(1000);
  M5.Lcd.setCursor(0, 0);
  //M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(WHITE,BLACK);
  M5.Lcd.printf("Button A Status: %d ",M5.BtnA.read());   
  //M5.Lcd.invertDisplay(0);    //Turn off Invert
  M5.Lcd.println("");
  M5.Lcd.println("");
  M5.Lcd.println("The last change"); 
  M5.Lcd.printf(" at %d ms /n",M5.BtnA.lastChange()); 
  M5.Lcd.println("");
  M5.Lcd.println("");
  M5.Lcd.println("");

  delay(100);
  //M5.Lcd.setCursor(0, 0);
  if (M5.BtnA.isPressed()) {    //If the key is pressed. 
    M5.Lcd.setTextColor(RED,WHITE);
    M5.Lcd.println("Button is Pressed..");
  }else{
    M5.Lcd.setTextColor(BLUE,WHITE);
    M5.Lcd.println("Button is Released.");
  }
  delay(200);
  
}


