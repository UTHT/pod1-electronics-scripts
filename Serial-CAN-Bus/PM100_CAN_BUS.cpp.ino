#include <Serial_CAN_Module_2.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include <PM100_CAN_BUS.h>


command_message::send_command_message( message_type command_message, 
                                       float torque_val, //Value in N.m
                                       float speed_val,  //RPM
                                       inverter_direction direction_command, 
                                       inverter_enable enable_command ){

    int inverter_torque_val;
    int data_high_byte;
    int data_low_byte;
    int direction_byte;
    int enable_byte;

    unsigned int can_data_bytes[8];
   

    if( command_message == COMMAND_TOURQUE_M ){
      
      //calculate the int values for byte 0 & byte 1 (torque data)
      inverter_torque_val = int(torque_val * 10);
      data_low_byte = inverter_torque_val % 256;
      data_high_byte = inverter_torque_val / 256;

      if( direction_command == REVERSE ){
        direction_byte = 0;
      }
      else if( direction_command == FORWARD ){
        direction_byte = 1;
      }
      else{
        //WE GOT A PROBLEM
      }


      if( enable_command == ENABLE ){
        enable_byte = 1;
      }
      else if( enable_command == DISABLE ){
        enable_byte = 0;
      }
      else{
        //WE GOT A PROBLEM
      }


      //Finally, format the CAN message data bytes
      can_data_bytes[8] = { data_low_byte, data_high_byte, 0, 0, direction_byte, enable_byte, 0, 0 };

    }
    





    else if( command_message == COMMAND_SPEED_M ){

      data_low_byte = speed_val % 256;
      data_high_byte = speed_val / 256;

      if( direction_command == REVERSE ){
        direction_byte = 0;
      }
      else if( direction_command == FORWARD ){
        direction_byte = 1;
      }
      else{
        //WE GOT A PROBLEM
      }


      if( enable_command == ENABLE ){
        enable_byte = 1;
      }
      else if( enable_command == DISABLE ){
        enable_byte = 0;
      }
      else{
        //WE GOT A PROBLEM
      }


      //Finally, format the CAN message data bytes
      can_data_bytes[8] = { 0, 0, data_low_byte, data_high_byte, direction_byte, enable_byte, 0, 0 };
      
    }
   
}
