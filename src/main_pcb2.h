#ifndef UTHT_MAIN_H_
#define UTHT_MAIN_H_

#include <Arduino.h>

#include <sensors/sensor.h>
#include <actuators/actuator.h>
#include <utils/instructions.h>
#include <sensors/opt2002.h>
#include <sensors/mpu9250.h>

// Settings
#define ENABLE_WATCHDOG false

// Constants
#define NUM_SENSORS   2
#define NUM_ACTUATORS 0
#define REVISION      0
#define BAUDRATE      115200

// Pins
#define PIN_STATUS          13
#define PIN_OPT2002         A0

#define ADDRESS_MPU9250      0x68

// Sensors
OPT2002 opt2002 = OPT2002(PIN_OPT2002);
MPU9250 mpu9250 = MPU9250(ADDRESS_MPU9250);

Sensor* sensors [NUM_SENSORS] = { 
  &opt2002,
  &mpu9250,
};

// Actuators

Actuator* actuators [NUM_ACTUATORS] = { };

const char* instr [NUM_ACTUATORS] = { };

static const InstructionActuatorMatrix matrix = {
  NUM_ACTUATORS,
  actuators,
  instr
};

#endif