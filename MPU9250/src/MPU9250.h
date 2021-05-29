#ifndef MPU9250_H
#define MPU9250_H

#include "Sensor.h"
#include "MPU9250_Lib.h"
#include "quaternionFilters.h" // Currently Unused.
#include "arduinoFFT.h"
#include "Wire.h"
#define AHRS false          // Set to false for basic data read
#define SerialDebug true    // Set to true to get Serial output for debugging
#define I2Cclock 400000     // Options: 100k, 400k
#define I2Cport Wire
#define MPU9250_ADDRESS 104 // I2C Address: 0x68
#define ENABLE_FFT true     // Set to true for FFT calculations
#define FFT_AXIS 'x'        // Can set FFT_AXIZ to 'x', 'y', or 'z'
#define SAMPLES 256         // Must be a power of 2
#define SAMPLING_FREQUENCY 1000

// FFT
extern arduinoFFT FFT;
extern double vReal[SAMPLES];
extern double vImag[SAMPLES];
extern int count;
extern double peak;
extern double vibrationFreq;

class MPU9250 : public Sensor {
    public:
        MPU9250(arduino_t arduino);
    private:
        MPU9250_Lib mpu9250;
        errorlevel_t read(t_datum* data, uint8_t numdata) override;
        errorlevel_t init() override;
        /**
         * Analog input pin.
         * */
        uint8_t pin;
};

#endif