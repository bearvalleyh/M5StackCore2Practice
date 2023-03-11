#include <Arduino.h>
#include <M5Core2.h>

void setup() {
  M5.begin();  //Initialize M5Core2
  int X = M5.Lcd.getCursorX();
  M5.Lcd.print(X);
  int Y = M5.Lcd.getCursorY();
  M5.Lcd.print(Y);
  M5.Lcd.fillScreen(M5.Lcd.alphaBlend(128, 0X00FF00, 0XFF0000));
  M5.Lcd.drawFastHLine(3, 100, 255, GREEN);
  M5.Lcd.loadFont("filename", SD);
  M5.Lcd.setTextSize(4);
  M5.Lcd.print("Hello");
  M5.Lcd.setRotation(2);  //Rotate the screen 180 degrees clockwise (2*90)
  M5.Lcd.fillEllipse(160, 100, 60, 100, YELLOW);    //Create a yellow ellipse at (160, 100) with the long axis and the short axis to 60 and 100 respectively.
  M5.Lcd.print(M5.Lcd.getRotation());    //Display screen rotation direction.
  delay(1000);
  M5.Lcd.setRotation(1);  //Restore the screen to the default display state
  M5.Lcd.fillRect(150, 120, 122, 10, BLUE); 
  M5.Lcd.print(M5.Lcd.getRotation());    //Display screen rotation direction.
  delay(1000);
  M5.Lcd.setRotation(2);  //Restore the screen to the default display state
  M5.Lcd.drawRoundRect(55,55,30,50,10,RED); 
  M5.Lcd.print(M5.Lcd.getRotation());    //Display screen rotation direction.
  M5.Lcd.unloadFont();
  M5.Lcd.setRotation(0);  //Restore the screen to the default display state
  M5.Lcd.fillRoundRect(55, 55, 30, 50, 10, GREEN);
  M5.Lcd.setTextColor(RED,BLACK);
  M5.Lcd.print("Hello");
  M5.Lcd.drawFastVLine(100, 0, 255, TFT_GREEN);
  M5.Lcd.drawCircle(100, 100, 50, YELLOW);
  M5.Lcd.progressBar(0, 0, 240, 20, 20);    //Display a progress bar with a width of 240 and a 20% progress at (0, 0)
  M5.Lcd.qrcode("http://www.m5stack.com", 50, 10, 220, 6);
}

void loop(){
  M5.update();
  for(int i=2500; i<3300;i++){
    M5.Axp.SetLcdVoltage(i);  //Set the voltage value every 10ms
    delay(10);
  }
  for(int i=3300; i>2500;i--){
    M5.Axp.SetLcdVoltage(i);
    delay(10);
  }
  M5.Lcd.invertDisplay(1);    //Turn on Invert
  delay(1000);
  M5.Lcd.invertDisplay(0);    //Turn off Invert
}


