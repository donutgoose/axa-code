
#include <Wire.h>
#include <Adafruit_VEML7700.h>

Adafruit_VEML7700 sensor;

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  if (!sensor.begin()) {
    Serial.println("VEML7700 not found!");
    while (1);
  }
  
  // Optional configuration
  sensor.setGain(VEML7700_GAIN_1);         // other options: GAIN_2, GAIN_1_8, GAIN_1_4
  sensor.setIntegrationTime(VEML7700_IT_100MS);  // etc.

  // Optionally set interrupt thresholds
  sensor.setLowThreshold(1000);
  sensor.setHighThreshold(10000);
  sensor.interruptEnable(true);
}

void loop() {
  uint16_t raw = sensor.readALS();       // ambient light raw
  uint16_t white = sensor.readWhite();   // white channel
  float lux = sensor.readLux();          // computed lux

  Serial.print("Raw ALS: "); Serial.println(raw);
  Serial.print("White: "); Serial.println(white);
  Serial.print("Lux: "); Serial.println(lux, 2);

  delay(500);
}
