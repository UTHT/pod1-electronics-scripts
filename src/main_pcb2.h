#ifndef UTHT_MAIN_H_
#define UTHT_MAIN_H_

#include <Arduino.h>
#include "Serial_CAN_Module.h"

#include <sensors/sensor.h>
#include <actuators/actuator.h>
#include <sensors/opt2002.h>
#include <sensors/dct200.h>
// #include <sensors/mpu9250.h>
#include <interfaces/inverter.h>

// Settings
#define ENABLE_WATCHDOG false

// Constants
#define NUM_SENSORS   3
#define NUM_ACTUATORS 1
#define REVISION      0
#define BAUDRATE      115200
#define ENABLE_SERIAL 1
#define ENABLE_ZCM    0
#ifndef PCB
  #define PCB 2
#endif

// Pins
#define PIN_STATUS          13
#define PIN_OPT2002         A0
#define PIN_DCT200          A1

// #define ADDRESS_MPU9250     0x68

// Interfaces
Serial_CAN canbus = Serial_CAN();
Inverter inverter = Inverter(&canbus, Serial1);

// Sensors
OPT2002 opt2002 = OPT2002(PIN_OPT2002);
DCT200 dct200 = DCT200(PIN_DCT200);
// MPU9250 mpu9250 = MPU9250(ADDRESS_MPU9250);

Sensor* sensors [NUM_SENSORS] = { 
  &opt2002,
  &dct200,
  // &mpu9250,
  &inverter
};

// Actuators
Actuator* actuators [NUM_ACTUATORS] = {
  &inverter
};

#endif