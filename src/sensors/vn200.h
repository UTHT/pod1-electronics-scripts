#ifndef UTHT_SENSORS_VN200_H_
#define UTHT_SENSORS_VN200_H_

#include <Arduino.h>

#include "sensors.h"
#include "thread.h"

#include <sensors/sensor.h>

#define VN200_NAME  "VN200"
#define VN200_DELTA 250
#define VN200_BAUD  115200
#define VN200_PORT  ""

typedef union { unsigned short s; unsigned char b[2]; } VNChecksum;
typedef union { float f; unsigned char b[4]; } VNData;
typedef union { double d; unsigned char b[8]; } VNBigData;

class VN200 : public Sensor {
  public:
  /**
   * Constructor.
   */
    VN200(void);
  private:
    errorlevel_t initialize(void) override;

    errorlevel_t read(float* data, uint8_t numdata) override;

    // Sensor interface
    vn::sensors::VnSensor vn200;

    // Attitude -> Binary Group 1, Bit Offset 3
    VNData yaw, pitch, roll;

    // Angular Rate -> Binary Group 1, Bit Offset 5
    VNData W_x, W_y, W_z;

    // Angular Rate -> Binary Group 1, Bit Offset 5
    VNBigData lati, longi, alti;

    // Velocity -> Binary Group 1, Bit Offset 7
    VNData v_x, v_y, v_z;

    // Acceleration -> Binary Group 1, Bit Offset 8
    VNData a_x, a_y, a_z;

    // Magnetic field -> Binary Group 1, Bit Offset 10
    VNData m_x, m_y, m_z;

    // Temperature and pressure (same offset)
    VNData temp, pres;

    // Checksum
    VNChecksum checksum;
};

#endif