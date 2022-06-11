#ifndef UTHT_MAIN_H_
#define UTHT_MAIN_H_

#include <Arduino.h>

#include <sensors/sensor.h>
#include <actuators/actuator.h>
#include <sensors/rtd.h>
#include <sensors/vfs500.h>
#include <actuators/relay.h>
#include <actuators/brakes.h>

// Settings
#define ENABLE_WATCHDOG false

// Constants
#define NUM_SENSORS   3
#define NUM_ACTUATORS 2
#define REVISION      0
#define BAUDRATE      115200
#define ENABLE_SERIAL 1
#define ENABLE_ZCM    0
#ifndef PCB
  #define PCB 3
#endif

// Pins
#define PIN_STATUS          13
#define PIN_RTD_1           A0
#define PIN_RTD_2           A1
#define PIN_VFS             A2
#define PIN_RELAY           2
#define PIN_BRAKES          3

// Sensors
RTD rtd_1 = RTD(PIN_RTD_1);
RTD rtd_2 = RTD(PIN_RTD_2);
VFS500 vfs500 = VFS500(PIN_VFS);

Sensor* sensors[NUM_SENSORS] = { 
  &rtd_1,
  &rtd_2,
  &vfs500,
};

// Actuators
Relay relay = Relay(PIN_RELAY);
Brakes brakes = Brakes(PIN_BRAKES);

Actuator* actuators [NUM_ACTUATORS] = { 
  &relay,
  &brakes,
};

#endif