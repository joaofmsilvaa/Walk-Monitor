#include "IMU_SENSORS.h"
#include "SD_ADAPTER.h"

int count = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Initializing system...");

  // Initialize SD card
  if (!begin_sd()) {
    Serial.println("SD card initialization failed. Check connections.");
    while (1);
  }

  // Remove the previous file if it exists and create a new one
  removeFile();
  File dataFile = SD.open(filename, FILE_WRITE);
  if (dataFile) {
    Serial.println("CSV file created successfully!");
    dataFile.println("Time,TiltX,TiltY,Status");
    dataFile.close();
  } else {
    Serial.println("Error creating CSV file!");
    while (1);
  }

  // Initialize IMU sensor
  if (!begin_imu()) {
    Serial.println("IMU initialization failed!");
    while (1);
  }

  Serial.println("System initialized successfully!");
}

void loop() {
  // Read acceleration data
  axis accData = getAcceleration();
  
  // Calculate tilt angles and posture status
  data tiltData = getTilt();

  // Save the data to SD card
  writeData(tiltData.tiltX, tiltData.tiltY, tiltData.status);

  printData();

  delay(500);

  count++;
  if (count >= 10000) {
    Serial.println("Finalizing the data storage process.");
    while (1);
  }
}
