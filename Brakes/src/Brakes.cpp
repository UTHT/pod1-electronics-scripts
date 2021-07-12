#include "Brakes.h"
#include "Actuator.h"

Brakes::Brakes(uint8_t pin, arduino_t arduino) : Actuator(A_BRAKES, arduino, 1.0) {
    this->pin = pin;
    this->engaged = false;
}

errorlevel_t Brakes::init() {
    pinMode(pin, OUTPUT);
}

errorlevel_t Brakes::set(float target) {
    engaged |= (target != 0); //If engaged is true already, target doesn't matter (stay engaged)
    digitalWrite(pin, !engaged); // Engaged = digital low
}