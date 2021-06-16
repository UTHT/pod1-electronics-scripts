#include "VFS500.h"

const char* arr[1] = {"A"};
t_datasetup datasetup = {1, arr};

VFS500::VFS500(uint8_t pin, arduino_t arduino) : Sensor(S_VFS500, arduino, datasetup, 250){
    this->pin = pin;
}

errorlevel_t VFS500::init(){
    pinMode(pin, INPUT);
    return ERR_NONE;
}

// Maping function for VFS500
double VFS500_map(const int x, const double in_min, const double in_max, const double out_min, const double out_max){
    return (x - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
}

errorlevel_t VFS500::read(t_datum* data, uint8_t numdata){
    // NOTE: Convention - check that numdata given matches expected
    if(numdata != 1){       //TODO: globally declare the array size instead of using the int value
      return ERR_FAIL;
    }

    float ontime = pulseIn(pulse_pin,HIGH);
    float offtime = pulseIn(pulse_pin,LOW);
    float period = ontime + offtime;

    float freq = 1000000.0 / period;
    float duty = (ontime / period) * 100;
    
    float flow =  VFS500_map(freq, 0, 100, 0.26, 5.28);     //measured in gallons per minute(gpm)
    
    data[0].data = (float)flow;
    // TODO: other error conditions?
    return ERR_NONE;
}
