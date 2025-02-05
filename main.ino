/* 
 * Walk-Monitor
 * 
 * Version: 1.0
 * 
 * Applied Physics - Computer Science
 * 
 * Academic year 2024-25
 * 
 * João Silva | 24284@stu.ipbeja.pt
 * 
 * Hardware: Arduino Nano 33 BLE Sense Rev2
 *           Breadboard
 *           Power-supply
 *           9V battery
 *           Micro SD card adapter
 *           Micro SD (4GB)
 * 
 * Description: This project aims to monitor posture using an Arduino Nano 33 BLE Sense Rev2, 
 *              equipped with an Inertial Measurement Unit (IMU) sensor. The system records the tilt data on an 
 *              SD card, which can then be analysed to improve posture. It provides feedback in 
 *              real time via the serial monitor.
 * 
 * Resources: IMU sensor documentation: https://docs.arduino.cc/libraries/arduino_bmi270_bmm150/
 *            SD library documentation: https://docs.arduino.cc/libraries/sd/
 *             
 * 
 * CC-BY-SA 4.0
 */

#include "IMU_SENSORS.h"
#include "SD_ADAPTER.h"

int count = 0;
int MAX_RECORDS = 10000;
int USE_SDCARD = 1;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Initializing system...");

  // Initialize SD card
  if(USE_SDCARD){
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

  if (USE_SDCARD) {
    // Save the data to SD card
    writeData(tiltData.tiltX, tiltData.tiltY, tiltData.status);

    delay(1000);
  }

  printData();

}
