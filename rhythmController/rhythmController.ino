//1=gnd 2=analogOut 3=5volt
void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(A5));
  delay(50);
}
