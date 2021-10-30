#include "DCT500.h"

const char* arr[2] = {"A", "Raw"};
t_datasetup datasetup = {2, arr};

DCT500::DCT500(uint8_t pin, arduino_t arduino) : Sensor(S_DCT500, arduino, datasetup, 250){
    this->pin = pin;
}

errorlevel_t DCT500::init(){
    pinMode(pin, INPUT);
    return ERR_NONE;
}

// Maping function for DCT500
double dct500_map(const int x, const double in_min, const double in_max, const double out_min, const double out_max){
    return out_min + (((x - in_min) / (in_max - in_min)) * (out_max - out_min));
}

errorlevel_t DCT500::read(t_datum* data, uint8_t numdata){
    // NOTE: Convention - check that numdata given matches expected
    if(numdata != 1){       //TODO: globally declare the array size instead of using the int value
      return ERR_FAIL;
    }

    uint32_t analogRaw = analogRead(pin);
    // 165ohm output load at 4mA "zero read" output current = 0.66V or 20% of 3.3V -> Due reads 204
    double calculated_current = dct500_map(analogRaw, 204, 1024, 0, 500); 
    
    data[0].data = (float)calculated_current;
    data[1].data = analogRaw;
    // TODO: other error conditions?
    return ERR_NONE;
}
