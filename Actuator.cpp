#include "Arduino.h"
#include "Actuator.h"

Actuator::Actuator(actuators_t actuator, arduino_t arduino, float failtarget){
    this->actuator = actuator;
    this->arduino = arduino;
    state.error = ERR_NONE;
    state.debug = DS_DISABLED;
    state.target = this->failtarget = failtarget; // Default start target
}

ActuatorState* Actuator::update(){
    if(state.error < ERR_FAIL && state.debug >= DS_INIT){
        state.error = set(state.target);

        switch(state.error){
            case ERR_NONE:  //Success!
                state.debug = DS_SUCCESS;
                break;
            case ERR_WARN:  //Set didn't go as planned, non-fatal
                // DO NOT UPDATE STATE VALUES
                break;
            case ERR_FAIL:  //Set failed catastrophically
                state.debug = DS_DISABLED;
                set(failtarget);
                break;
        }
    }
    return &state;
}

ActuatorState* Actuator::begin(){
    state.error = init();
    if(state.error > ERR_NONE){
        state.debug = DS_DISABLED;
    } else {
        state.debug = DS_INIT;
    }
    return &state;
}