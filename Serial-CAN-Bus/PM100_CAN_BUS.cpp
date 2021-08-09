//#include <Serial_CAN_Module_2.h>
//#include <SoftwareSerial.h>
//#include <Arduino.h>
#include <iostream>
#include <PM100_CAN_BUS_testing.h>
//#include <C:\adam\utht\extra low level\PM100_CAN_BUS_testing.h>

using namespace std;


int* command_message::send_command_message( message_type command_message, 
                                       float torque_val, //Value in N.m
                                       float speed_val,  //RPM
                                       inverter_direction direction_command, 
                                       inverter_enable enable_command ){

    int inverter_torque_val;
    int data_high_byte;
    int data_low_byte;
    int direction_byte;
    int enable_byte;

    //int can_data_bytes[8];
   

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
        //WE GOT A PROBLEM, should not be here
        cout << "Problem!" << endl;
      }


      if( enable_command == ENABLE ){
        enable_byte = 1;
      }
      else if( enable_command == DISABLE ){
        enable_byte = 0;
      }
      else{
        //WE GOT A PROBLEM
        cout << "Problem, no enable command!" << endl;
      }
      

      //Finally, format the CAN message data bytes
      static int can_data_bytes[8] = { data_low_byte, data_high_byte, 0, 0, direction_byte, enable_byte, 0, 0 };
      
      /*
      cout << "Printintg CAN Message inside the function here:..." << endl;
      for ( int i = 0; i < 8; i++ ) {
	      
	      cout << can_data_bytes[i] << endl;
	      
	   }
	   */
      
	  return can_data_bytes;
		
    }
    


	/*


    else if( command_message == COMMAND_SPEED_M ){

      data_low_byte = int(speed_val) % 256;
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
      int can_data_bytes[8] = { 0, 0, data_low_byte, data_high_byte, direction_byte, enable_byte, 0, 0 };
      
      cout << "Printintg CAN Message inside the function here WRONG ONE:..." << endl;
      for ( int i = 0; i < 8; i++ ) {
	      
	      cout << can_data_bytes[i] << endl;
	      
	   }
      
	
      
      return can_data_bytes;
      
    }
    
    */
   
}



int main(){
	
	float torque_val;
	
	cout << "Hello" << endl;
	cout << "Please enter a Torque Value:";
	cin >> torque_val;
	
	cout << torque_val;
	
	command_message test_message; //class
	message_type test_type = COMMAND_TOURQUE_M; //specify command type
	inverter_direction test_direction = FORWARD; //inverter direction
	inverter_enable test_enable = ENABLE; //inverter enable
	
	//test command message
	//pass all the necessary parameters needed to make CAN Message
	int *CAM_mssg = test_message.send_command_message( test_type, torque_val, 0, test_direction, test_enable );
	
	cout << "CAN Message generated: " << endl;
	for ( int i = 0; i < 8; i++ ) {
      cout << CAM_mssg[i] << endl;
   }
	
	
	
	
}
