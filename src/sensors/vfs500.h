#ifndef UTHT_SENSORS_VFS500_H_
#define UTHT_SENSORS_VFS500_H_

#define VFS500_DELTA 250
#define VFS500_IN_MIN 0
#define VFS500_IN_MAX 100
#define VFS500_OUT_MIN 0.26
#define VFS500_OUT_MAX 5.28

#include <Arduino.h>

#include <sensors/sensor.h>

class VFS500 : public Sensor {
  public:
  /**
   * Constructor.
   * @param pin Input pin
   */
    VFS500(uint8_t pin);
  private:
    errorlevel_t initialize(void) override;

    errorlevel_t read(float* data, uint8_t numdata) override;

    // Input pin.
    uint8_t pin;
};

#endif