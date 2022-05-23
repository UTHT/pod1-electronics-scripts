#ifndef UTHT_ACTUATORS_RELAY_H_
#define UTHT_ACTUATORS_RELAY_H_

#include <Arduino.h>

#include <utils/base.h>
#include <actuators/actuator.h>

class Relay : public Actuator {
  public:
    /**
     * Constructor.
     * @param pin Output pin.
     */
    Relay(uint8_t pin);
  private:
    errorlevel_t initialize(void) override;
    errorlevel_t set(float target) override;

    // Output pin.
    uint8_t pin;
};

#endif
