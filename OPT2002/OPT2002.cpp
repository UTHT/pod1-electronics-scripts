#include "OPT2002.h"

const char* arr[2] = {"kPa (Raw)", "kPa"};
t_datasetup datasetup = {2, arr};

OPT2002::OPT2002(uint8_t pin, arduino_t arduino) : Sensor(S_OP, arduino, datasetup, 250){
    this->pin = pin;
}

errorlevel_t OPT2002::init(){
    pinMode(pin, INPUT);
    return ERR_NONE;
}

// TODO Calibrate from 0 psi to 200 psi
// Currently sensorValue at 0 psi is 176 - 177
errorlevel_t OPT2002::read(t_datum* data, uint8_t numdata){
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

double spt25_map(const int x, const double in_min, const double in_max, const double out_min, const double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}