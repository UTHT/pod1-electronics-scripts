#ifndef VN200_H
#define VN200_H

#include "Sensor.h"
#include "VN200_Lib_cpp/include/vn/sensors.h" // Access to VectorNav sensors.
#include "VN200_Lib_cpp/include/vn/thread.h"  // We need this file for our sleep function.

// Parameters
extern bool imu_sync_detected;
extern unsigned char in[100];

class VN200 : public Sensor
{
public:
    VN200(arduino_t arduino);

private:
    errorlevel_t read(t_datum *data, uint8_t numdata) override;
    errorlevel_t init() override;
    void check_sync_byte();
    unsigned short calculate_imu_crc(unsigned char data[], unsigned int length);
    void read_imu_data();
}

#endif