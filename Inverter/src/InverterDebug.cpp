#include "Sensor.h"
#include "InverterDebug.h"

InverterDebug::InverterDebug(Serial_CAN* CANBus, arduino_t arduino) : Sensor(S_INVERTERDEBUG, arduino, 1000) {
    this->CANBus = CANBus;
}

InverterDebug::init(){
    return ERR_NONE;
}

errorlevel_t InverterDebug::read(t_datum* data, uint8_t numdata){
    // TODO: Get can message
    int msg[9] = {0,0,0,0,0,0,0,0,0};
    data[0].data = read_vsm_state(msg);
    data[1].data = read_inverter_state(msg);
    data[2].data = read_fault_codes(msg);
    // TODO: fault triggers return error
    return ERR_NONE;
}

vsm_state read_vsm_state(int CAN_msg[9])
{
    if (CAN_msg[0] != 0x0AA || !((CAN_msg[2] >= 0 && CAN_msg[2] <= 7) || CAN_msg[2] == 14 || CAN_msg[2] == 15)){
        // cout << "Message CAN ID is not 0x0AA, this is NOT a VSM message" << endl;
        return NOT_A_VSM_STATE;
    }
    return (vsm_state)CAN_msg[2];
}

inverter_state read_inverter_state(int CAN_msg[9])
{
    if (CAN_msg[0] != 0x0AA || !((CAN_msg[3] >=0 && CAN_msg[3] <= 4) || CAN_msg[3] == 8 || CAN_msg[3] == 9))
    {
        // cout << "Message CAN ID is not 0xAA, this is NOT an inverter" << endl;
        return NOT_AN_INVERTER_STATE;
    }

    return (inverter_state)CAN_msg[3];
}

/* Disable one or more messages at a time
 * Example usage: disable_broadcast_message(test_msg, TEMPERATURE_2 | CURRENT_INFORMATION | DIAG_DATA);
 */
void disable_broadcast_message(int CAN_msg[9], int listOfMsgs)
{
    if (CAN_msg[0] != 0x0C1 || CAN_msg[1] != 148) // CAN ID
    {
        // cout << "Message CAN Address is not 0x0C1" << endl;
        // cout << "Message Parameter Address is not 148" << endl;
        return;
    }

    int loWord = (CAN_msg[6] << 8) | CAN_msg[5]; // loWord = [6] concatenated with [5]

    if ((TEMPERATURE_1 & listOfMsgs) == TEMPERATURE_1)
        loWord &= ~(1U);
    if ((TEMPERATURE_2 & listOfMsgs) == TEMPERATURE_2)
        loWord &= ~(1U << 1);
    if ((TEMPERATURE_3 & listOfMsgs) == TEMPERATURE_3)
        loWord &= ~(1U << 2);
    if ((ANALOG_INPUT_VOLTAGES & listOfMsgs) == ANALOG_INPUT_VOLTAGES)
        loWord &= ~(1U << 3);
    if ((DIGITAL_INPUT_STATUS & listOfMsgs) == DIGITAL_INPUT_STATUS)
        loWord &= ~(1U << 4);
    if ((MOTOR_POSITION_INFORMATION & listOfMsgs) == MOTOR_POSITION_INFORMATION)
        loWord &= ~(1U << 5);
    if ((CURRENT_INFORMATION & listOfMsgs) == CURRENT_INFORMATION)
        loWord &= ~(1U << 6);
    if ((VOLTAGE_INFORMATION & listOfMsgs) == VOLTAGE_INFORMATION)
        loWord &= ~(1U << 7);
    if ((FLUX_INFORMATION & listOfMsgs) == FLUX_INFORMATION)
        loWord &= ~(1U << 8);
    if ((INTERNAL_VOLTAGES & listOfMsgs) == INTERNAL_VOLTAGES)
        loWord &= ~(1U << 9);
    if ((INTERNAL_STATES & listOfMsgs) == INTERNAL_STATES)
        loWord &= ~(1U << 10);
    if ((FAULT_CODES & listOfMsgs) == FAULT_CODES)
        loWord &= ~(1U << 11);
    if ((TORQUE_AND_TIMER_INFORMATION & listOfMsgs) == TORQUE_AND_TIMER_INFORMATION)
        loWord &= ~(1U << 12);
    if ((MOD_INDEX_FLUX_WEAKENING_OUTPUT_INFO & listOfMsgs) == MOD_INDEX_FLUX_WEAKENING_OUTPUT_INFO)
        loWord &= ~(1U << 13);
    if ((FIRMWARE_INFORMATION & listOfMsgs) == FIRMWARE_INFORMATION)
        loWord &= ~(1U << 14);
    if ((DIAG_DATA & listOfMsgs) == DIAG_DATA)
        loWord &= ~(1U << 15);

    // Separate loWord back into two bytes
    CAN_msg[5] = loWord & 0xFF;
    CAN_msg[6] = (loWord >> 8) & 0xFF;
}
 
int64_t read_fault_codes(int CAN_msg[9]) {
    //extract the CAN ID & define return vector of fault codes
    int CAN_ID = CAN_msg[0];
    int64_t faults;

    //check if CAN message is FAULT message
    if( CAN_ID != 0x0AB ){
        faults |= 1UL << 64;
    } else if( CAN_ID == 0x0AB ){
        int fault_byte = 0; //var used for indexing fault code array

        //here we'll loop 8 times, analyze each CAN data byte
        for( int msgbyte = 0; msgbyte < 8; msgbyte++  )
        {
            int byte_i = CAN_msg[msgbyte];

            //vars to check each bit of current byte
            int current_bit;
            int int_to_binary = byte_i;
            for( int i = 0; i < 8; i++ ){
                current_bit = int_to_binary % 2;
                int_to_binary = int_to_binary / 2;

                if( current_bit ){
                    faults |= 1UL << (fault_byte*8 + i);
                }
            }

            fault_byte++;
        }	
    }

    return faults;
}