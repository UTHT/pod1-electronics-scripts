// SEND EXAMPLE OF SERIAL CAN MODULE
// unsigned char send(unsigned long id, uchar ext, uchar rtrBit, uchar len, const uchar *buf);
// SUPPORT: joney.sui@longan-labs.cc
//First send() test by Adam P

#include <Serial_CAN_Module_2.h>
#include <SoftwareSerial.h>

Serial_CAN can;

#define can_tx  10           //TX of Serial CAN Module connected to D10
#define can_rx  11           //RX of Serial CAN Module connected to D11


void setup()
{
    Serial.begin(9600);                   //Baud Rates: 9600, 19200, 38400, 57600, 74880, 115200
                                          //9600 works the best, higher rates start to have problems
    can.begin(can_tx, can_rx, 9600);      
    Serial.println("Starting:");
    //can.debugMode();
}


unsigned char dta[8] = {1, 2, 3, 4, 5, 10, 11, 12};


// send(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf);
void loop()
{
    can.send(0x77, 0, 0, 8, dta);   // SEND TO ID:0X77
                                    // CAN ID is not important for testing, all frames will be recieved
    delay(100);
    //can.debugMode();
}

// END FILE
