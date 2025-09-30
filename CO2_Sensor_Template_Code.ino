#include <Wire.h>
#include "SparkFun_SCD4x_Arduino_Library.h"  // Install via Library Manager

SCD4x mySensor;  // Create sensor object

void setup() {
  Serial.begin(9600);
  Wire.begin();

  Serial.println("Starting SCD41 CO2 Sensor...");

  if (!mySensor.begin()) {
    Serial.println("Sensor not detected. Check wiring!");
    while (1); // Halt if sensor not found
  }

  if (mySensor.startPeriodicMeasurement()) {
    Serial.println("Measurement started.");
  } else {
    Serial.println("Could not start measurement!");
  }
}

void loop() {
  // The SCD41 updates about once every 5 seconds
  if (mySensor.readMeasurement()) {
    float co2 = mySensor.getCO2();  // <-- Correct accessor
    if (co2 == 0) {
      Serial.println("Invalid sample, skipping...");
    } else {
      Serial.print("CO2 (ppm): ");
      Serial.println(co2);
    }
  }
  delay(2000);  // check every 2s
}
