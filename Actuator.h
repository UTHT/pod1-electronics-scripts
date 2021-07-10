#ifndef Actuator_H
#define Actuator_H

#include "Base.h"

// Actuator identifiers
typedef enum actuators_t {
  A_BRAKES,
  A_INVERTER
} actuators_t;

typedef struct ActuatorState {
  errorlevel_t error;
  debuglevel_t debug;
  float target;
} ActuatorState;

class Actuator{
  protected:
    virtual errorlevel_t set(float target) = 0;
    virtual errorlevel_t init() = 0;
    ActuatorState state;
  public:
    arduino_t arduino;
    actuators_t actuator;
    float nulltarget;       // what should we default to in case of failure?
    Actuator(actuators_t actuator, arduino_t arduino, float nulltarget);
    ActuatorState* update();
    ActuatorState* begin();
};

#endif 