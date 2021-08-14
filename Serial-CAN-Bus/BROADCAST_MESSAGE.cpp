#include <iostream>
#include "BROADCAST_MESSAGE.h"

using namesapce std;

// fyi unsigned char message_arr[9] = {CAN ID, data byte 1, data byte 2, ...}

void print_vsm_state(unsigned char message_arr[9])
{
    if (message_arr[0] != 0xAA)
    {
        cout << "Message CAN ID is not 0xAA" << endl;
        return;
    }

    switch (message_arr[2]) // message_arr[1] should be 0
    {
    case 0:
        cout << "VSM State: VSM Start State";
        break;
    case 1:
        cout << "VSM State: Pre-charge Init State";
        break;
    case 2:
        cout << "VSM State: Pre-charge Active State";
        break;
    case 3:
        cout << "VSM State: Pre-charge Complete State";
        break;
    case 4:
        cout << "VSM State: VSM Wait State";
        break;
    case 5:
        cout << "VSM State: VSM Ready State";
        break;
    case 6:
        cout << "VSM State: Motor Running State";
        break;
    case 7:
        cout << "VSM State: Blink Fault Code State";
        break;
    case 14:
        cout << "VSM State: Shutdown in Process";
        break;
    case 15:
        cout << "VSM State: Recycle Power State";
        break;
    default:
        cout << "Invalid VSM State";
    }
}

void print_inverter_state(unsigned char message_arr[9])
{
    if (message_arr[0] != 0xAA)
    {
        cout << "Message CAN ID is not 0xAA" << endl;
        return;
    }

    switch (message_arr[3])
    {
    case 0:
        cout << "Inverter State: Power on State";
        break;
    case 1:
        cout << "Inverter State: Stop State";
        break;
    case 2:
        cout << "Inverter State: Open Loop State";
        break;
    case 3:
        cout << "Inverter State: Closed Loop State";
        break;
    case 4:
        cout << "Inverter State: Wait State";
        break;
    case 8:
        cout << "Inverter State: Idle Run State";
        break;
    case 9:
        cout << "Inverter State: Idle Stop State";
        break;
    default:
        cout << "Invalid Inverter State";
    }
}