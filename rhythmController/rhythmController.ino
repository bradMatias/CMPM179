#include <Keyboard.h>

//1=gnd 2=analogOut 3=5volt

byte b1Pin = 1;
byte b2Pin = 2;
byte b3Pin = 3;
byte b4Pin = 4;

byte led1Pin = 10;
byte led2Pin = 11;
byte led3Pin = 12;
byte led4Pin = 13;

bool d = false;
bool f = false;
bool j = false;
bool k = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Keyboard.begin();
  pinMode(b1Pin, INPUT_PULLUP);
  pinMode(b2Pin, INPUT_PULLUP);
  pinMode(b3Pin, INPUT_PULLUP);
  pinMode(b4Pin, INPUT_PULLUP);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  pinMode(led4Pin, OUTPUT);
}

void loop() {
  //button 1 input 'd'
  if(digitalRead(b1Pin) == LOW){
    Keyboard.press('d');
    if(!d){
      digitalWrite(led1Pin,HIGH);
    }
    d = true;
  }else if(d){
    Keyboard.release('d');
    digitalWrite(led1Pin,LOW);
    d = false;
  }

  //button 2 input 'f'
  if(digitalRead(b2Pin) == LOW){
    Keyboard.press('f');
    if(!f){
      digitalWrite(led2Pin,HIGH);
    }
    f = true;
  }else if(f){
    Keyboard.release('f');
    digitalWrite(led2Pin,LOW);
    f = false;
  }  

  //button 3 input 'j'
  if(digitalRead(b3Pin) == LOW){
    Keyboard.press('j');
    if(!j){
      digitalWrite(led3Pin,HIGH);
    }
    j = true;
  }else if(j){
    Keyboard.release('j');
    digitalWrite(led3Pin,LOW);
    j = false;
  }  

  //button 4 input 'k'
  if(digitalRead(b4Pin) == LOW){
    Keyboard.press('k');
    if(!k){
      digitalWrite(led4Pin,HIGH);
    }
    k = true;
  }else if(k){
    Keyboard.release('k');
    digitalWrite(led4Pin,LOW);
    k = false;
  }
}
