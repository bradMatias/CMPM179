#include <Joystick.h>

//creating joystick
Joystick_ joy(JOYSTICK_DEFAULT_REPORT_ID, 
  JOYSTICK_TYPE_MULTI_AXIS, 32, 0,
  true, true, true, true, true, true,
  true, true, true, true, true);

//1=gnd 2=analogOut 3=5volt
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //range 0 - 1023
  joy.begin(true);//send data immeadiately
}

void loop() {
  //Serial.println(analogRead(A5));
  joy.setXAxis(analogRead(A5));
  joy.setZAxis(analogRead(A5));
  joy.setYAxis(512);
  joy.setRxAxis(analogRead(A5));
  delay(16);
}
