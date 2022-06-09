#ifndef UTHT_ACTUATORS_BRAKES_H_
#define UTHT_ACTUATORS_BRAKES_H_

#include <Arduino.h>

#include <utils/base.h>
#include <actuators/actuator.h>

#define BRAKES_NAME "Braking Solenoid"

class Brakes : public Actuator {
  public:
    /**
     * Constructor.
     * @param pin Output pin.
     */
    Brakes(uint8_t pin);
  private:
    errorlevel_t initialize(void) override;
    errorlevel_t set(float target) override;

    // Output pin.
    uint8_t pin;

    bool engaged = false;
};

#endif
