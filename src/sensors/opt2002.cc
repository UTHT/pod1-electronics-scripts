#include <sensors/opt2002.h>

#include <Arduino.h>

#include <utils/base.h>
#include <sensors/sensor.h>

// Data setup
static const char* labels[2] = { "mm (Raw)", "mm" };
static const SensorDataSetup datasetup = {
  .numdata = 2,
  .labels = labels
};

OPT2002::OPT2002(uint8_t pin) : Sensor(SENSOR_OPT2002, &datasetup, OPT2002_DELTA){
    this->pin = pin;
}

errorlevel_t OPT2002::initialize(void) {
    pinMode(pin, INPUT);     // To use the internal resistor in the arduino
    return ERR_NONE;
}

errorlevel_t OPT2002::read(float* data, uint8_t numdata) {
    float raw = analogRead(pin);
    data[0] = raw;
    data[1] = raw < OPT2002_OUT_MIN ? -1 : ((raw - OPT2002_IN_MIN) / (OPT2002_IN_MAX - OPT2002_IN_MIN) * (OPT2002_OUT_MAX - OPT2002_OUT_MIN) + OPT2002_OUT_MIN);
    // TODO: other error conditions?
    return ERR_NONE;
}