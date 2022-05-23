#ifndef UTHT_MAIN_H_
#define UTHT_MAIN_H_

#include <Arduino.h>

#include <sensors/sensor.h>
#include <actuators/actuator.h>
#include <utils/instructions.h>

// Settings
#define ENABLE_WATCHDOG false

// Constants
#define NUM_SENSORS   0
#define NUM_ACTUATORS 0
#define REVISION      0
#define BAUDRATE      115200

// Pins
#define PIN_STATUS          13

#define ADDRESS_MPU9250      0x68

// Sensors

Sensor* sensors[NUM_SENSORS] = { };

// Actuators

Actuator* actuators [NUM_ACTUATORS] = { };

const char* instr [NUM_ACTUATORS] = { };

static const InstructionActuatorMatrix matrix = {
  NUM_ACTUATORS,
  actuators,
  instr
};

#endif