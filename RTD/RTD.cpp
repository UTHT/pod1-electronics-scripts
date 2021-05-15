#include "RTD.h"

const char* arr[2] = {"deg F (Raw)", "deg F"};
t_datasetup datasetup = {2, arr};

RTD::RTD(uint8_t pin, arduino_t arduino) : Sensor(S_PROSENSERTD, arduino, datasetup, 500){
    this->pin = pin;
}

errorlevel_t RTD::init(){
    pinMode(pin, INPUT);
    return ERR_NONE;
}

// TODO Calibrate from 0 psi to 200 psi
// Currently sensorValue at 0 psi is 176 - 177
errorlevel_t RTD::read(t_datum* data, uint8_t numdata){
    // NOTE: Convention - check that numdata given matches expected
    if(numdata != 2){
      return ERR_FAIL;
    }
    int val = analogRead(pin);
    data[0].data = (float)val;
    data[1].data = (float)map(val, 180, 901, -40, 300);
    // TODO: other error conditions?
    return ERR_NONE;
}