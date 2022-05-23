#ifndef UTHT_MAIN_H_
#define UTHT_MAIN_H_

#include <Arduino.h>

#include "sensors/sensor.h"
#include "actuators/actuator.h"
#include "utils/instructions.h"
#include "sensors/sptd25_20_1000h.h"

// Settings
#define ENABLE_WATCHDOG false

// Constants
#define NUM_SENSORS   5
#define NUM_ACTUATORS 0
#define REVISION      0
#define BAUDRATE      115200

// Pins
#define PIN_STATUS            13
#define PIN_RTD               A0
#define PIN_SPT25_20_0200A_1  A1
#define PIN_SPT25_20_0200A_2  A2
#define PIN_SPTD25_20_1000H   A3

#define ADDRESS_TMP006        0x40

// Sensors
SPTD25_20_1000H sptd25_20_1000h = SPTD25_20_1000H(PIN_SPTD25_20_1000H);

Sensor* sensors[NUM_SENSORS] = {
  &sptd25_20_1000h,
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