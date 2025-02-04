#include "Arduino_BMI270_BMM150.h"
#include <Arduino.h>

// Structures to store axis readings and tilt data
typedef struct {
  float x;
  float y;
  float z;
} axis;

typedef struct {
  float tiltX;
  float tiltY;
  char status[4];  // Fixed array to store "Good" or "Bad"
} data;

// Global variables to hold sensor readings
axis axisReadings;
data tiltReadings;

// Function prototypes
bool begin_imu(void);
axis getAcceleration(void);
data getTilt(void);
void printData(void);

// Function to initialize the IMU sensor
bool begin_imu() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    return false;
  }
  Serial.println("IMU initialized successfully!");
  return true;
}

// Function to get acceleration readings from X, Y, and Z axes
axis getAcceleration() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(axisReadings.x, axisReadings.y, axisReadings.z);
  }
  return axisReadings;
}

// Function to calculate tilt angles and determine posture status
data getTilt() {
  // Calculate the tilt angles (pitch) for the X and Y axes
  tiltReadings.tiltX = atan2(axisReadings.y, sqrt(axisReadings.x * axisReadings.x + axisReadings.z * axisReadings.z)) * 180 / PI;
  tiltReadings.tiltY = atan2(axisReadings.x, sqrt(axisReadings.y * axisReadings.y + axisReadings.z * axisReadings.z)) * 180 / PI;

  // Determine posture status based on tilt angles
  strcpy(tiltReadings.status, "Good");
  if (abs(tiltReadings.tiltX) > 12. || tiltReadings.tiltY < 75. || tiltReadings.tiltY > 90.) {
    strcpy(tiltReadings.status, "Bad");
  }

  return tiltReadings;
}

void printData(){
  Serial.print("Time (s): ");
  Serial.print(millis() / 1000.0, 3);
  Serial.print(" | Tilt X: ");
  Serial.print(tiltReadings.tiltX, 2);
  Serial.print(" | Tilt Y: ");
  Serial.print(tiltReadings.tiltY, 2);
  Serial.print(" | Status: ");
  Serial.println(tiltReadings.status);
}