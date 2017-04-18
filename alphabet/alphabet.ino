#include "LedControl.h"

//pin 12 is connected to the DataIn 
//pin 11 is connected to the CLK 
//pin 10 is connected to LOAD 

//LedControl(int dataPin, int clkPin, int csPin, int numDevices);
LedControl lc=LedControl(12,11,10,1);

void setup() {
  //The MAX72XX is in power-saving mode on startup, we have to do a wakeup call   
  lc.shutdown(0,false);
  //Set the brightness to a medium values (0~15 possible values)
  lc.setIntensity(0,1);
  //and clear the display
  lc.clearDisplay(0);
}
#define CHAR_WIDTH 5
#define SP {0, 0, 0, 0, 0}               //Space
#define EX {0, 125, 0, 0, 0}             //Exclamation !
#define A {31, 36, 68, 36, 31}
#define B {127, 73, 73, 73, 54,}
#define C {62, 65, 65, 65, 34}
#define D {127, 65, 65, 34, 28}
#define E {127, 73, 73, 65, 65}
#define F {127, 72, 72, 72, 64}
#define G {62, 65, 65, 69, 38}
#define H {127, 8, 8, 8, 127}
#define I {0, 65, 127, 65, 0}
#define J {2, 1, 1, 1, 126}
#define K {127, 8, 20, 34, 65}
#define L {127, 1, 1, 1, 1}
#define M {127, 32, 16, 32, 127}
#define N {127, 32, 16, 8, 127}
#define O {62, 65, 65, 65, 62}
#define P {127, 72, 72, 72, 48}
#define Q {62, 65, 69, 66, 61}
#define R {127, 72, 76, 74, 49}
#define S {50, 73, 73, 73, 38}
#define T {64, 64, 127, 64, 64}
#define U {126, 1, 1, 1, 126}
#define V {124, 2, 1, 2, 124}
#define W {126, 1, 6, 1, 126}
#define X {99, 20, 8, 20, 99}
#define Y {96, 16, 15, 16, 96}
#define Z {67, 69, 73, 81, 97}
#define ZERO {62, 65, 65, 65, 62}//
#define ONE {33, 127, 1, 0, 0}//
#define TWO {35, 69, 73, 73, 49}//
#define THREE {34, 65, 73, 73, 54}//
#define FOUR {120, 8, 8, 127, 8}//
#define FIVE {121, 73, 73, 73, 6}//
#define SIX {62, 73, 73, 73, 38}//
#define SEVEN {64, 71, 88, 96, 0}//
#define EIGHT {54, 73, 73, 73, 54}//
#define NINE {48, 73, 73, 73, 62}//

byte Alphabet[36][CHAR_WIDTH]={A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
                               ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE};
int len = sizeof(Alphabet)/CHAR_WIDTH;

void loop() { 
  for(int i=26;i<len;i++){
    for(int row=0;row<CHAR_WIDTH;row++)
      lc.setColumn(0,row,Alphabet[i][row]);
    delay(1000);
    lc.clearDisplay(0);
    delay(100);
  }
}
