#ifndef UTHT_SENSORS_TMP006_H_
#define UTHT_SENSORS_TMP006_H_

#define TMP006_NAME "TMP006"
#define TMP006_DELTA 250
#define TMP006_IN_MIN 204.8 // 4mA x 165ohm / 3.3V x 1023
#define TMP006_IN_MAX 1023 // 20mA x 165ohm / 3.3V  x 1023
#define TMP006_OUT_MIN -40
#define TMP006_OUT_MAX 150

// Thermal constants
#define TMP006_B0 -0.0000294
#define TMP006_B1 -0.00000057
#define TMP006_B2 0.00000000463
#define TMP006_C2 13.4
#define TMP006_TREF 298.15
#define TMP006_A2 -0.00001678
#define TMP006_A1 0.00175
#define TMP006_S0 6.4 // * 10^-14

// Register map
#define TMP006_CONFIG 0x02

#define TMP006_CFG_RESET 0x8000
#define TMP006_CFG_MODEON 0x7000
#define TMP006_CFG_1SAMPLE 0x0000
#define TMP006_CFG_2SAMPLE 0x0200
#define TMP006_CFG_4SAMPLE 0x0400
#define TMP006_CFG_8SAMPLE 0x0600
#define TMP006_CFG_16SAMPLE 0x0800
#define TMP006_CFG_DRDYEN 0x0100
#define TMP006_CFG_DRDY 0x0080

#define TMP006_MANID 0xFE
#define TMP006_DEVID 0xFF

#define TMP006_VOBJ 0x0
#define TMP006_TAMB 0x01

// Options
#define TMP006_SAMPLERATE TMP006_CFG_16SAMPLE

#include <Arduino.h>
#include <Wire.h>
  
#include <sensors/sensor.h>

class TMP006 : public Sensor {
  public:
    /**
     * Constructor.
     * @param address I2C Address
     */
    TMP006(uint8_t address);
  private:
    errorlevel_t initialize(void) override;

    errorlevel_t read(float* data, uint8_t numdata) override;

    // I2C Address
    uint8_t address;
};

#endif