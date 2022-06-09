#include <actuators/brakes.h>

#include <Arduino.h>

#include <utils/base.h>
#include <actuators/actuator.h>

Brakes::Brakes(uint8_t pin) : Actuator(ACTUATOR_BRAKES, BRAKES_NAME, 0) {
  this->pin = pin;
}

errorlevel_t Brakes::initialize(void) {
  pinMode(pin, OUTPUT);
  return ERR_NONE;
}

errorlevel_t Brakes::set(float target) {
  engaged |= ((int)target > 0); // Engages if target high, latching
  digitalWrite(pin, !engaged);  // Engage (digital out LOW)
  return ERR_NONE;
}