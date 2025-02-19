/* 
 * Walk-Monitor
 * 
 * Version: 1.1
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
  if (USE_SDCARD) {
    if (!begin_sd()) {
      Serial.println("SD card initialization failed. Check connections.");
    }

    // Remove previous file if it exists
    removeFile();
  }

  // Initialize IMU sensor
  if (!begin_imu()) {
    Serial.println("IMU initialization failed!");
  }

  Serial.println("System initialized successfully!");
}

void loop() {
  // Read accelerometer and gyroscope data
  getIMUData();
  axis accData = accelReadings;

  // Calculate tilt angles and posture status
  data tiltData = getTilt();

  gyro_data gyroData = getGyroscope();

  // Print data to Serial Monitor
  Serial.print("Time (s): ");
  Serial.print(millis());
  Serial.print(" | Ax: ");
  Serial.print(accData.x, 2);
  Serial.print(" | Ay: ");
  Serial.print(accData.y, 2);
  Serial.print(" | Az: ");
  Serial.print(accData.z, 2);
  Serial.print(" | Gx: ");
  Serial.print(gyroData.x, 2);
  Serial.print(" | Gy: ");
  Serial.print(gyroData.y, 2);
  Serial.print(" | Gz: ");
  Serial.print(gyroData.z, 2);
  Serial.print(" | Tilt X: ");
  Serial.print(tiltData.tiltX, 2);
  Serial.print(" | Tilt Y: ");
  Serial.print(tiltData.tiltY, 2);
  Serial.print(" | Angular Velocity Module: ");
  Serial.println(gyroData.omega_total, 2);

  // Save data to SD card
  if (USE_SDCARD) {
    writeData(accData.x, accData.y, accData.z, gyroData.x, gyroData.y, gyroData.z, tiltData.tiltX, tiltData.tiltY,gyroData.omega_total);
    delay(700);
  }

}
