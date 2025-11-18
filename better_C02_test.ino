
// CO2
#include "SparkFun_SCD4x_Arduino_Library.h"
SCD4x CO2sensor;
float reading_CO2[3];
void CO2_setup() {
  if (!CO2sensor.begin(Wire)) {  // Use default Wire (Pins 18 & 19)
    Serial.println("SCD40 not detected. Check wiring.");
    //while (1);  // Halt execution if sensor is not found
  }
  Serial.println("SCD40 Sensor Initialized.");
}



void CO2_run() {
  if (CO2sensor.readMeasurement()) {  // Read sensor data
    reading_CO2[0] = CO2sensor.getCO2();
    reading_CO2[1] = CO2sensor.getTemperature();
    reading_CO2[2] = CO2sensor.getHumidity();

    Serial.print("CO2: ");
    Serial.print(reading_CO2[0]);
    Serial.print(" ppm | Temp: ");
    Serial.print(reading_CO2[1]);
    Serial.print(" °C | Humidity: ");
    Serial.print(reading_CO2[2]);
    Serial.println(" %");
  } else {
    Serial.println("Sensor not ready.");
  }
} 

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire2.begin();

  CO2_setup();
}

void loop() {
  CO2_run();
}
