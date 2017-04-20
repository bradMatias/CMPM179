#include "LedControl.h"

//LedControl(int dataPin, int clkPin, int csPin, int numDevices);
//VCC = 5V GND = ground DIN = 12 CS = 10 CLK = 11
LedControl lc=LedControl(12,11,10,1);

int flexValue = 0;
int timer = 0;
int flexPin = A5;
int rows = 0;

void setup() {
  //The MAX72XX is in power-saving mode on startup, we have to do a wakeup call   
  lc.shutdown(0,false);
  //Set the brightness to a medium values (0~15 possible values)
  lc.setIntensity(0,1);
  //and clear the display
  lc.clearDisplay(0);
  Serial.begin(9600);
  randomSeed(analogRead(0));//randomizing using a reasonably random analog read
}

void loop() {
  timer++;//timer used to delay game speed for reasonable framerate
  if(timer==100){//move snake and draw a new frame
    flexValue = analogRead(flexPin);
    rows = floor(flexValue / 100);
    Serial.println(rows);
    Serial.println(flexValue);
    lc.clearDisplay(0);
    for(int i=0;i<rows;i++)
      lc.setColumn(0,i,B11111111);
    timer = 0;
  }
}


