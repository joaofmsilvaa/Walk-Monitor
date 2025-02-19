#include <SPI.h>
#include <SD.h>

// File name to store posture data
const char filename[] = "posture.csv";

// Function prototypes
bool begin_sd(void);
void writeData(float ax, float ay, float az, float gx, float gy, float gz, float tiltX, float tiltY);
void removeFile(void);
bool fileExists(void);

// Function to initialize the SD card
bool begin_sd() {
  const int chipSelect = 10;  // Define the CS (chip select) pin

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return false;
  }
  Serial.println("SD card initialized successfully!");

  // Check if the file exists; if not, create it and add the header
  if (!fileExists()) {
    File dataFile = SD.open(filename, FILE_WRITE);
    if (dataFile) {
      dataFile.println("Time(s),Ax,Ay,Az,Gx,Gy,Gz,TiltX,TiltY,OmegaTotal");
      dataFile.close();
      Serial.println("CSV file created with header.");
    } else {
      Serial.println("Error creating CSV file!");
      return false;
    }
  }

  return true;
}

// Function to write data to the CSV file
void writeData(float accX, float accY, float accZ, float gyroX, float gyroY, float gyroZ, float tiltX, float tiltY, float omega_total) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.print(millis() / 1000.0, 3);  // Time in seconds
    dataFile.print(",");
    dataFile.print(gyroX, 2);
    dataFile.print(",");
    dataFile.print(gyroY, 2);
    dataFile.print(",");
    dataFile.print(gyroZ, 2);
    dataFile.print(",");
    dataFile.print(accX, 2);
    dataFile.print(",");
    dataFile.print(accY, 2);
    dataFile.print(",");
    dataFile.print(accZ, 2);
    dataFile.print(",");
    dataFile.print(tiltX, 2);
    dataFile.print(",");
    dataFile.print(tiltY, 2);
    dataFile.println(omega_total, 2);
    dataFile.close();
    Serial.println("Data successfully saved!");
  } else {
    Serial.println("Error writing to SD card!");
  }
}


// Function to remove the CSV file if it exists
void removeFile() {
  if (SD.exists(filename)) {
    SD.remove(filename);
    Serial.println("File removed successfully.");
  } else {
    Serial.println("File does not exist.");
  }
}

// Function to check if the CSV file exists
bool fileExists() {
  return SD.exists(filename);
}
