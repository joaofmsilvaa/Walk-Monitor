#include "Arduino_BMI270_BMM150.h"
#include <SPI.h>
#include <SD.h>

const int chipSelect = 4; // CS module PIN
int count = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.print("Initializing the SD card... ");
  if (!SD.begin(chipSelect)) {
    Serial.println("There was an error in the initialization!");
    Serial.println("1. Verify the connections");
    Serial.println("2. Check if the SD Card is well connected");
    while (1);
  }
  Serial.println("The SD card has been initialized successfully!");

  // Clears the data file before inserting the new data
  if (SD.exists("posture.csv")) {
    SD.remove("posture.csv");
  }

  // Creates the .CSV file and adds the needed headers
  File dataFile = SD.open("posture.csv", FILE_WRITE);
  if (dataFile) {
    Serial.println("CSV file created successfully!");
    dataFile.println("Time,TiltX,TiltY,Status");
    dataFile.close();
  } else {
    Serial.println("An unexpected error occured while creating the CSV file!");
    while (1);
  }

  // Starts the IMU Sensor
  if (!IMU.begin()) {
    Serial.println("There was an error in the initialization of the IMU!");
    while (1);
  }
  Serial.println("The IMU has been initialized successfully!");
}

void loop() {
  float accX, accY, accZ;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accX, accY, accZ);

    // Calculates the Pitch (tilt angle in the X and Y axis)
    float tiltX = atan2(accY, sqrt(accX * accX + accZ * accZ)) * 180 / PI;
    float tiltY = atan2(accX, sqrt(accY * accY + accZ * accZ)) * 180 / PI;

    String status = "Good";
    if (abs(tiltX) < -12. || abs(tiltX) > 12. || abs(tiltY) < 75. || abs(tiltY) > 90.) {
      status = "Bad";
    }

    File dataFile = SD.open("posture.csv", FILE_WRITE);
    if (dataFile) {
      dataFile.print(millis() / 1000.0, 3); // Time in seconds
      dataFile.print(",");
      dataFile.print(tiltX, 2);
      dataFile.print(",");
      dataFile.print(tiltY, 2);
      dataFile.print(",");
      dataFile.println(status);
      dataFile.close();
      Serial.println("The data has been successfully saved!");
    } else {
      Serial.println("An error occurred while saving the data!");
    }

    Serial.print("Time (s): ");
    Serial.print(millis() / 1000.0, 3);
    Serial.print(" | Tilt X: ");
    Serial.print(tiltX, 2);
    Serial.print(" | Tilt Y: ");
    Serial.print(tiltY, 2);
    Serial.print(" | Status: ");
    Serial.println(status);
  }

  delay(500);

  count += 1;
  if (count >= 150) {
    Serial.println("Finalizing the data storage process.");
    while (1);
  }
}
