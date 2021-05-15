#include "OPT2002.h"

const char* arr[1] = {"mm"};
t_datasetup datasetup = {1, arr};

OPT2002::OPT2002(uint8_t inpin, uint8_t errpin, arduino_t arduino) : Sensor(S_OPT2002, arduino, datasetup, 250){
    this->inpin = inpin;
    this->errpin = errpin;
}

errorlevel_t OPT2002::init(){
    pinMode(inpin, INPUT);
    pinMode(errpin, INPUT);
    return ERR_NONE;
}

// TODO Calibrate from 0 psi to 200 psi
// Currently sensorValue at 0 psi is 176 - 177
errorlevel_t OPT2002::read(t_datum* data, uint8_t numdata){
    // NOTE: Convention - check that numdata given matches expected
    if(numdata != 1){
      return ERR_FAIL;
    }
    //turn on the sensors
    digitalWrite(errpin,LOW);
    digitalWrite(errpin,HIGH);

    // TODO: Figure out the error checking
    // String error = String(digitalRead(errpin));
    // if(error.length() > 0){
    //     return ERR_WARN;
    // }
    
    double distance = 30 + 5 * (analogRead(inpin) * 3.3 * 2);	

    data[0].data = (float)distance;
    // TODO: other error conditions?
    return ERR_NONE;
}