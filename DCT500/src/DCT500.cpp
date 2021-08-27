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

// Maping function for DCT500
double dct500_map(const int x, const double in_min, const double in_max, const double out_min, const double out_max){
    return out_min + (((x - in_min) / (in_max - in_min)) * (out_max - out_min));
}

errorlevel_t DCT500::read(t_datum* data, uint8_t numdata){
    // NOTE: Convention - check that numdata given matches expected
    if(numdata != 1){       //TODO: globally declare the array size instead of using the int value
      return ERR_FAIL;
    }

    /*
    For Arduino Due use a 300 ohm resistor, 10V input read as 3.3V at the Arduino
    For Arduino Mega use a 200 ohm resistors, 10V input read as 5.0V at the Arduino
    The Sensor output 4-20mA. 
    */
   
    // Calibrate from analogRead to 500A 
    const double resistor = 200;

     // This is only for Arduino Mega with 200 ohm 
    //double calculated_current = dct500_map(analogRead(pin), 160, 819, 0, 500); 
    // This is only for Arduino Mega with 300 ohm (bc 1023 rep. 3.3V instead of 5, and we will ever reach that high of a current)
    // double calculated_current = dct500_map(analogRead(pin), 160, 1860, 0, 500);  
    
    data[0].data = (float)analogRead(pin);
    
    //data[0].data = (float)calculated_current;
    // TODO: other error conditions?
    return ERR_NONE;
}
