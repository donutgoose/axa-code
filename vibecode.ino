int pin = 2; // Change pin based on analog, Ex. A#
const int adcMax = 1023; // lowkey forgot what this does
const float seriesResistor = 10000.0; // 10k Ohm resistor
const float nominalResistance = 10000.0; // Thermistor resistance at 25°C
const float nominalTemperature = 25.0;   // Nominal temperature (°C)
const float bCoefficient = 3950.0;       // Beta coefficient of thermistor

void setup() {
  Serial.begin(9600);
}

void loop () {
  int analogValue = analogRead(pin);
  double temp = doSteinhart(adcMax);
  Serial.print("Temperature: ");
  Serial.print(steinhart);
  Serial.println(" °C");

  delay(1000); // Wait 1 second
}

double doSteinhart(adcMax) {
  float voltage = analogValue * 5.0 / adcMax;
  float resistance = (5.0 * seriesResistor / voltage) - seriesResistor;

  // Steinhart-Hart Equation
  float steinhart;
  steinhart = resistance / nominalResistance;     // (R/Ro)
  steinhart = log(steinhart);                     // ln(R/Ro)
  steinhart /= bCoefficient;                      // 1/B * ln(R/Ro)
  steinhart += 1.0 / (nominalTemperature + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                    // Invert
  steinhart -= 273.15;

  return steinhart;
}
