#ifndef UTHT_SENSORS_SPT25_20_0200A_H_
#define UTHT_SENSORS_SPT25_20_0200A_H_

#define SPT25_20_0200A_NAME     "SPT25-20-0200A"
#define SPT25_20_0200A_DELTA    250
#define SPT25_20_0200A_IN_MIN   204.8 // 4mA x 165ohm / 3.3V x 1023
#define SPT25_20_0200A_IN_MAX   1023 // 20mA x 165ohm / 3.3V  x 1023
#define SPT25_20_0200A_OUT_MIN  0
#define SPT25_20_0200A_OUT_MAX  200

#include <Arduino.h>

#include <sensors/sensor.h>

class SPT25_20_0200A : public Sensor {
  public:
    /**
     * Constructor.
     * @param pin Analog input pin.
     */
    SPT25_20_0200A(uint8_t pin);
  private:
    errorlevel_t initialize(void) override;

    errorlevel_t read(float* data, uint8_t numdata) override;

    // Analog input pin.
    uint8_t pin;
};

#endif