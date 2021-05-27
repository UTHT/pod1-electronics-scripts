#include "OPT2002.h"

const char* arr[1] = {"mm"};
t_datasetup datasetup = {1, arr};

OPT2002::OPT2002(uint8_t inpin, uint8_t errpin, arduino_t arduino) : Sensor(S_OPT2002, arduino, datasetup, 250){
    this->inpin = inpin;
    this->errpin = errpin;
}

errorlevel_t OPT2002::init(){
    pinMode(inpin, INPUT);
    pinMode(errpin, INPUT_PULLUP);      // To use the internal resistor in the arduino
    return ERR_NONE;
}

// Maping function for OPT2002
double opt2002_map(const int x, const double in_min, const double in_max, const double out_min, const double out_max){
    return (x - in_min) / (in_max - in_min) * (out_max - out_min) + out_min;
}

errorlevel_t OPT2002::read(t_datum* data, uint8_t numdata){
    if(numdata != 1){ //TODO: globally declare the array size instead of using the int value
      return ERR_FAIL;
    }

    // Error checking (opt2002 has a PNP output, turn positive when an error has occurred)
    // An error occurs when the sensor cannot detect an object too. Also the red F LED on the sensor lights up.
    double sensorVal = digitalRead(errpin);
    if (sensorVal == HIGH){
        return ERR_WARN;
    }

    // Set its analog output as voltage output(10V)
    const double resistor = 300;     // ohm (we are using THREE 100 ohm ressitor for a voltage divider) 
    // For Arduino Due voltage divider using 3 100 ohm resistors, 10V input read as 3.3V at the Arduino
    // For Arduino Mega voltage divider using 2 100 ohm resistors, 10V input read as 5.0V at the Arduino
    
    // Calibrate from analogRead to 10V (undoing the voltage divider)
    double voltage = dct500_map(analogRead(pin), 0, 1023, 0, 10);
    double current = voltage / resistor;

    // Calibrate from 30mm to 80mm
    double distance = dct500_map(current, 4, 20, 30, 80);

    data[0].data = (float)distance;
    // TODO: other error conditions?
    return ERR_NONE;
}