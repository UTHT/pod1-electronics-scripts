#include <sensors/sptd25_20_1000h.h>

#include <Arduino.h>

#include <utils/base.h>
#include <sensors/sensor.h>

// Data setup
static const char* labels[2] = { 
  "kPa (Raw)", 
  "kPa" 
};

static const SensorDataSetup datasetup = {
  .numdata = 2,
  .labels = labels
};

SPTD25_20_1000H::SPTD25_20_1000H(uint8_t pin) : Sensor(SENSOR_SPTD25_20_1000H, &datasetup, SPTD25_20_1000H_DELTA) {
    this->pin = pin;
}

errorlevel_t SPTD25_20_1000H::initialize(void) {
    pinMode(pin, INPUT);
    return ERR_NONE;
}

errorlevel_t SPTD25_20_1000H::read(float* data, uint8_t numdata) {
    int val = analogRead(pin);
    data[0] = (float)val;
    data[1] = (float)((val - SPTD25_20_1000H_IN_MIN) * (SPTD25_20_1000H_OUT_MAX - SPTD25_20_1000H_OUT_MIN) / (SPTD25_20_1000H_IN_MAX - SPTD25_20_1000H_IN_MIN) + SPTD25_20_1000H_OUT_MIN);
    // TODO: other error conditions?
    return ERR_NONE;
}