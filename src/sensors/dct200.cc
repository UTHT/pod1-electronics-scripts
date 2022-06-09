#include <sensors/dct200.h>

#include <Arduino.h>

#include <utils/base.h>
#include <sensors/sensor.h>

// Data setup
static const char* labels[2] = { 
  "A (Raw)", 
  "A"
};

static const SensorDataSetup datasetup = {
  .numdata = 2,
  .labels = labels
};

DCT200::DCT200(uint8_t pin) : Sensor(SENSOR_DCT200, DCT200_NAME, &datasetup, DCT200_DELTA){
    this->pin = pin;
}

errorlevel_t DCT200::initialize(void) {
    pinMode(pin, INPUT);     // To use the internal resistor in the arduino
    return ERR_NONE;
}

errorlevel_t DCT200::read(float* data, uint8_t numdata) {
    float raw = analogRead(pin);
    if (raw < DCT200_OUT_MIN) {
      return ERR_FATAL;
    }
    data[0] = raw;
    data[1] = ((raw - DCT200_IN_MIN) / (DCT200_IN_MAX - DCT200_IN_MIN) * (DCT200_OUT_MAX - DCT200_OUT_MIN) + DCT200_OUT_MIN);
    // TODO: other error conditions?
    return ERR_NONE;
}