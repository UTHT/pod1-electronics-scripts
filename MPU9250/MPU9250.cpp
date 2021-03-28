#include "MPU9250.h"

const char* arr[1] = {"A"};
t_datasetup datasetup = {1, arr};

MPU9250::MPU9250(uint8_t pin, arduino_t arduino) : Sensor(S_MPU, arduino, datasetup, 250){
    this->pin = pin;
}

errorlevel_t MPU9250::init(){
    pinMode(pin, INPUT);
    return ERR_NONE;
}

// TODO Calibrate from 0 psi to 200 psi
// Currently sensorValue at 0 psi is 176 - 177
errorlevel_t MPU9250::read(t_datum* data, uint8_t numdata){
    // NOTE: Convention - check that numdata given matches expected
    if(numdata != 1){
      return ERR_FAIL;
    }
    double current = analogRead(pin)*5/1023.0/100;

    data[0].data = (float)current;
    // TODO: other error conditions?
    return ERR_NONE;
}