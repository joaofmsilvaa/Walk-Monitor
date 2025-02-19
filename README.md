# Walk Monitor

## 📖 Project Overview
This project aims to monitor posture using an **Arduino Nano 33 BLE Sense Rev2**, equipped with an Inertial Measurement Unit (IMU) sensor. The system logs tilt data to an SD card, which can be analyzed later for posture improvement. It provides real-time feedback via the serial monitor.

---

## 📋 Materials Needed

To build this project, you'll need the following components:

### Components List:
- **1x Arduino Nano 33 BLE Sense Rev2**  
  - Microcontroller with built-in IMU sensor.
- **1x Micro SD Card Adapter**  
  - Used to store collected posture data.
- **1x Micro SD Card**  
  - Recommended size: 8GB or higher (FAT32 format).
- **1x Power Supply**  
  - USB power source or battery pack for portability.
- **1x 9V Battery**  
  - To power the system independently.

---

## 🛠 Setup Instructions

### 1. Circuit Connections

Follow these steps to wire the components:

#### **Micro SD Card Adapter to Arduino Nano 33 BLE Sense**
| SD Adapter Pin | Arduino Pin |
|----------------|-------------|
| VCC            | 5V         |
| GND            | GND          |
| CS             | D10           |
| MOSI           | D11 (MOSI)    |
| MISO           | D12 (MISO)    |
| SCK            | D13 (SCK)     |

#### **Powering the System:**
- Connect the **9V battery** to the Arduino's VIN and GND pins.
- Alternatively, power via USB from a 5V power source.

---

### 2. Software Setup

#### **Required Tools:**
1. Download and install the latest version of the [Arduino IDE](https://www.arduino.cc/en/software).
2. Install the necessary libraries via **Library Manager** (`Sketch > Include Library > Manage Libraries`):
   - `Arduino_LSM9DS1` (IMU sensor handling)
   - `Arduino_BMI270_BMM150` (IMU sensor handling)
   - `SD` (SD card operations)

#### **Project Code Structure:**
Ensure the following files are present in your project folder:

```
/Walk_Monitor
├── IMU_SENSORS.h     # Handles IMU sensor readings
├── SD_CARD.h         # Handles SD card operations
├── main.ino          # Main Arduino code
```

---

## 🚀 Running the Project

1. Connect the Arduino to your computer via USB.
2. Open `main.ino` in the Arduino IDE.
3. Select the correct board settings:
   - **Board:** "Arduino Nano 33 BLE"
   - **Port:** Select the correct COM port.
4. Upload the code to the Arduino.
5. Open the serial monitor to check real-time posture data.

---

## 📊 Expected Output

When the system is running correctly, you should see output similar to:

```
Initializing system...
SD card initialized successfully!
IMU initialized successfully!
Time,Ax,Ay,Az,Gx,Gy,Gz,TiltX,TiltY,OmegaTotal
26.251,-5.85,0.22,-0.50,0.97,0.02,0.18,-4.78,1.76,0.99
27.048,1.40,-1.45,-2.90,0.98,-0.01,0.21,-3.85,1.34,3.45
...
```

The data will also be saved to the `posture.csv` file on the SD card.

---

## 🔋 Powering the System

Once the system is tested and ready for deployment:

1. Disconnect the USB cable and connect the 9V battery.
2. The system will automatically start logging data to the SD card.

---

## 🛡 Troubleshooting

| Issue                  | Possible Solution                                      |
|------------------------|--------------------------------------------------------|
| SD card not detected   | Check wiring and format the SD card to FAT32.          |
| No IMU data recorded   | Ensure the Arduino Nano 33 BLE Sense is recognized.    |
| Power issues           | Use a fresh 9V battery or check USB power source.      |


Happy building! 🚀

