#include <iostream>
#include "BROADCAST_MESSAGE.h"

using namespace std;

// fyi unsigned char CAN_mssg[9] = {CAN ID, data byte 1, data byte 2, ...}

void print_vsm_state(unsigned char CAN_mssg[9]) // TODO restructure like Fault Code File
{
    if (CAN_mssg[0] != 0xAA)
    {
        cout << "Message CAN ID is not 0xAA" << endl;
        return;
    }

    switch (CAN_mssg[2]) // CAN_mssg[1] should be 0
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

void print_inverter_state(unsigned char CAN_mssg[9]) // TODO restructure like Fault Code File
{
    if (CAN_mssg[0] != 0xAA)
    {
        cout << "Message CAN ID is not 0xAA" << endl;
        return;
    }

    switch (CAN_mssg[3])
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

// Disabling a specific broadcast message
// TODO ability to disable multiple broadcast messages at a time (Vector?, bitwise and param?)
void disable_broadcast_message(unsigned char (&CAN_mssg)[9], can_active_messages_lo_word content)
{
    if (CAN_mssg[1] != 148) // Data Byte 0
    {
        cout << "Message Parameter Address is not 148" << endl;
        return;
    }

    if (content < 8)
        CAN_mssg[5] &= ~(1U << content); // Clearing the corresponding bit in Data Byte 4
    else
        CAN_mssg[6] &= ~(1U << (content - 8)); // Clearing the corresponding bit in Data Byte 5
}