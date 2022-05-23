#ifndef UTHT_SENSORS_SPTD25_20_1000H_H_
#define UTHT_SENSORS_SPTD25_20_1000H_H_

#define SPTD25_20_1000H_DELTA 250
#define SPTD25_20_1000H_IN_MIN 204.8 // 4mA x 165ohm / 3.3V x 1023
#define SPTD25_20_1000H_IN_MAX 1023 // 20mA x 165ohm / 3.3V  x 1023
#define SPTD25_20_1000H_OUT_MIN 0
#define SPTD25_20_1000H_OUT_MAX 1000

#include <Arduino.h>

#include <sensors/sensor.h>

class SPTD25_20_1000H : public Sensor {
  public:
    /**
     * Constructor.
     * @param pin Analog input pin.
     */
    SPTD25_20_1000H(uint8_t pin);
  private:
    errorlevel_t initialize(void) override;

    errorlevel_t read(float* data, uint8_t numdata) override;

    // Analog input pin.
    uint8_t pin;
};

#endif