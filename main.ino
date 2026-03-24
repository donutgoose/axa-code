#include <SD.h>
#include <Wire.h>
#include <SparkFun_SCD4x_Arduino_Library.h>
#include <Adafruit_BMP280.h>

SCD4x co2sensor;


float data[8];

const int soilPin = 23;  // A9 on Teensy (Pin 23)
float soilValue = 0;

Adafruit_BMP280 bmp;
void bmp_setup() {
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find BMP280 at 0x76, trying 0x77..."));
    if (!bmp.begin(0x77)) {
        Serial.println(F("Could not find BMP280 at 0x77 either!"));
        // Halt
    }
}
Serial.println("if no other error printed, BMP sstart is successful!");
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time */
}
float temperature;
float pressure_hPa;
float pressure_psi;
float altitude;
void bmp_run() {
  temperature = bmp.readTemperature();
  pressure_hPa = bmp.readPressure() / 100.0F;  // Read pressure in hPa
  pressure_psi = pressure_hPa * 0.0145038;     // Convert hPa to PSI
  altitude = bmp.readAltitude(1013.25);        // Adjust according to local pressure

  Serial.print(F("Temperature = "));
  Serial.print(temperature);
  Serial.println(F(" *C"));

  Serial.print(F("Pressure = "));
  Serial.print(pressure_psi);
  Serial.println(F(" PSI"));

  Serial.print(F("Approx. Altitude = "));
  Serial.print(altitude);
  Serial.println(F(" m"));

  Serial.println();
  //delay(2000); // Wait 2 seconds before next reading
}


const int chipSelect = BUILTIN_SDCARD;  // Teensy onboard SD card

void CO2_setup() {
  delay(500);
  if (!co2sensor.begin(Wire)) {  // Use default Wire (Pins 18 & 19)
    Serial.println("SCD40 not detected. Check wiring.");
    //while (1);  // Halt execution if sensor is not found
  } else {
    Serial.println("SCD40 Sensor Initialized.");
  }
  co2sensor.stopPeriodicMeasurement();
  co2sensor.startPeriodicMeasurement();
  }
 
float reading_CO2[3];
void CO2_run() {
  if (co2sensor.readMeasurement()) {  // Read sensor data
    reading_CO2[0] = co2sensor.getCO2();
    reading_CO2[1] = co2sensor.getTemperature();
    reading_CO2[2] = co2sensor.getHumidity();

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
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);
  CO2_setup();
  bmp_setup();
  SD.begin(chipSelect);
  File file = SD.open("data.csv", O_WRITE | O_CREAT | O_TRUNC);  // Overwrites existing file
  if (file) {
    file.println("SOIL,???,????,dih");
    file.close();
    Serial.println(" Header written to data.csv");
  } else {
    Serial.println(" Failed to write header to data.csv");
  }

}

void writeToCSV(float data[8]) {
    File file = SD.open("data.csv", FILE_WRITE);
    if (file) {
        file.seek(file.size());
        for (int i = 0; i < 8; i++) {
            file.print(data[i], 6);  // Print float with 6 decimal places

            //Correct me if im wrong, but wouldn't it be <=, now the last 2 elements aren't seperated
            if (i < 7) file.print(",");  // Add a comma except for the last element
        }
        file.println();  // New line for the next entry
        file.close();
        Serial.println("Data written to data.csv");
    } else {
        //Serial.println("Failed to open data.csv");
    }
}

// DATA HEADER: Soil moisture, XX, XX, XX, XX, XX, XX, XX

void loop() {
  soilValue = analogRead(soilPin); 

  Serial.print("Analog value: ");
  Serial.println(soilValue);

  // CO2
  CO2_run();
  bmp_run();

  data[0] = reading_CO2[0];// CO2, Temp, Humidity in that order
  data[1] = reading_CO2[1];
  data[2] = reading_CO2[2];
  data[3] = soilValue;
  data[4] = temperature;
  data[5] = altitude;
  data[6] = pressure_psi;
  data[7] = 8.8;
  Serial.println("");

  writeToCSV(data);
  delay(5001);  
}
