#include <math.h>

int pin = A9;
const int R_FIXED = 10000; // 10k fixed resistor
const float BETA = 3950.0; // Beta value for thermistor
const float T0 = 298.15;   // 25°C in Kelvin
const float R0 = 10000.0;  // 10kΩ at 25°C

void setup() {
  Serial.begin(9600);
}

float calculateData(int rawData){
    float Vout = rawData * (5.0/1023.0);
    float Rtherm = R_FIXED * (Vout/(5-Vout));

    float tempK = 1.0 / ( (1.0 / T0) + (1.0 / BETA) * log(Rtherm / R0) );
    float tempC = tempK - 273.15;

    return tempC;
}

void loop() {
    int rawData = analogRead(pin);
    float tempC = calculateData(rawData);
    Serial.print("Temperature: ");
    Serial.print(tempC);

    delay(1000);
  }
