#ifndef BASE_H
#define BASE_H

#include "Arduino.h"
#include "stdlib.h"

// Arduino identifiers
typedef enum arduino_t {
  ARDUINO_ONE,
  ARDUINO_TWO,
  ARDUINO_THREE
} arduino_t;

// Degree of error occurred
// TODO: Maybe add more specific codes? (i.e. hardware/wiring fail, comms/protocol fail, etc.)
typedef enum errorlevel_t {
  ERR_NONE,
  ERR_WARN,
  ERR_FAIL  //NOTE: Implies debug level 'DISABLED'
} errorlevel_t;

// General state - On fail != 0, what state did it last complete successfully?
typedef enum debuglevel_t {
  //NOTE: The various debug states imply the value of cache.state.data as follows:
  DS_DISABLED,     //NULL
  DS_INIT,         //NULL
  // DS_CALIBRATING,  //NULL if first time, otherwise unknown
  DS_WAITING,      //NOT NULL, age > 0
  DS_NEWREAD          //NOT NULL, age == 0
} debuglevel_t;

#endif