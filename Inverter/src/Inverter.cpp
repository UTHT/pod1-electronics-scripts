#include "Actuator.h"
#include "Inverter.h"

Inverter::Inverter(Serial_CAN* CANBus, arduino_t arduino) : Actuator(A_INVERTER, arduino, 0) {
    this->CANBus = CANBus;
}

int* create_command_message (message_type command, float value, inverter_direction direction, inverter_enable enable){
    int data_high_byte;
    int data_low_byte;

    if(command == COMMAND_TOURQUE_M){
        if(value >= 0){
            data_low_byte = int(value * 10) % 256;
            data_high_byte = int(value * 10) / 256;
        } else{
            data_low_byte = ( int(value * 10) & 255 );
            data_high_byte = ( int(value * 10) & 65280 ) >> 8;
        }
        
        static int can_data_bytes[8] = { data_low_byte, data_high_byte, 0, 0, direction, enable, 0, 0 };
            
        return can_data_bytes;	
    } else if(command == COMMAND_SPEED_M){

        data_low_byte = int(value) % 256;
        data_high_byte = value / 256;

        static int can_data_bytes[8] = { 0, 0, data_low_byte, data_high_byte, direction, enable, 0, 0 };

        return can_data_bytes;
    }
}