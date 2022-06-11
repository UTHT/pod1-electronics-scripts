#ifndef UTHT_ACTUATORS_INVERTER_H_
#define UTHT_ACTUATORS_INVERTER_H_

#include <Arduino.h>

#include <utils/base.h>
#include <interfaces/interface.h>

#define INVERTER_NAME       "Inverter"
#define INVERTER_TORQUE     30
#define INVERTER_BAUD       250000
#define INVERTER_FAILTARGET 0
#define INVERTER_DELTA      50
#define INVERTER_RPM_MAX    1000        // Safety first

class Inverter : public Interface {
  public:
    /**
     * Constructor.
     * @param canbus Serial CAN bus interface.
     * @param tx
     * @param rx
     */
    Inverter(Serial_CAN* canbus, uint8_t tx, uint8_t rx);
  private:
    errorlevel_t initialize(void) override;
    errorlevel_t set(float target) override;
    errorlevel_t read(float* data, uint8_t numdata) override;

    void disableLockout(void);
    void sendSpeedCommand(void);

    // Serial CAN bus interface.
    Serial_CAN* canbus;

    unsigned char vsm_state;
    unsigned char inv_state;
    int rpm;
};

#endif
