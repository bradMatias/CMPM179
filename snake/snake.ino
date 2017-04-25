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

byte snakeLength = 3; //current length of the snake
const byte totalLength = 32;//max length of snake
byte snake[totalLength] = {convertXY(1,8),convertXY(2,8),convertXY(3,8)}; //starting on top left
char direction = 'r'; // r l u d // right left up down
//padded 1 pixel top and bottom // array listed in rows
byte gameOverImg[6] = {B01100110,B01100110,B00000000,B00000000,B01111110,B01000010};
unsigned int timer = 0;
bool dirChosen = false;//was a direction chosen this frame?
byte foodPos;//position of the food particle 
bool foodFlicker = true;//bool used to flicker the food particle

void setup() {
  //The MAX72XX is in power-saving mode on startup, we have to do a wakeup call   
  lc.shutdown(0,false);
  //Set the brightness to a medium values (0~15 possible values)
  lc.setIntensity(0,1);
  //and clear the display
  lc.clearDisplay(0);
  Serial.begin(9600);
  randomSeed(analogRead(0));//randomizing using a reasonably random analog read
  foodPos = convertXY(floor(random(8))+1,floor(random(8))+1); //range [1,8]
}

void loop() {
  takeInput();
  timer++;//timer used to delay game speed for reasonable framerate
  if(timer % 200 == 0){//flickering food particle
    lc.setLed(0,getX(foodPos)-1,getY(foodPos)-1,foodFlicker);
    foodFlicker = !foodFlicker;
  }
  if(timer==1000){//move snake and draw a new frame
    moveSnake();
    lc.clearDisplay(0);
    for(int i=0;i<snakeLength;i++)
      lc.setLed(0,getX(snake[i])-1,getY(snake[i])-1,true);
    timer = 0;
    dirChosen = false;
  }
}

//converts 2 ints into a single position byte
byte convertXY(int x, int y){
  return (((byte) x) << 4) | ((byte) y);
}

//converts position byte into an int x coordinate
int getX(byte snakeLink){
  return (int) (B11110000 & snakeLink) >> 4;
}

//converts position byte into an int y coordinate
int getY(byte snakeLink){
  return (int) (B00001111 & snakeLink);
}

//draws game over sad face and resets game
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

//runs the game logic, moving and eating
void moveSnake(){
  if(direction == 'l' && snake[snakeLength-1] - B00010000 == foodPos){
    snakeLength++;
    snake[snakeLength-1] = foodPos;
    foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
  }else if(direction == 'r' && snake[snakeLength-1] + B00010000 == foodPos){
    snakeLength++;
    snake[snakeLength-1] = foodPos;
    foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
  }else if(direction == 'u' && snake[snakeLength-1] + B0000001 == foodPos){
    snakeLength++;
    snake[snakeLength-1] = foodPos;
    foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
  }else if(direction == 'd' && snake[snakeLength-1] - B0000001 == foodPos){
    snakeLength++;
    snake[snakeLength-1] = foodPos;
    foodPos = convertXY(floor(random(8))+1,floor(random(8))+1);
  }else{
    //moving each link up 
    for(int i=0;i<snakeLength-1;i++){
      snake[i] = snake[i+1];
    }

    //we didn't get a food so move as normal
    if(direction=='l'){      // left
      //snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]) - 1, getY(snake[snakeLength-1]));
      snake[snakeLength-1] -= B00010000;
      if(getX(snake[snakeLength-1]) == 0){
        gameOver();
        return;
      }
    }
    else if(direction=='r'){ // right
      //snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]) + 1, getY(snake[snakeLength-1]));
      snake[snakeLength-1] += B00010000;
      if(getX(snake[snakeLength-1]) == 9){
        gameOver();
        return;
      }
    }
    else if(direction=='u'){ // up
      //snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) + 1);
      snake[snakeLength-1] += B00000001;
      if(getY(snake[snakeLength-1]) == 9){
        gameOver();
        return;
      }
    }
    else{                    // down
      //snake[snakeLength-1] = convertXY(getX(snake[snakeLength-1]), getY(snake[snakeLength-1]) - 1);
      snake[snakeLength-1] -= B00000001;
      if(getY(snake[snakeLength-1]) == 0){
        gameOver();
        return;
      }
    }

    //checking if we overlapped ourself
    for(byte i=0;i<snakeLength-1;i++){
      if(snake[snakeLength-1] == snake[i]){
        gameOver();
      }
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

