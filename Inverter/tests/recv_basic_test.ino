// RECV EXAMPLE OF SERIAL CAN MODULE
// unsigned char recv(unsigned long *id, uchar *buf);
// SUPPORT: joney.sui@longan-labs.cc
//First recv() test by Adam P

#include <Serial_CAN_Module_2.h>
#include <SoftwareSerial.h>

Serial_CAN can;

#define can_tx  10           //TX of Serial CAN Module connected to D10
#define can_rx  11           //RX of Serial CAN Module connected to D11


void setup()
{
    Serial.begin(9600);
    while(!Serial);
    can.begin(can_tx, can_rx, 9600);      

    if(can.canRate(CAN_RATE_500))
    {
        Serial.println("Set CAN rate OK");
    }
    else
    {
        Serial.println("Set CAN rate FAIL");
    }
}

unsigned long id = 5;
unsigned char dta[8];

// send(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf);
void loop()
{

    can.debugMode();
    if(can.recv(&id, dta))
    {
        Serial.print("GETTING DATA FROM ID: ");
        Serial.println(id);
        for(int i=0; i<8; i++)
        {
            Serial.print("0x");
            Serial.print(dta[i], HEX);
            Serial.print('\t');
        }
        Serial.println();
    }
    else{
      //Serial.print("Didn't work!!!");
    }
}

// END FILE
