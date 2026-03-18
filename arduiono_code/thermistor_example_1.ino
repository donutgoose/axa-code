const int thermistorPin = A0;      
const float seriesResistor = 10000.0; 
const float nominalResistance = 10000.0;  
const float nominalTemperature = 25.0;    
const float betaCoefficient = 3950.0;     
const int adcMax = 1023;
const float supplyVoltage = 5.0;
float raw_temp;

void setup() {
  Serial.begin(9600);
}

float readTemperature(int adcValue) {
  float voltage = adcValue * supplyVoltage / adcMax;
  float resistance = (supplyVoltage * seriesResistor / voltage) - seriesResistor;
  float temperatureK = 1.0 / (1.0 / (nominalTemperature + 273.15) + 
                              (1.0 / betaCoefficient) * log(resistance / nominalResistance));
  return temperatureK - 273.15;
}

void loop() {
  raw_temp = analogRead(thermistorPin);
  Serial.println(readTemperature(raw_temp));
  delay(300);
}
