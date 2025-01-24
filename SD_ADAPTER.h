#include <SPI.h>
#include <SD.h>

// File name to store posture data
const char filename[] = "posture.csv";

// Function prototypes
bool begin_sd(void);
void writeData(float x, float y, const char status[10]);
void removeFile(void);
bool fileExists(void);

// Function to initialize the SD card
bool begin_sd() {
  const int chipSelect = 4;  // Define the CS (chip select) pin
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return false;
  }
  Serial.println("SD card initialized successfully!");
  return true;
}

// Function to write data to the CSV file
void writeData(float x, float y, const char status[10]) {
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    dataFile.print(millis() / 1000.0, 3);  // Time in seconds
    dataFile.print(",");
    dataFile.print(x, 2);
    dataFile.print(",");
    dataFile.print(y, 2);
    dataFile.print(",");
    dataFile.println(status);
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