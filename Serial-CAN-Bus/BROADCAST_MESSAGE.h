#ifndef BROADCAST_MESSAGE_H
#define BROADCAST_MESSAGE_H

#include "PM100_CAN_BUS.h"

// Reference 0x0AA - Internal States, Byte #0,1
typedef enum vsm_state
{
    VSM_START,
    PRE_CHARGE_INIT,
    PRE_CHARGE_ACTIVE,
    PRE_CHARGE_COMPLETE,
    VSM_WAIT,
    VSM_READY,
    MOTOR_RUNNING,
    BLINK_FAULT_CODE,
    SHUTDOWN_IN_PROGRESS = 14,
    RECYCLE_POWER
} vsm_state;

// Reference 0x0AA - Internal States, Byte #2
typedef enum inverter_state
{
    POWER_ON,
    STOP,
    OPEN_LOOP,
    CLOSED_LOOP,
    WAIT,
    IDLE_RUN = 8,
    IDLE_STOP
} inverter_state;

class broadcast_message
{
public:
    message_type broadcast_message;
    void print_vsm_state(int message_arr[9]);
    void print_inverter_state(int message_arr[9]);
}

#endif