#ifndef UNIT_TEST

#if PCB == 1
  #include <main_pcb1.h>
#endif
#if PCB == 2
  #include <main_pcb2.h>
#endif
#if PCB == 3
  #include <main_pcb3.h>
#endif

#include <Arduino.h>

#if ENABLE_WATCHDOG
  #include <avr/wdt.h>
#endif

#include <sensors/sensor.h>
#include <actuators/actuator.h>
#include <utils/zcm.h>
#include <utils/zcm/zcm.h>
#include <utils/zcm/transport.h>
#include <utils/zcm/arduino_cobs_serial_transport.hpp>
#include <utils/zcm/channel_array_msg.h>

// Declarations
bool post(void);

// ZCM Interface
zcm_t* zcm_interface;

void setup(void) {
  // Status LED
  pinMode(PIN_STATUS, OUTPUT);
  digitalWrite(PIN_STATUS, LOW);

  zcm_interface = ZCM::create(0);

  // Serial communications established, initialize sensors and actuators
  if (!post()) {
    // Failed POST, blink
    while (true) {
      delay(250);
      digitalWrite(PIN_STATUS, HIGH);
      delay(250);
      digitalWrite(PIN_STATUS, LOW);
    }
  }

  // Enable the watchdog timer
  #if ENABLE_WATCHDOG
    wdt_enable(WDTO_1S);
  #endif
  digitalWrite(PIN_STATUS, HIGH);
}

void loop(void) {
  zcm_handle_nonblock(zcm_interface);

  for (int i = 0; i < NUM_SENSORS; ++i) {
    SensorState* state = sensors[i]->update();
    if (state->error == ERR_NONE) {
      if (state->debug == DS_SUCCESS) {
        for (int j = 0; j < state->numdata; ++j) {
          double values[state->numdata];

          // Concat units to string, split out data
          String units = "[";
          for(int x = 0; x < state->numdata; x++){
              values[x] = (double)state->data[x].value;
              units += state->data[x].label;
              if(x < state->numdata-1) {
                units += ", ";
              }
          }
          units += "]";
          
          ZCM::publish(zcm_interface, PCB, values, state->numdata, String(sensors[i]->getName()), units);
        }
      }
    }
  }
  for (int i = 0; i < NUM_ACTUATORS; ++i) {
    ActuatorState* state = actuators[i]->update();
  }

  #if ENABLE_WATCHDOG
    wdt_enable(WDTO_1S);
  #endif
}

bool post(void) {
  bool success = true;
  for (int i = 0; i < NUM_SENSORS; ++i) {
    SensorState* state = sensors[i]->begin();
    bool latest = (state->debug >= DS_INITIALIZED && state->error == ERR_NONE);
    success &= latest;
  }
  for (int i = 0; i < NUM_ACTUATORS; ++i) {
    ActuatorState* state = actuators[i]->begin();
    bool latest = (state->debug >= DS_INITIALIZED && state->error == ERR_NONE);
    success &= latest;
  }
  return success;
}

#endif