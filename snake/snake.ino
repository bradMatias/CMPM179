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
//B??XXXYYY
//highest order bits unused
//0-2 used for Y Coordinate
//3-5 used for X Coordinate

byte convertXY(int x, int y);

size_t snakeLength = 3; //current length of the snake
const size_t totalLength = 12;
char snake[totalLength] = {convertXY(0,7),convertXY(1,7),convertXY(2,7)}; //starting on top left
char direction = 'r'; // r l u d // right left up down
//padded 1 pixel top and bottom // array listed in rows
byte gameOverImg[6] = {B01100110,B01100110,B00000000,B00000000,B01111110,B01000010};
unsigned int timer = 0;
bool dirChosen = false;

void setup() {
  //The MAX72XX is in power-saving mode on startup, we have to do a wakeup call   
  lc.shutdown(0,false);
  //Set the brightness to a medium values (0~15 possible values)
  lc.setIntensity(0,1);
  //and clear the display
  lc.clearDisplay(0);
  Serial.begin(9600);
}

void loop() {
  takeInput();
  timer++;
  if(timer==1000){
    moveSnake();
    lc.clearDisplay(0);
    for(int i=0;i<snakeLength;i++)
      lc.setLed(0,getX(snake[i]),getY(snake[i]),true);
    timer = 0;
    dirChosen = false;
  }
}

byte convertXY(int x, int y){
  return (((byte) x) << 3) | ((byte) y);
}

int getX(byte snakeLink){
  return (int) (B00111000 & snakeLink) >> 3;
}

int getY(byte snakeLink){
  return (int) (B00000111 & snakeLink);
}

void gameOver(){
  snakeLength = 3;
  direction = 'r';
  for(int i=0;i<snakeLength; i++){
    snake[i] = convertXY(i,7);
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
    if(getX(snake[snakeLength-2]) == 0){
      gameOver();
      return;
    }
    snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]) - 1, getY(snake[snakeLength-1]));
  }
  else if(direction=='r'){ // right
    if(getX(snake[snakeLength-2]) == 7){
      gameOver();
      return;
    }
    snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]) + 1, getY(snake[snakeLength-1]));
  }
  else if(direction=='u'){ // up
    if(getY(snake[snakeLength-2]) == 7){
      gameOver();
      return;
    }
    snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) + 1);
  }
  else{                    // down
    if(getY(snake[snakeLength-2]) == 0){
      gameOver();
      return;
    }
    snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) - 1);
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

