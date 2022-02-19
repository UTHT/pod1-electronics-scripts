#ifndef INVERTER_H
#define INVERTER_H

#include "Actuator.h"
#include "Serial_CAN_Module.h"

//types of CAN messages
typedef enum m_type {
  BROADCAST_M,
  COMMAND_TOURQUE_M,
  COMMAND_SPEED_M
} message_type;

// COMMAND

//command message variables for inverter
typedef enum i_direction {
  REVERSE, //clock-wise
  FORWARD  //counter-clock-wise
} inverter_direction;

typedef enum i_enable {
  DISABLE,
  ENABLE
} inverter_enable;

class Inverter : public Actuator {
    public:
        Inverter(Serial_CAN* CANBus, arduino_t arduino);
    private:
        Serial_CAN* CANBus;
        errorlevel_t set(float target) override;
        errorlevel_t init() override;
};

#endif