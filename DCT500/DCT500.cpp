#include "DCT500.h"

const char* arr[1] = {"A"};
t_datasetup datasetup = {1, arr};

DCT500::DCT500(uint8_t pin, arduino_t arduino) : Sensor(S_DCT500, arduino, datasetup, 250){
    this->pin = pin;
}

errorlevel_t DCT500::init(){
    pinMode(pin, INPUT);
    return ERR_NONE;
}

// TODO Calibrate from 0 A to 500 A
errorlevel_t DCT500::read(t_datum* data, uint8_t numdata){
    // NOTE: Convention - check that numdata given matches expected
    if(numdata != 1){//TODO: globally declare the array size instead of using the int value
      return ERR_FAIL;
    }
    double current = analogRead(pin)*5/1023.0/100;

    data[0].data = (float)current;
    // TODO: other error conditions?
    return ERR_NONE;
}