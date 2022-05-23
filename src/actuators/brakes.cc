#include <actuators/brakes.h>

#include <Arduino.h>

#include <utils/base.h>
#include <actuators/actuator.h>

Brakes::Brakes(uint8_t pin) : Actuator(ACTUATOR_BRAKES, 0) {
  this->pin = pin;
}

errorlevel_t Brakes::initialize(void) {
  pinMode(pin, OUTPUT);
  return ERR_NONE;
}

errorlevel_t Brakes::set(float target) {
  engaged |= (target != 0); //If engaged is true already, target doesn't matter (stay engaged)
  digitalWrite(pin, !engaged); // Engaged = digital low
  return ERR_NONE;
}