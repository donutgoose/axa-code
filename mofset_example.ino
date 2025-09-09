int pin = 9; // change to whatever

void setup() {
  pinMode(pin,OUTPUT);
}



void loop() {
  analogWrite(pin, 125);
  delay(1000);
  analogWrite(pin,255);
  delay(1000);
  analogWrite(pin,0);
  delay(1000);
}
