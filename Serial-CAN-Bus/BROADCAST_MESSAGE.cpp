#include <iostream>
//#include <C:\\adam\\utht\\extra low level\\BROADCAST_MESSAGE.h>
#include <BROADCAST_MESSAGE.h>

using namespace std;

std::ostream &operator<<(std::ostream &lhs, vsm_state state)
{
    switch (state) {
        case VSM_START: lhs << "VSM Start State"; break;
        case PRE_CHARGE_INIT: lhs << "Pre-Charge Init State"; break;
        case PRE_CHARGE_ACTIVE: lhs << "Pre-Charge Active State"; break;
        case PRE_CHARGE_COMPLETE: lhs << "Pre-Charge Complete State"; break;
        case VSM_WAIT: lhs << "VSM Wait State"; break;
        case VSM_READY: lhs << "VSM Ready State"; break;
        case MOTOR_RUNNING: lhs << "Motor Running State"; break;
        case BLINK_FAULT_CODE: lhs << "Blink Fault Code State"; break;
        case SHUTDOWN_IN_PROGRESS: lhs << "Shutdown in Process"; break;
        case RECYCLE_POWER: lhs << "Recycle Power State"; break;
        case NOT_A_VSM_STATE: lhs << "ERROR - NOT a VSM State - Invalid Value"; break;
    }

    return lhs;
}

int return_vsm_state_val(vsm_state state)
{
    int index_value = state;
    return index_value;
}

vsm_state broadcast_message::read_vsm_state(int CAN_mssg[9])
{
    vsm_state state;

    if (CAN_mssg[0] != 0x0AA)
    {
        cout << "Message CAN ID is not 0x0AA, this is NOT a VSM message" << endl;
        return state = NOT_A_VSM_STATE;
    }

    switch (CAN_mssg[2])
    {
    case 0:
        cout << "VSM State: VSM Start State";
        state = VSM_START; break;
    case 1:
        cout << "VSM State: Pre-charge Init State";
        state = PRE_CHARGE_INIT; break;
    case 2:
        cout << "VSM State: Pre-charge Active State";
        state = PRE_CHARGE_ACTIVE;  break;
    case 3:
        cout << "VSM State: Pre-charge Complete State";
        state = PRE_CHARGE_COMPLETE; break;
    case 4:
        cout << "VSM State: VSM Wait State";
        state = VSM_WAIT; break;
    case 5:
        cout << "VSM State: VSM Ready State";
        state = VSM_READY; break;
    case 6:
        cout << "VSM State: Motor Running State";
        state = MOTOR_RUNNING; break;
    case 7:
        cout << "VSM State: Blink Fault Code State";
        state = BLINK_FAULT_CODE; break;
    case 14:
        cout << "VSM State: Shutdown in Process";
        state = SHUTDOWN_IN_PROGRESS; break;
    case 15:
        cout << "VSM State: Recycle Power State";
        state = RECYCLE_POWER; break;
    default:
        cout << "Invalid VSM State";
        state = NOT_A_VSM_STATE;
    }

    return state;
}

std::ostream &operator<<(std::ostream &lhs, inverter_state state)
{
    switch (state) {
        case POWER_ON: lhs << "Power On State"; break;
        case STOP: lhs << "Stop State"; break;
        case OPEN_LOOP: lhs << "Open Loop State"; break;
        case CLOSED_LOOP: lhs << "Closed Loop State"; break;
        case WAIT: lhs << "Wait State"; break;
        case IDLE_RUN: lhs << "Idle Run State"; break;
        case IDLE_STOP: lhs << "Idle Stop State"; break;
        case NOT_AN_INVERTER_STATE: lhs << "ERROR - NOT an Inverter State - Invalid Value"; break;
    }

    return lhs;
}

int return_inverter_state_val(inverter_state state)
{
    int index_value = state;
    return index_value;
}

inverter_state broadcast_message::read_inverter_state(int CAN_mssg[9])
{
    inverter_state state;

    if (CAN_mssg[0] != 0x0AA)
    {
        cout << "Message CAN ID is not 0xAA" << endl;
        return state = NOT_AN_INVERTER_STATE;
    }

    switch (CAN_mssg[3])
    {
    case 0:
        cout << "Inverter State: Power on State" << endl;
        state = POWER_ON; break;
    case 1:
        cout << "Inverter State: Stop State" << endl;
        state = STOP; break;
    case 2:
        cout << "Inverter State: Open Loop State" << endl;
        state = OPEN_LOOP; break;
    case 3:
        cout << "Inverter State: Closed Loop State" << endl;
        state = CLOSED_LOOP; break;
    case 4:
        cout << "Inverter State: Wait State" << endl;
        state = WAIT; break;
    case 8:
        cout << "Inverter State: Idle Run State" << endl;
        state = IDLE_RUN; break;
    case 9:
        cout << "Inverter State: Idle Stop State" << endl;
        state = IDLE_STOP; break;
    default:
        cout << "Invalid Inverter State" << endl;
        state = NOT_AN_INVERTER_STATE; break;
    }

    return state;
}

/* Disable one or more messages at a time
 * Example usage: disable_broadcast_message(test_msg, TEMPERATURE_2 | CURRENT_INFORMATION | DIAG_DATA);
 */
void disable_broadcast_message(int CAN_mssg[9], int listOfMsgs)
{
    if (CAN_mssg[0] != 0x0C1) // CAN ID
    {
        cout << "Message CAN Address is not 0x0C1" << endl;
        return;
    }

    if (CAN_mssg[1] != 148) // Data Byte 0
    {
        cout << "Message Parameter Address is not 148" << endl;
        return;
    }

    int loWord = (CAN_mssg[6] << 8) | CAN_mssg[5]; // loWord = [6] concatenated with [5]

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
    CAN_mssg[5] = loWord & 0xFF;
    CAN_mssg[6] = (loWord >> 8) & 0xFF;
}

int main()
{
    cout << "Testing print_vms_state function: " << endl;

    broadcast_message testing_class;
    vsm_state CAN_MSSG_VSM_STATE;

    int test_CAN_message[9] = {0x0AA, 0, 1, 0, 0, 0, 0, 0, 0};

    cout << "CAN Message--------------------" << endl;
    cout << "{";
    for (int i = 0; i < 9; i++)
    {

        cout << " " << test_CAN_message[i];
        if (i != 8)
        {
            cout << ",";
        }
    }
    cout << " }\n" << endl;

    CAN_MSSG_VSM_STATE = testing_class.read_vsm_state(test_CAN_message);
    cout << "\n\nThe VSM State retrieved from the CAN Message is: " << CAN_MSSG_VSM_STATE << endl;
    cout << "Value of VSM State: " << return_vsm_state_val(CAN_MSSG_VSM_STATE) << endl;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    cout << "\n\n\nTesting print_inverter_state function: " << endl;
    inverter_state CAN_MSSG_INVERTER_STATE;

    int test_CAN_message_2[9] = {0x0AA, 0, 0, 2, 0, 0, 0, 0, 0};

    cout << "CAN Message--------------------" << endl;
    cout << "{";
    for (int i = 0; i < 9; i++)
    {

        cout << " " << test_CAN_message_2[i];
        if (i != 8)
        {
            cout << ",";
        }
    }
    cout << " }\n" << endl;

    CAN_MSSG_INVERTER_STATE = testing_class.read_inverter_state(test_CAN_message_2);
    cout << "\nThe Inverter State retrieved from the CAN Message is: " << CAN_MSSG_INVERTER_STATE << endl;
    cout << "Value of Inverter State: " << return_inverter_state_val(CAN_MSSG_INVERTER_STATE) << endl;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    cout << "\n\n\nTesting disable_broadcast_message function: " << endl;

    int test_msg[9] = {0x0C1, 148, 0, 1, 0, 0xFF, 0xFF, 0xFF, 0xFF};

    cout << "CAN Message--------------------" << endl;
    cout << "{ " << test_msg[0];
    for (int i = 1; i < 9; i++)
        cout << ", " << test_msg[i];
    cout << " }\n" << endl;

    disable_broadcast_message(test_msg, TEMPERATURE_2 | DIAG_DATA);

    cout << "New CAN Message--------------------" << endl;
    cout << "{ " << test_msg[0];
    for (int i = 1; i < 9; i++)
        cout << ", " << test_msg[i];
    cout << " }\n" << endl;
}