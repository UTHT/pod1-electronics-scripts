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
	
    //All necessary variables for CAN Message (data bytes)
    int inverter_torque_val;
    int data_high_byte;
    int data_low_byte;
    int direction_byte;
    int enable_byte;

    //There are 2 types of CAN Command Messages to generate:
    //Torque Message code below
    if( command_message == COMMAND_TOURQUE_M ){
	    
	//Calculate the int values for byte 0 & byte 1 (torque data)
	//PM100 requires (Torque Val * 10) to be sent through CAN
	inverter_torque_val = int(torque_val * 10);
	data_low_byte = inverter_torque_val % 256;
	data_high_byte = inverter_torque_val / 256;
	
	//Set direction command byte accordingly
	if( direction_command == REVERSE ){ direction_byte = 0; }
	else if( direction_command == FORWARD ){ direction_byte = 1; }
	else{
	    //SHOULD NOT BE HERE, raise ERROR
	    cout << "--ERROR-- Un-identified Inverter Direction Command" << endl;
	}
	
	//Set inverter enable byte accordingly
	if( enable_command == ENABLE ){ enable_byte = 1; }
	else if( enable_command == DISABLE ){ enable_byte = 0; }
	else{
	    //SHOULD NOT BE HERE, raise ERROR
	    cout << "--ERROR-- Un-identified Inverter Enable Command" << endl;
	}
	      
	
	//Finally, format the CAN message data bytes
	//Represented as int array
	static int can_data_bytes[8] = { data_low_byte, data_high_byte, 0, 0, direction_byte, enable_byte, 0, 0 };
	      
	/* DEBUG---PRINT OUT CAN DATA BYTES----------------------------------
	cout << "Printintg CAN Message inside the function here:..." << endl;
	for ( int i = 0; i < 8; i++ ) {
	    cout << can_data_bytes[i] << endl;
	}-------------------------------------------------------------------
	*/
	      
	return can_data_bytes;	
    }
    


	


    else if( command_message == COMMAND_SPEED_M ){


        data_low_byte = int(speed_val) % 256;
        data_high_byte = speed_val / 256;


        if( direction_command == REVERSE ){ direction_byte = 0; }
        else if( direction_command == FORWARD ){ direction_byte = 1; }
        else{
            //SHOULD NOT BE HERE, raise ERROR
            cout << "--ERROR-- Un-identified Inverter Direction Command" << endl;
        }

        if( enable_command == ENABLE ){ enable_byte = 1; }
        else if( enable_command == DISABLE ){ enable_byte = 0; }
        else{
            //SHOULD NOT BE HERE, raise ERROR
            cout << "--ERROR-- Un-identified Inverter Enable Command" << endl;
        }


        //Finally, format the CAN message data bytes
        static int can_data_bytes[8] = { 0, 0, data_low_byte, data_high_byte, direction_byte, enable_byte, 0, 0 };
      
        /* DEBUG---PRINT OUT CAN DATA BYTES----------------------------------
        cout << "Printintg CAN Message inside the function here:..." << endl;
        for ( int i = 0; i < 8; i++ ) {
	    cout << can_data_bytes[i] << endl;
	}-------------------------------------------------------------------
	*/
      

        return can_data_bytes;
    }
    
    
   
}



int main(){
	
	char  speed_or_torque;
	char  inv_direction;
	float torque_val = 0;
	float speed_val = 0;
	
	//These variables are all custom made
	command_message test_message; //message class
	message_type test_type;
	inverter_direction test_direction;
	inverter_enable test_enable = ENABLE; //inverter enable
	
	
	cout << "-----------------------------------------------------------------------------" << endl;
	cout << "|                  PM100 CAN Bus Message Generator (TESTING)                |" << endl;
	cout << "|                                                                           |" << endl;
	cout << "|                                                                           |" << endl;
	cout << "|                                                                           |" << endl;
	cout << "-----------------------------------------------------------------------------" << endl;
	cout << "\nWelcome :) \n" << endl;
	cout << "Do you want to make a Speed or Torque Command Message?" << endl;
	cout << " _______________________________" << endl;
	cout << "| [s] for Speed, [t] for Torque |" << endl; 
	cout << "|_______________________________|" << endl;
	cout << ">";
	cin >> speed_or_torque;
	
	if( speed_or_torque == 's' || speed_or_torque == 'S' ){
		
		test_type = COMMAND_SPEED_M;
		cout << "\nPlease enter a Speed Value: (RPM)" << endl;
		cout << ">";
		cin >> speed_val;
	}
	else if( speed_or_torque == 't' || speed_or_torque == 'T' ){
		
		test_type = COMMAND_TOURQUE_M;
		cout << "\nPlease enter a Torque Value: (N*m)" << endl;
		cout << ">";
		cin >> torque_val;
	}
	
	cout << "\nNow, specify the Inverter's Direction:" << endl;
	cout << " ____________________________________________" << endl;
	cout << "| [f] for FORWARD (default), [r] for REVERSE |" << endl; 
	cout << "|____________________________________________|" << endl;
	cout << ">";
	cin >> inv_direction;
	
	if( inv_direction == 'f' || inv_direction == 'F' ){ test_direction = FORWARD; }
	else if( inv_direction == 'r' || inv_direction == 'R' ){ test_direction = REVERSE; }
	
	

	
	//test command message
	//pass all the necessary parameters needed to make CAN Message
	int *CAM_mssg = test_message.send_command_message( test_type, torque_val, speed_val, test_direction, test_enable );
	
	//Printing out CAN Message (data bytes)
	cout << "\n _______________________________________" << endl;
	cout <<   "| CAN Message generated:                |" << endl;
	cout <<   "| { ";
	for ( int i = 0; i < 8; i++ ) {
            cout << CAM_mssg[i];
	    if( i != 7 ){ cout << ", "; }
        }
        cout << " }" << endl;
        cout << "|                                       |" << endl;
        cout << "|_______________________________________|" << endl;
   

	
}

