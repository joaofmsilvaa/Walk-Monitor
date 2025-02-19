#include "Arduino_BMI270_BMM150.h"
#include <Arduino.h>
#include <math.h>

// Structures to store sensor readings and orientation data
typedef struct {
  float x;
  float y;
  float z;
} axis;

typedef struct {
  float x;
  float y;
  float z;
  float omega_total;
} gyro_data;

typedef struct {
  float tiltX;
  float tiltY;
} data;

// Global variables to hold sensor readings
axis accelReadings;
gyro_data gyroReadings;
data tiltReadings;

// Auxiliary variables for sensor fusion
float angleX = 0, angleY = 0;  // Filtered tilt angles
float dt = 0.02;               // Estimated time interval (20ms)
unsigned long lastTime = 0;

// Function prototypes
bool begin_imu(void);
void getIMUData(void);
data getTilt(void);
void complementaryFilter(void);

// Initialize the IMU sensor
bool begin_imu() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    return false;
  }
  Serial.println("IMU successfully initialized!");
  return true;
}

// Read data from the accelerometer and gyroscope
void getIMUData() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(accelReadings.x, accelReadings.y, accelReadings.z);
  }
  
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyroReadings.x, gyroReadings.y, gyroReadings.z);
  }
}

// Apply the complementary filter to estimate tilt angles
void complementaryFilter() {
  unsigned long currentTime = millis();
  dt = (currentTime - lastTime) / 1000.0; // Compute elapsed time in seconds
  lastTime = currentTime;

  // Calculate tilt angles from the accelerometer using trigonometry
  float accelAngleX = atan2(accelReadings.y, sqrt(accelReadings.x * accelReadings.x + accelReadings.z * accelReadings.z)) * 180 / PI;
  float accelAngleY = atan2(accelReadings.x, sqrt(accelReadings.y * accelReadings.y + accelReadings.z * accelReadings.z)) * 180 / PI;

  // Integrate gyroscope data (convert to degrees per second)
  float gyroRateX = gyroReadings.x * dt;
  float gyroRateY = gyroReadings.y * dt;

  // Apply the complementary filter
  angleX = 0.98 * (angleX + gyroRateX) + 0.02 * accelAngleX;
  angleY = 0.98 * (angleY + gyroRateY) + 0.02 * accelAngleY;
}

// Compute tilt and evaluate posture status
data getTilt() {
  complementaryFilter();
  tiltReadings.tiltX = angleX;
  tiltReadings.tiltY = angleY;

  return tiltReadings;
}

gyro_data getGyroscope() {
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gyroReadings.x, gyroReadings.y, gyroReadings.z);
    
    // Compute the magnitude of angular velocity
    gyroReadings.omega_total = sqrt(gyroReadings.x * gyroReadings.x +
                                    gyroReadings.y * gyroReadings.y +
                                    gyroReadings.z * gyroReadings.z);
  }
  return gyroReadings;
}
