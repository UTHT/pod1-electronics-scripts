#include <sensors/rtd.h>

#include <Arduino.h>

#include <utils/base.h>
#include <sensors/sensor.h>

// Data setup
static const char* labels[2] = { 
  "deg C (Raw)", 
  "deg C" 
};
static const SensorDataSetup datasetup = {
  .numdata = 2,
  .labels = labels
};

RTD::RTD(uint8_t pin) : Sensor(SENSOR_RTD, &datasetup, RTD_DELTA) {
    this->pin = pin;
}

errorlevel_t RTD::initialize(void) {
    pinMode(pin, INPUT);
    return ERR_NONE;
}

errorlevel_t RTD::read(float* data, uint8_t numdata) {
    float val = analogRead(pin);
    data[0] = val;
    data[1] = (val - RTD_IN_MIN) * (RTD_OUT_MAX - RTD_OUT_MIN) / (RTD_IN_MAX - RTD_IN_MIN) + RTD_OUT_MIN;
    // TODO: other error conditions?
    return ERR_NONE;
}