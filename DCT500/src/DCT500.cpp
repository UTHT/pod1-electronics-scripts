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
    return (x - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
}

errorlevel_t DCT500::read(t_datum* data, uint8_t numdata){
    // NOTE: Convention - check that numdata given matches expected
    if(numdata != 1){       //TODO: globally declare the array size instead of using the int value
      return ERR_FAIL;
    }

    const double resistor = 300;
    // For Arduino Due use a 300 ohm resistor, 10V input read as 3.3V at the Arduino
    // For Arduino Mega use a 200 ohm resistors, 10V input read as 5.0V at the Arduino
    
    // Calibrate from analogRead to 10V (undoing the resistor)
    double output_voltage = dct500_map(analogRead(pin), 0, 1023, 0, 10);
    double read_current = output_voltage / resistor;
    double calculated_current = 0;

    if(read_current != 0){
        // Calibrate from 0 A to 500 A
        calculated_current = dct500_map(read_current, 4, 20, 0, 500);
    }
    
    data[0].data = (float)calculated_current;
    // TODO: other error conditions?
    return ERR_NONE;
}
