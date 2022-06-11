#include <interfaces/inverter.h>

#include <Arduino.h>
#include "Serial_CAN_Module.h"

#include <utils/base.h>
#include <actuators/actuator.h>

static const char* labels[2] = {
  "Inverter State",
  "VSM State"
};

static const SensorDataSetup datasetup = {
  .numdata = 2,
  .labels = labels
};

Inverter::Inverter(Serial_CAN* canbus, uint8_t tx, uint8_t rx) : Interface(INTERFACE_INVERTER, INVERTER_NAME, &datasetup, INVERTER_DELTA, INVERTER_FAILTARGET) {
  this->canbus = canbus;
  canbus->begin(tx, rx, INVERTER_BAUD);
}

errorlevel_t Inverter::initialize(void) {
  return ERR_NONE;
}

errorlevel_t Inverter::set(float target) {
  if (target < -32768 || target > 32767 || target > INVERTER_RPM_MAX) {
    return ERR_WARNING;
  }

  this->rpm = target;
  
  return ERR_NONE;
}

errorlevel_t Inverter::read(float* data, uint8_t numdata) {
  unsigned long id = 0;
  unsigned char buffer[8];

  if(canbus->recv(&id, buffer)) {
    // Data recieved!

    if (id == 0xAA) {
      // Internal states
      data[0] = inv_state = buffer[2];
      data[1] = vsm_state = buffer[0];
      
      // Handle states
      switch (vsm_state) {
        case 0: // VSM Start State
            break;
        case 1: // Pre-charge Init State
        case 2: // Pre-charge Active State
        case 3: // Pre-charge Complete State
            break;
            
        // ***** start sending CAN Command Messages to controller before state 4 (before inverter is powered on) (CAN Protocol p32) *****
        
        case 4: // VSM Wait State -> To power up PM100, turn ignition to ON position,
                // state then checks brake is active and only Forward or Reverse switch is active
            disableLockout();
            break;
        case 5: // VSM Ready State -> Should automatically transition to next state if no faults
            disableLockout();
            break;
        case 6: // Motor Running State -> Can now change from torque to speed command mode
            // Keep setting/changing speed and torque (apparently torque should be input, but not sure why)
            // example in CAN Protocol p14
            sendSpeedCommand();
            break;
        case 7: // Blink Fault Code State
            break;
        case 14: // Shutdown in Process – in key switch mode 1, user has turned the key switch to off position.
            break;
        case 15: // Recycle Power State – user must recycle power when the unit is in this state.
            break;
      }
    }

    return ERR_NONE;
  }

  // Workaround - prevents updating "new data" flag
  return ERR_WARNING;
}

void Inverter::disableLockout(void) {
  unsigned char msg[8] = {0,0,0,0,0,0,0,0};
  canbus->send(0xC0, 0, 0, 8, msg);
}

void Inverter::sendSpeedCommand(void) {
  int trq_command = INVERTER_TORQUE * 10;
  char trq_lo = (trq_command & 0x00FF);
  char trq_hi = (trq_command & 0xFF00) >> 8;
  char spd_lo = (rpm & 0x00FF);
  char spd_hi = (rpm & 0xFF00) >> 8;
  unsigned char msg[8] = { trq_lo, trq_hi, spd_lo, spd_hi, 1, 5, 0, 0 }; // torque lo & hi, speed lo & hi, torque direction (forward), speed mode (0x4) | inverter on (0x1)

  canbus->send(0xC0, 0, 0, 8, msg);
}