#include <SD.h>
#include <Wire.h>



float data[8];

const int soilPin = 23;  // A9 on Teensy (Pin 23)
float soilValue = 0;

const int chipSelect = BUILTIN_SDCARD;  // Teensy onboard SD card

void setup() {
  Serial.begin(115200);

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
        Serial.println("Failed to open data.csv");
    }
}

// DATA HEADER: Soil moisture, XX, XX, XX, XX, XX, XX, XX

void loop() {
  soilValue = analogRead(soilPin); 

  Serial.print("Analog value: ");
  Serial.println(soilValue);

  data[0] = soilValue;
  data[1] = 2.2;
  data[2] = 3.3;
  data[3] = 4.4;
  data[4] = 5.5;
  data[5] = 6.6;
  data[6] = 7.7;
  data[7] = 8.8;

  writeToCSV(data);
  delay(100);  
}
