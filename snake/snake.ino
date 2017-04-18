#include <NESpad.h>
#include "LedControl.h"

//LedControl(int dataPin, int clkPin, int csPin, int numDevices);
//VCC = 5V GND = ground DIN = 12 CS = 10 CLK = 11
LedControl lc=LedControl(12,11,10,1);

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

//format for byte array
//BXXXXYYYY
//0-3 used for Y Coordinate
//4-7 used for X Coordinate

//coords start at 1 and goto 8 as a workaround for non-signed
byte convertXY(int x, int y);

size_t snakeLength = 3; //current length of the snake
const size_t totalLength = 12;
byte snake[totalLength] = {convertXY(1,8),convertXY(2,8),convertXY(3,8)}; //starting on top left
char direction = 'r'; // r l u d // right left up down
//padded 1 pixel top and bottom // array listed in rows
byte gameOverImg[6] = {B01100110,B01100110,B00000000,B00000000,B01111110,B01000010};
unsigned int timer = 0;
bool dirChosen = false;
byte foodPos;
bool foodFlicker = true;

void setup() {
  //The MAX72XX is in power-saving mode on startup, we have to do a wakeup call   
  lc.shutdown(0,false);
  //Set the brightness to a medium values (0~15 possible values)
  lc.setIntensity(0,1);
  //and clear the display
  lc.clearDisplay(0);
  Serial.begin(9600);
  randomSeed(analogRead(0));
  foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
}


//problem is you cant collect food when u pick it up mid turn
void loop() {
  takeInput();
  timer++;
  if(timer % 200 == 0){
    lc.setLed(0,getX(foodPos)-1,getY(foodPos)-1,foodFlicker);
    foodFlicker = !foodFlicker;
    //Serial.println(getX(foodPos)-1);
    //Serial.println(getY(foodPos)-1);
  }
  if(timer==1000){
    moveSnake();
    lc.clearDisplay(0);
    for(int i=0;i<snakeLength;i++)
      lc.setLed(0,getX(snake[i])-1,getY(snake[i])-1,true);
    timer = 0;
    dirChosen = false;
  }
}

byte convertXY(int x, int y){
  return (((byte) x) << 4) | ((byte) y);
}

int getX(byte snakeLink){
  return (int) (B11110000 & snakeLink) >> 4;
}

int getY(byte snakeLink){
  return (int) (B00001111 & snakeLink);
}

void gameOver(){
  foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
  snakeLength = 3;
  direction = 'r';
  for(int i=0;i<snakeLength; i++){
    snake[i] = convertXY(i+1,8);
  }
  lc.clearDisplay(0);
  for(int i=0;i<6;i++)//the X and Y and row and columns are so backwards lol
    lc.setColumn(0,6-i,gameOverImg[i]);
  delay(1000);
}

void moveSnake(){ 
  for(int i=0;i<snakeLength-1;i++){
    snake[i] = snake[i+1];
  }//moving each link up
  
  if(direction=='l'){      // left
    snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]) - 1, getY(snake[snakeLength-1]));
    if(getX(snake[snakeLength-1]) == 0){
      gameOver();
      return;
    }else if(convertXY(getX(snake[snakeLength-1]) - 1, getY(snake[snakeLength-1])) == foodPos){
      foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
      snake[snakeLength] = convertXY(getX(snake[snakeLength-1]) - 1, getY(snake[snakeLength-1]));
      snakeLength++;
    }
  }
  else if(direction=='r'){ // right
    snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]) + 1, getY(snake[snakeLength-1]));
    if(getX(snake[snakeLength-1]) == 9){
      gameOver();
      return;
    }else if(convertXY(getX(snake[snakeLength-1]) + 1, getY(snake[snakeLength-1])) == foodPos){
      foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
      snake[snakeLength] = convertXY(getX(snake[snakeLength-1]) + 1, getY(snake[snakeLength-1]));
      snakeLength++;
    }
  }
  else if(direction=='u'){ // up
    snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) + 1);
    if(getY(snake[snakeLength-1]) == 9){
      gameOver();
      return;
    }else if(convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) + 1) == foodPos){
      foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
      snake[snakeLength] = convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) + 1);
      snakeLength++;
    }
  }
  else{                    // down
    snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) - 1);
    if(getY(snake[snakeLength-1]) == 0){
      gameOver();
      return;
    }else if(convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) - 1) == foodPos){
      foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
      snake[snakeLength] = convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) - 1);
      snakeLength++;
    }
  }
}

void takeInput(){
  state = nes.buttons();
  
  //dirChosen makes it so we only pick 1 direction per update to move in
  if(!dirChosen && direction != 'd' && (state & NES_UP)){
    direction = 'u';
    dirChosen = true;
  }else if(!dirChosen && direction != 'u' && (state & NES_DOWN)){
    direction = 'd';
    dirChosen = true;
  }else if(!dirChosen && direction != 'r' && (state & NES_LEFT)){
    direction = 'l';
    dirChosen = true;
  }else if(!dirChosen && direction != 'l' && (state & NES_RIGHT)){
    direction = 'r';
    dirChosen = true;
  }
}

