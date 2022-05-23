#ifndef UTHT_SENSORS_RTD_H_
#define UTHT_SENSORS_RTD_H_

#define RTD_DELTA 250
#define RTD_IN_MIN 204.8 // 4mA x 165ohm / 3.3V x 1023
#define RTD_IN_MAX 1023 // 20mA x 165ohm / 3.3V  x 1023
#define RTD_OUT_MIN -40
#define RTD_OUT_MAX 150

#include <Arduino.h>

#include <sensors/sensor.h>

class RTD : public Sensor {
  public:
    /**
     * Constructor.
     * @param pin Analog input pin.
     */
    RTD(uint8_t pin);
  private:
    errorlevel_t initialize(void) override;

    errorlevel_t read(float* data, uint8_t numdata) override;

    // Analog input pin.
    uint8_t pin;
};

#endif