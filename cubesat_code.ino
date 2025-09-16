#include <Wire.h>
#include <SD.h>

// FAN
// Motor Driver Pins (Both Fans Connected Together on M1)
#define SPEED_PIN 3   // PWM control
#define DIR_A_PIN 4   // Direction A
#define DIR_B_PIN 5   // Direction B
#define ENABLE_PIN 6  // Enable pin

void fan_setup() {
  Serial.println("Starting Fans Together...");

  // Set Direction (Clockwise)
  digitalWrite(DIR_A_PIN, HIGH);
  digitalWrite(DIR_B_PIN, LOW);

  // Enable the motor driver
  digitalWrite(ENABLE_PIN, HIGH);

  Serial.println("Fans ON - Full Speed");

  // Run fans at full speed (PWM 255 = max speed)
  analogWrite(SPEED_PIN, 255);
}

//PUMP (sda = 25, scl =24)
#define RELAY_I2C_ADDRESS 0x18
void pump_run() {
  turnRelayOn();
  delay(5000);
  turnRelayOff();
  delay(2000);
}
void turnRelayOn() {
  Wire2.beginTransmission(RELAY_I2C_ADDRESS);
  Wire2.write(0x01); // Command to turn relay ON
  Wire2.endTransmission();
}
void turnRelayOff() {
  Wire2.beginTransmission(RELAY_I2C_ADDRESS);
  Wire2.write(0x00);  // Command to turn relay OFF
  Wire2.endTransmission();
}

// DATA COLLECTION
float collected_data[8];


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

//soil moisture
#define soil_pin 41
float soil_moisture = 0;

//SD card
#define SD_CS_PIN BUILTIN_SDCARD
void SD_run( ){
  File dataFile = SD.open("data.csv");
  
}

void writeToCSV(float data[8]) {
    File file = SD.open("data.csv", FILE_WRITE);
    if (file) {
        file.seek(file.size());
        for (int i = 0; i < 8; i++) {
            file.print(data[i], 6);  // Print float with 6 decimal places

            //Correct me if im wrong, but wouldn't it be <=, now the last 2 elements aren't seperated
            if (i < 6) file.print(",");  // Add a comma except for the last element
        }
        file.println();  // New line for the next entry
        file.close();
        Serial.println("Data written to data.csv");
    } else {
        Serial.println("Failed to open data.csv");
    }
}



//bmp280 (PRESSURE/TEMP/ALTITUDE)
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp(&Wire1);
void bmp_setup() {
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or I2C address!"));
    //while (1) delay(10); // Stop execution if sensor isn't found
  }
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

// LED
// #include <Adafruit_NeoPixel.h>
// #define LED_PIN 9
// #define NUM_LEDS 166
// #define BRIGHTNESS 10
// Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
// void light_setup() {
//   strip.begin();
//   strip.setBrightness(BRIGHTNESS);
//   strip.show();  // Clear all LEDs
// }
// void light_run() {
//   for (int i = 0; i < NUM_LEDS; i++) {
//     // Strong Blue Light with slight Red for photosynthesis support
//     strip.setPixelColor(i, strip.Color(50, 0, 255));  // (Red:50, Green:0, Blue:255)
//   }
//   strip.show();
// }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  Wire2.begin();
  


  //soil moisture
  pinMode(soil_pin, INPUT);

  // light_setup();
  fan_setup();
  CO2_setup();
  bmp_setup();

  // light_run();

  SD.begin(SD_CS_PIN);

  File file = SD.open("data.csv", O_WRITE | O_CREAT | O_TRUNC);  // Overwrites existing file
  if (file) {
    file.println("CO2,TEMP,HUMIDITY,MOISTURE,TEMP_BMP,PRESSURE_BMP,ALT_BMP,TIME(MS)");
    file.close();
    Serial.println(" Header written to data.csv");
  } else {
    Serial.println(" Failed to write header to data.csv");
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  //pump_run();

  soil_moisture = analogRead(soil_pin);
  Serial.println(soil_moisture);

  //light_run();
  CO2_run();
  bmp_run();
  
  collected_data[0] = reading_CO2[0];// CO2, Temp, Humidity in that order
  collected_data[1] = reading_CO2[1];
  collected_data[2] = reading_CO2[2];

  collected_data[3] = soil_moisture;

  collected_data[4] = temperature;
  collected_data[5] = altitude;
  collected_data[6] = pressure_psi;
  unsigned long currentMillis = millis();
  collected_data[7] = (float)currentMillis;
  //collected_data[7] = altitude;
  writeToCSV(collected_data);


  delay(5000);
}
