#include "LedControl.h"
#include "pitches.h"

//pin 12 is connected to the DataIn 
//pin 11 is connected to the CLK 
//pin 10 is connected to LOAD 
//VCC    is connected to 5V
//LedControl(int dataPin, int clkPin, int csPin, int numDevices);
LedControl lc=LedControl(12,11,10,1);

//digital pin that button is connected to (other end is gnd)
int buttonPin = 7;
//button that is used to toggle brightness 1, 4, 8
int brightPin = 6; 
int brightness = 1;

// notes in the melody:
int melody[] = {
  NOTE_D4, NOTE_F3, NOTE_B3, NOTE_F3,      // D4-F3-B3-F3
  NOTE_D4, NOTE_GS3, NOTE_B3, NOTE_GS3,    // D4-Ab3-B3-Ab3
  NOTE_G4, NOTE_B3, NOTE_E4, NOTE_B3,      // G4-B3-E4-B3
  NOTE_E4, NOTE_A3, NOTE_CS4, NOTE_A3,     // E4-A3-C#4-A3
  NOTE_D4, NOTE_FS4, NOTE_B3, NOTE_FS4,    // D4-F#4-B3-F#4
  NOTE_D4, NOTE_GS3, NOTE_B3, NOTE_GS3,    // D4-Ab3-B3-Ab3
  NOTE_G4, NOTE_B3, NOTE_E4, NOTE_B3,      // G4-B3-E4-B3
  NOTE_E4, NOTE_A3, NOTE_CS4, NOTE_A3,     // E4-A3-C#-A3
};

int outputPin     = 8;
int maxChords     = 4;
int maxRepeats    = 2;         // every chord will be repeated 2 times
int noteDuration  = 1000/8;    // 4 = quarter note, 8 = eight note, etc.

// notes in the melody:
int sad[] = {NOTE_C5, NOTE_B4, NOTE_AS4, NOTE_A4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int sadDurations[] = {4, 4, 4, 1};

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
int aLen = sizeof(Alphabet)/CHAR_WIDTH;

byte numbers[10][CHAR_WIDTH]={ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE};
int nLen = sizeof(numbers)/CHAR_WIDTH;

byte iD[7] = {B00011100, B00010010, B10010001, B00010001, B10010001, B10010010, B10011100};
byte arrow[8] = {B00011000, B00011000, B00011000, B10011001, B11011011, B01111110, B00111100, B00011000};
byte smile[6] = {B01100110, B01100110, B00000000, B00000000, B01000010, B00111100};
byte frown[6] = {B01100110, B01100110, B00000000, B00000000, B00111100, B01000010};
byte jackpot[6] = {B01100110, B01100110, B00000000, B01111110, B01111110, B00111100};
byte ten[8] = {B01000110, B11001001, B01001001, B01001001, B01001001, B11100110, B00000000, B11111111};

unsigned long idleTimer = 0;
bool flip = true;

void setup() {
  //Serial.begin(9600);//REMOVE ME
  //The MAX72XX is in power-saving mode on startup, we have to do a wakeup call   
  lc.shutdown(0,false);
  //Set the brightness to a medium values (0~15 possible values)
  lc.setIntensity(0,brightness);
  //and clear the display
  lc.clearDisplay(0);
  //seeding random number generator
  randomSeed(analogRead(0));
  //initializing the buttons
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(brightPin, INPUT_PULLUP);
  idle(!flip);
}

void loop() { 
  if(digitalRead(buttonPin) == LOW){
    spinWheel();
  }
  if(digitalRead(brightPin) == LOW){
    if(brightness == 1) brightness = 4;
    else if(brightness == 4) brightness = 8;
    else brightness = 1;
    lc.setIntensity(0,brightness);
    delay(300);
  }
  if(idleTimer == 250000){
    idle(flip);
    flip = !flip;
    idleTimer = 0;
  }
  idleTimer++;
}

void playLossSound(){
   // iterate over the notes of the "fail" melody:
  for (int thisNote = 0; thisNote < 4; thisNote++) {
    // to calculate the note duration, take one second
    // divided by the note type.
    // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/sadDurations[thisNote];
    tone(outputPin, sad[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(outputPin);
  }
}

void playWinSound() {
  // play all 8 chords
  for (int chords = 0; chords < maxChords; chords++) {
    for (int chordTone = 0; chordTone < 4; chordTone++) {
      for (int repeats = 0; repeats < maxRepeats; repeats++) {
        tone(outputPin, melody[chords*maxRepeats + repeats], noteDuration);
        // set a delay between the notes
        delay(noteDuration);
        // stop the tone playing
        noTone(outputPin);
      }
    }
  }
}


void playJackpotSound(){
  for(int i=100;i<4000;i+=10){
    tone(outputPin, i);
    delay(10);
  }
  for(int i=0;i<20;i++){
    tone(outputPin, 500 + (i%2 * 200),100);
    delay(100);
  }
  noTone(outputPin);
}

//if you pass true it shows iD, if not it shows an arrow pointing to the button
void idle(bool logo) {
  if(logo){
    lc.clearDisplay(0);
    for(int i=0; i<7; i++){
      lc.setRow(0,i+1,iD[i]);
    }
  }
  else{
    lc.clearDisplay(0);
    for(int i=0; i<8; i++){
      lc.setRow(0,i,arrow[i]);
    }
  }
}

void spinWheel(){
  int offset = 0;
  int spinSpeed = 30; //pixels per tick
  byte wheel[2][6];
  int rounds = 0;//amount of faces that go by
  int roll;
  
  size_t note = 200;//frequency of note during spin
  
  if(random(3) == 0)//33% chance of a smile passing
    for(int i=0; i<6; i++)
      wheel[0][i] = smile[i];
  else
    for(int i=0; i<6; i++)
      wheel[0][i] = frown[i];

  if(random(3) == 0)//33% chance of a smile passing
    for(int i=0; i<6; i++)
      wheel[1][i] = smile[i];
  else
    for(int i=0; i<6; i++)
      wheel[1][i] = frown[i];
    
  while(rounds < 10){
    if(offset == 8){
      offset = 0;
      rounds++;
      spinSpeed += 10;
      for(int i=0; i<6; i++)//moving face that is on screen back to origin
        wheel[0][i] = wheel[1][i];
      roll = random(100);
      if(roll >= 66)//34% chance of a smile passing
        for(int i=0; i<6; i++)
          wheel[1][i] = smile[i];
      else if(roll > 0)//65% chance of frown passing
        for(int i=0; i<6; i++)
          wheel[1][i] = frown[i];
      else//1% chance of jackpot passing
        for(int i=0; i<6; i++)
            wheel[1][i] = jackpot[i];
    }
    

    for(int i=0; i<6; i++)//face that starts on screen
      lc.setRow(0, i - offset + 1, wheel[0][i]);
    for(int i=0; i<6; i++)//face that starts off the bottom of screen
      lc.setRow(0, i + 8 - offset + 1, wheel[1][i]);
      
    offset++;
    tone(outputPin,note,spinSpeed);
    if(note == 200) note = 300;
    else note = 200;
    delay(spinSpeed);
    lc.clearDisplay(0);
  }
 
  for(int j=0; j<4; j++){
    for(int i=0; i<6; i++)//display the face they rolled
        lc.setRow(0, i + 1, wheel[0][i]);
    delay(100);
    lc.setRow(0,0,B11111111);
    lc.setRow(0,7,B11111111);
    lc.setColumn(0,0,B11111111);
    lc.setColumn(0,7,B11111111);
    delay(100);
    lc.setRow(0,0,B00000000);
    lc.setRow(0,7,B00000000);
    lc.setColumn(0,0,B00000000);
    lc.setColumn(0,7,B00000000);
  }
  
  if(wheel[0][4] == B01000010){
    playWinSound();
    delay(600);
    lc.clearDisplay(0);
    size_t randNum = random(1,4);//amount of tickets they won
    for(int i=0;i<5;i++){
      lc.setColumn(0,i,numbers[randNum][i]);
    }
    //buzz the amount of tickets they won
    for(int i=0;i<randNum;i++){
      tone(outputPin,400,300);//400Hz for 300 ms
      delay(600);
    }
    delay(2000);//stay on ticket payout screen for 2 seconds
  }
  else if(wheel[0][3] == B01111110){
    playJackpotSound();
    delay(600);
    lc.clearDisplay(0);
    for(int i=0;i<8;i++)
      lc.setRow(0,i,ten[i]);
    //buzz ten times
    for(int i=0;i<10;i++){
      tone(outputPin,400,300);
      delay(600);
    }
    delay(2000);//stay on ticket payout screen for 2 seconds
  }
  else playLossSound();
}


