/* Copyright (c) 2021 University of Toronto Hyperloop Team
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/* Code References:
 * 1) Used for sensor setup
 * https://github.com/dawonn/vectornav/blob/master/src/main.cpp
 * 2) Used for data reading and sensor binary output conversion
 * https://github.com/fdcl-gwu/arduino-vn100/blob/master/read-vn100/read-vn100.ino
 * 
 *               *** READ BEFORE TESTING ***
 * This code should allow the arduino to automatically print out sensor values
 * at regular intervals (configure on line 131).
 * Please configure the correct communication port on line 108
 */

// Include this header file to get access to VectorNav sensors.
#include "VN200_Lib_cpp/include/vn/sensors.h"

// We need this file for our sleep function.
#include "VN200_Lib_cpp/include/vn/thread.h"

using namespace std;
using namespace vn::math;
using namespace vn::sensors;
using namespace vn::protocol::uart;
using namespace vn::xplat;

// Function declarations
void read_imu_data(void);
void check_sync_byte(void);
unsigned short calculate_imu_crc(byte data[], unsigned int length);

// Union functions for byte to float conversions
// IMU sends data as bytes, the union functions are used to convert
// these data into other data types

// YawPitchRoll (Attitude)  -> Binary Group 1, Bit Offset 3
union {float f; byte b[4];} yaw;
union {float f; byte b[4];} pitch;
union {float f; byte b[4];} roll;

// AngularRate              -> Binary Group 1, Bit Offset 5
union {float f; byte b[4];} W_x;
union {float f; byte b[4];} W_y;
union {float f; byte b[4];} W_z;

// Position                 -> Binary Group 1, Bit Offset 6
union {double d; byte b[8];} lati;
union {double d; byte b[8];} longi;
union {double d; byte b[8];} alti;

// Velocity                 -> Binary Group 1, Bit Offset 7
union {float f; byte b[4];} v_x;
union {float f; byte b[4];} v_y;
union {float f; byte b[4];} v_z;

// Acceleration             -> Binary Group 1, Bit Offset 8
union {float f; byte b[4];} a_x;
union {float f; byte b[4];} a_y;
union {float f; byte b[4];} a_z;

// MagPres (Mag,Temp,Pres)  -> Binary Group 1, Bit Offset 10
union {float f; byte b[4];} m_x;
union {float f; byte b[4];} m_y;
union {float f; byte b[4];} m_z;
union {float f; byte b[4];} temp;
union {float f; byte b[4];} pres;

// Checksum
union {unsigned short s; byte b[2];} checksum;


// Parameters
bool imu_sync_detected = false;  // check if the sync byte (0xFA) is detected
byte in[100];  // buffer, array to save data send from the IMU, data total 98 bytes


void setup() {

  // Start Serial for printing data to the Serial Monitor
  Serial.begin(115200);

  // Start Serial1 for IMU communication
  Serial1.begin(115200);

  // Create a VnSensor object and connect to sensor
  VnSensor vs;

  // Connect sensor to Arduino
  const string SensorPort = "COM3";                       // ** Adjust as needed **
  const uint32_t SensorBaudrate = 115200;
  vs.connect(SensorPort, SensorBaudrate);

  // Check connectivity
  if(vs.verifySensorConnectivity()) {
    String mn = vs.readModelNumber();
    String fv = vs.readFirmwareVersion();

    Serial.println("Sensor connection established");
    Serial.println("Model Number: " + mn);
    Serial.println("Firmware Version: " + fv);
  }
  else {
    Serial.println("Sensor connection unsuccessful");
  }

  // Make sure no generic async output is registered
  vs.writeAsyncDataOutputType(VNOFF);

  // Sensor Register Configuration
  BinaryOutputRegister bor(
  ASYNCMODE_PORT1,             // Async Port 1 is equivalent to Register 75
  200,                         // IMU Rate: 800(default) / 200 = 4 [Hz]
  COMMONGROUP_YAWPITCHROLL
  | COMMONGROUP_ANGULARRATE
  | COMMONGROUP_POSITION
  | COMMONGROUP_VELOCITY
  | COMMONGROUP_ACCEL
  | COMMONGROUP_MAGPRES,
  TIMEGROUP_NONE,
  IMUGROUP_NONE,
  GPSGROUP_NONE,
  ATTITUDEGROUP_NONE,
  INSGROUP_NONE};
  
  vs.writeBinaryOutput1(bor);   // Sensor should asynchronously output messages at the indicated IMU rate
}

void loop() {
  imu_sync_detected = false;

  // Check if new IMU data is available
  if (Serial1.available() > 4) check_sync_byte();  // Every new binary message is at least 4 bytes long

  // If sync byte is detected, read the rest of the data
  if (imu_sync_detected) read_imu_data();
  
  delay(1);
}

// ===================================================

// Check for the sync byte (0xFA)
void check_sync_byte(void) {
  
  for (int i = 0; i < 6; i++) {  // not sure why 6 specifically, entire function copied from reference
    Serial1.readBytes(in, 1);
    if (in[0] == 0xFA) {
      imu_sync_detected = true;
      break;
    }
  } 
}


// Read the IMU bytes
void read_imu_data(void) {
  Serial1.readBytes(in, 97);

  checksum.b[0] = in[97];
  checksum.b[1] = in[96];

  if (calculate_imu_crc(in, 96) == checksum.s) {
    for (int i = 0; i < 4; i++) {
      yaw.b[i] = in[4 + i];      // Attitude
      pitch.b[i] = in[8 + i];
      roll.b[i] = in[12 + i];
      W_x.b[i] = in[16 + i];     // Angular rate
      W_y.b[i] = in[20 + i];
      W_z.b[i] = in[24 + i];
      lati.b[i] = in[28 + i];    // Position
      lati.b[i+4] = in[32 + i];
      longi.b[i] = in[36 + i];
      longi.b[i+4] = in[40 + i];
      alti.b[i] = in[44 + i];
      alti.b[i+4] = in[48 + i];
      v_x.b[i] = in[52 + i];     // Velcoty
      v_y.b[i] = in[56 + i];
      v_z.b[i] = in[60 + i];
      a_x.b[i] = in[64 + i];     // Acceleration
      a_y.b[i] = in[68 + i];
      a_z.b[i] = in[72 + i];
      m_x.b[i] = in[76 + i];     // Magnetometer
      m_y.b[i] = in[80 + i];
      m_z.b[i] = in[84 + i];
      temp.b[i] = in[88 + i];    // Temperature
      pres.b[i] = in[92 + i];    // Pressure
    }

    Serial.println("Attitude: " + String(yaw.f) + "," + String(pitch.f) + "," + String(roll.f));
    Serial.println("Angular Rate: " + String(W_x.f) + "," + String(W_y.f) + "," + String(W_z.f));
    Serial.println("Position: " + String(lati.d) + "," + String(longi.d) + "," + String(alti.d));
    Serial.println("Velocity: " + String(v_x.f) + "," + String(v_y.f) + "," + String(v_z.f));
    Serial.println("Acceleration: " + String(a_x.f) + "," + String(a_y.f) + "," + String(a_z.f));
    Serial.println("Magnetometer: " + String(m_x.f) + "," + String(m_y.f) + "," + String(m_z.f));
    Serial.println("Temperature: " + String(temp.f));
    Serial.println("Pressure: " + String(pres.f)); 
  }
}


// Calculate the 16-bit CRC for the given ASCII or binary message.
// Function taken from User Manual Section 3.8.3
unsigned short calculate_imu_crc(byte data[], unsigned int length)
{
  unsigned int i;
  unsigned short crc = 0;
  for(i=0; i<length; i++){
    crc = (byte)(crc >> 8) | (crc << 8);
    crc ^= data[i];
    crc ^= (byte)(crc & 0xff) >> 4;
    crc ^= crc << 12;
    crc ^= (crc & 0x00ff) << 5;
  }
  return crc;
}