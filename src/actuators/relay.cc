#include <actuators/relay.h>

#include <Arduino.h>

#include <utils/base.h>
#include <actuators/actuator.h>

Relay::Relay(uint8_t pin) : Actuator(ACTUATOR_RELAY, RELAY_NAME, 1) {
  this->pin = pin;
}

errorlevel_t Relay::initialize(void) {
  pinMode(pin, OUTPUT);
  return ERR_NONE;
}

errorlevel_t Relay::set(float target) {
  digitalWrite(pin, target); // Engaged = digital low
  return ERR_NONE;
}