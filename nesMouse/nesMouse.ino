#include <NESpad.h>
#include <Mouse.h>

//NES controller init
//6 = strobe 7 = clock 5 = data
/////////////////////////
////////// (GND)/////////
///////(5v)(CLK)/////////
///////(na)(SRB)/////////
///////(na)(DTA)/////////
/////////////////////////
NESpad nes = NESpad(6, 7, 5);
byte state;
const float sens = 1;
const int responseDelay = 5;
const int scrollAmount = 1;
const float maxMAccel = 6;
const float accelAmount = 0.1;
int scrollSlower = 0;
float mAccel = 0;
bool directionPressed = false;

void setup() {
  Mouse.begin();
}

void loop() {
  takeInput();
  delay(responseDelay);
}

void takeInput(){
  state = nes.buttons();
  
  //directional input
  directionPressed = false;
  if(state & NES_UP){
    Mouse.move(0, floor(-sens - mAccel), 0);
    directionPressed = true;
  }else if(state & NES_DOWN){
    Mouse.move(0, floor(sens + mAccel), 0);
    directionPressed = true;
  }
  //directional input
  if(state & NES_LEFT){
    Mouse.move(floor(-sens - mAccel), 0, 0);
    directionPressed = true;
  }else if(state & NES_RIGHT){
    Mouse.move(floor(sens + mAccel), 0, 0);
    directionPressed = true;
  }

  if(directionPressed && mAccel < maxMAccel){
    mAccel += accelAmount;
  }else if(!directionPressed){
    mAccel = 0;
  }

  //left click
  if(state & NES_B){
    Mouse.press(MOUSE_LEFT);
  }else if(Mouse.isPressed(MOUSE_LEFT)) {
    Mouse.release(MOUSE_LEFT);
  }

  //right click
  if(state & NES_A){
    Mouse.press(MOUSE_RIGHT);
  }else if(Mouse.isPressed(MOUSE_RIGHT)) {
    Mouse.release(MOUSE_RIGHT);
  }

  //scroll up/down
  if(state & NES_START){
    if(scrollSlower == 0){
      Mouse.move(0, 0, -scrollAmount);
      scrollSlower = 4;
    }else{
      scrollSlower--;
    }
  }else if(state & NES_SELECT){
    if(scrollSlower == 0){
      Mouse.move(0, 0, scrollAmount);
      scrollSlower = 4;
    }else{
      scrollSlower--;
    }
  }
}

