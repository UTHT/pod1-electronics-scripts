#ifndef UTHT_SENSORS_OPT2002_H_
#define UTHT_SENSORS_OPT2002_H_

#define OPT2002_NAME    "OPT2002"
#define OPT2002_DELTA   100
#define OPT2002_IN_MIN  0 // 4mA x 165ohm / 3.3V x 1023
#define OPT2002_IN_MAX  1023 // 20mA x 165ohm / 3.3V  x 1023
#define OPT2002_OUT_MIN 30
#define OPT2002_OUT_MAX 80

#include <Arduino.h>

#include <sensors/sensor.h>

class OPT2002 : public Sensor {
  public:
    /**
     * Constructor.
     * @param pin Analog input pin.
     */
    OPT2002(uint8_t pin);
  private:
    errorlevel_t initialize(void) override;

    errorlevel_t read(float* data, uint8_t numdata) override;

    // Analog input pin.
    uint8_t pin;
};

#endif