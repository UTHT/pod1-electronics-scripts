// RECV EXAMPLE OF SERIAL CAN MODULE - WITH MASK/FILTERING
// 
// SUPPORT: joney.sui@longan-labs.cc
//Second recv() test by Adam P

#include <Serial_CAN_Module_2.h>
#include <SoftwareSerial.h>

Serial_CAN can;

#define can_tx  10           //TX of Serial CAN Module connected to D10
#define can_rx  11           //RX of Serial CAN Module connected to D11

unsigned long mask[4] = 
{
    0, 0x0f,                // ext, maks 0
    0, 0x0f,                // ext, mask 1
};

unsigned long filt[12] = 
{
    0, 0xff,                // ext, filt 0
    0, 0xff,                // ext, filt 1
    0, 0xff,                // ext, filt 2
    0, 0xff,                // ext, filt 3
    0, 0xff,                // ext, filt 4
    0, 0xff,                // ext, filt 5
};

void setup()
{
    Serial.begin(9600);
    while(!Serial);
    can.begin(can_tx, can_rx, 9600);      // tx, rx

    if(can.setFilt(filt))
    {
        Serial.println("set filt ok");
    }
    else 
    {
        Serial.println("set filt fail");
    }
    
    if(can.setMask(mask))
    {
        Serial.println("set mask ok");
    }
    else
    {
        Serial.println("set mask fail");
    }

}

void loop()
{
    unsigned long id = 0;
    unsigned char dta[8];
    
    if(can.recv(&id, dta))
    {
        Serial.print("GET DATA FROM ID: ");
        Serial.println(id);
        for(int i=0; i<8; i++)
        {
            Serial.print("0x");
            Serial.print(dta[i], HEX);
            Serial.print('\t');
        }
        Serial.println();
    }
}

// END FILE
