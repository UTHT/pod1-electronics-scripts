#include <sensors/spt25_20_0200A.h>

#include <Arduino.h>

#include <utils/base.h>
#include <sensors/sensor.h>

// Data setup
static const char* labels[2] = { 
  "psig (Raw)", 
  "psig" 
};

static const SensorDataSetup datasetup = {
  .numdata = 2,
  .labels = labels
};

SPT25_20_0200A::SPT25_20_0200A(uint8_t pin) : Sensor(SENSOR_SPT25_20_0200A, &datasetup, SPT25_20_0200A_DELTA) {
    this->pin = pin;
}

errorlevel_t SPT25_20_0200A::initialize(void) {
    pinMode(pin, INPUT);
    return ERR_NONE;
}

errorlevel_t SPT25_20_0200A::read(float* data, uint8_t numdata) {
    int val = analogRead(pin);
    data[0] = (float)val;
    data[1] = (float)((val - SPT25_20_0200A_IN_MIN) * (SPT25_20_0200A_OUT_MAX - SPT25_20_0200A_OUT_MIN) / (SPT25_20_0200A_IN_MAX - SPT25_20_0200A_IN_MIN) + SPT25_20_0200A_OUT_MIN);
    // TODO: other error conditions?
    return ERR_NONE;
}