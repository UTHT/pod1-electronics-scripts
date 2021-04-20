#include <stdlib.h>
#include "Arduino.h"
#include "Sensor.h"

/**
 * Constructor. 
 * @param sensor - sensor_t - Which sensor is this?
 * @param arduino - arduino_t - Which Arduino are we attached to?
 * @param delta - uint16_t - The minimum time (in ms) between sensor reads.
 **/
Sensor(sensor_t sensor, arduino_t arduino, t_datasetup setup, uint16_t delta){
    // Set up cache
    this->sensor = sensor;
    this->arduino = arduino;
    state.error = NONE;
    state.debug = DISABLED;
    state.age = 0;

    // Allocate data locations
    state.data = (t_datum*)malloc(sizeof(t_datum)*setup.numdata);
    state.numdata = setup.numdata;
    for(int i = 0; i < setup.numdata; i++){
        // Since setup.units[i] is a const char*, we can just reassign our pointer
        state.data[i].units = setup.units[i];
    }

    this->delta = delta;
    lastread = millis();    //Buffer - Resets the last read time to the current time
}

bool Sensor::begin(){
    if(init()){
        state.debug = INIT;
    } else {
        state.debug = DISABLED;
        state.error = FAIL;
    }
}

/**
 * Wrapper for hardware-level read function. Updates the state.
 * Checks that enough time has passed between reads.
 **/
void Sensor::update(){
    // Check preconditions
    if(state.error < FAIL && state.debug >= INIT){
        // Check timing
        if(millis()-lastread > delta){
            read();
            //Reset the last read time to now
            lastread = millis();
            // Did read() generate an error?
            state.debug = NEWREAD;
            state.age = 0;      //Approximately
        }
        state.debug = WAITING;
        state.age = millis()-lastread;
    }
}