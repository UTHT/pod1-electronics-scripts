#include "SPTD25-20-1000H.h"

struct t_datasetup datasetup = {2, {"kPa", "kPa (Mapped)"}};

SPTD25_20_1000H::SPTD25_20_1000H(uint8_t pin, arduino_t arduino) : Sensor(S_SPTD25_20_1000H, arduino, datasetup, 250){
    this->pin = pin;
}

errorlevel_t SPTD25_20_1000H::init(){
    pinMode(pin, INPUT);
    return true;
}

// TODO Calibrate from 0 psi to 200 psi
// Currently sensorValue at 0 psi is 176 - 177
errorlevel_t SPTD25_20_1000H::read(t_datum* data, uint8_t numdata){
    // NOTE: Convention - check that numdata given matches expected
    if(numdata != 2){
      return ERR_FAIL;
    }
    int val = analogRead(pin);
    data[0].data = (float)val;
    data[1].data = (float)sptd25_map(val, 176.0, 1023.0, 0, 200.0);
    // TODO: other error conditions?
    return ERR_NONE;
}

double sptd25_map(const int x, const double in_min, const double in_max, const double out_min, const double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}