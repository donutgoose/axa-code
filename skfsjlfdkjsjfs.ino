#include <Wire.h>
// #include <Adafruit_VEML7700.h>

// Adafruit_VEML7700 veml = Adafruit_VEML7700();

const int LED = 5;

unsigned long lastToggle = 0;
bool ledState = false;

float lux = 0;
int brightness = 0;

void setup() {
  pinMode(LED, OUTPUT);

  Wire.begin();

  //veml.begin();

  delay(1000); // sensor settle
}

brightness = 0;
brightness_increment = 5;

void loop() {

  // -------- CONTINUOUS LUX READING --------
  //lux = veml.readLux();

  // Map lux to brightness
  // Adjust these numbers depending on your environment
  // brightness = map(lux, 0, 200, 10, 255);
  // brightness = constrain(brightness, 10, 255);



  // -------- LED TOGGLE EVERY 2 SECONDS --------
  //if (millis() - lastToggle >= 2000) {
  //   lastToggle = millis();
  //   ledState = !ledState;
  // }

  // // -------- APPLY OUTPUT --------
  // if (ledState) {
  //   analogWrite(LED, brightness);
  // } else {
  //   analogWrite(LED, 0);
  // }

  
  //delay(1000); // small delay for stability
  analogWrite(LED, brightness);

  brightness += fadeAmount;

  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }

  delay(30);
}
