#ifndef UTHT_SENSORS_DCT200_H_
#define UTHT_SENSORS_DCT200_H_

#define DCT200_DELTA 100
#define DCT200_IN_MIN 204.8 // 4mA x 165ohm / 3.3V x 1023
#define DCT200_IN_MAX 1023  // 20mA x 165ohm / 3.3V  x 1023
#define DCT200_OUT_MIN 0
#define DCT200_OUT_MAX 100

#include <Arduino.h>

#include <sensors/sensor.h>

class DCT200 : public Sensor {
  public:
    /**
     * Constructor.
     * @param pin Analog input pin.
     */
    DCT200(uint8_t pin);
  private:
    errorlevel_t initialize(void) override;

    errorlevel_t read(float* data, uint8_t numdata) override;

    // Analog input pin.
    uint8_t pin;
};

#endif