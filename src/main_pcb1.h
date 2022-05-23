#ifndef UTHT_MAIN_H_
#define UTHT_MAIN_H_

#include <Arduino.h>

#include <sensors/sensor.h>
#include <actuators/actuator.h>
#include <utils/instructions.h>
#include <sensors/rtd.h>
#include <sensors/spt25_20_0200A.h>
#include <sensors/sptd25_20_1000h.h>
#include <sensors/tmp006.h>

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
RTD rtd = RTD(PIN_RTD);
SPT25_20_0200A spt25_20_0200a_1 = SPT25_20_0200A(PIN_SPT25_20_0200A_1);
SPT25_20_0200A spt25_20_0200a_2 = SPT25_20_0200A(PIN_SPT25_20_0200A_2);
SPTD25_20_1000H sptd25_20_1000h = SPTD25_20_1000H(PIN_SPTD25_20_1000H);
TMP006 tmp006 = TMP006(ADDRESS_TMP006);

Sensor* sensors [NUM_SENSORS] = {
  &rtd,
  &spt25_20_0200a_1,
  &spt25_20_0200a_2,
  &sptd25_20_1000h,
  &tmp006,
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