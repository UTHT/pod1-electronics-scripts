#ifndef BRAKES_H
#define BRAKES_H

#include "Actuator.h"

class Brakes : public Actuator {
    public:
        Brakes(uint8_t pin, arduino_t arduino);
    private:
        errorlevel_t set(float target) override;
        errorlevel_t init() override;
        uint8_t pin;
        bool engaged = false; //Toggle-ON latch boolean (can only be turned ON)
};

#endif