#ifndef BROADCAST_MESSAGE_H
#define BROADCAST_MESSAGE_H

//#include <C:\adam\utht\extra low level\COMMAND_MESSAGE.h>
#include <COMMAND_MESSAGE.h>

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
    RECYCLE_POWER,
    NOT_A_VSM_STATE
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
    IDLE_STOP,
    NOT_AN_INVERTER_STATE = 13
} inverter_state;

// Reference Param Address 148
typedef enum can_active_messages_lo_word
{
    // Data Byte 4
    TEMPERATURE_1,
    TEMPERATURE_2,
    TEMPERATURE_3,
    ANALOG_INPUT_VOLTAGES,
    DIGITAL_INPUT_STATUS,
    MOTOR_POSITION_INFORMATION,
    CURRENT_INFORMATION,
    VOLTAGE_INFORMATION,
    // Data Byte 5
    FLUX_INFORMATION,
    INTERNAL_VOLTAGES,
    INTERNAL_STATES,
    FAULT_CODES,
    TORQUE_AND_TIMER_INFORMATION,
    MODULATION_INDEX_AND_FLUX_WEAKENING_OUTPUT_INFORMATION,
    FIRMWARE_INFORMATION,
    DIAG_DATA
} can_active_messages_lo_word;

// Reference Param Address 148, should always be enabled (default)
typedef enum can_active_messages_hi_word
{
    // Data Byte 6
    SLAVE_MODE_COMMAND_MESSAGE = 6,
    BMS_COMMAND_MESSAGE,
    // Data Byte 7
    OBD2_GENERAL_QUERY,
    OBD2_SPECIFIC_QUERY,
    OBD2_RESPONSE,
    U2C_TX_MESSAGE,
    U2C_RX_MESSAGE,
    PARAMETER_RESPONSE_MESAGE,
    PARAMETER_COMMAND_MESSAGE,
    CAN_COMMAND_MESSAGE
} can_active_messages_hi_word;

class broadcast_message{
    
    public:
    
        message_type broadcast_message;
    
        vsm_state read_vsm_state( int message_arr[9] );
        friend std::ostream& operator<<( std::ostream& lhs, vsm_state state );
        friend int return_vsm_state_val( vsm_state state );
    
        inverter_state read_inverter_state( int message_arr[9] );
        friend std::ostream& operator<<( std::ostream& lhs, inverter_state state );
        friend int return_inverter_state_val( inverter_state state );
    
        void disable_broadcast_message(unsigned char (&CAN_mssg)[9], can_active_messages_lo_word content);
}

#endif
