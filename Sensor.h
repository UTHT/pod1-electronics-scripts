#ifndef Sensor_H
#define Sensor_H

#include "Arduino.h"

// Sensor identifiers
enum sensor_t {
  OPT2002,
  VN200,
  PROSENSERTD,
  TMP006,
  MPU9250,
  DCT500,
  SPTD25_20_1000H,
  SPTD25_20_0200A,
  CANBUS
};

// Arduino identifiers
enum arduino_t {
  ARDUINO_ONE,
  ARDUINO_TWO,
  ARDUINO_THREE
};

// Degree of error occurred
// TODO: Maybe add more specific codes? (i.e. hardware/wiring fail, comms/protocol fail, etc.)
enum errorlevel_t {
  NONE,
  WARN,
  FAIL  //NOTE: Implies debug level 'DISABLED'
};

// General state - On fail != 0, what state did it last complete successfully?
enum debuglevel_t {
  //NOTE: The various debug states imply the value of cache.state.data as follows:
  DISABLED,     //NULL
  INIT,         //NULL
  // CALIBRATING,  //NULL if first time, otherwise unknown
  WAITING,      //NOT NULL, age > 0
  NEWREAD          //NOT NULL, age == 0
};

// Unimplemented for now...
// typedef struct SensorError {
//   errorlevel_t error,
//   char* msg,
//   uint8_t msglen
// } SensorError;
// typedef struct SensorDebug {
//   debuglevel_t debug,
//   char* msg,
//   uint8_t msglen
// } SensorError;

// Single datapoint
typedef struct t_datum {
  float data,
  char[3] units
} t_datum;

// All sensor state info + latest datum - used as the "cache" in the sensor object
// TODO: Replace state enums with state structs containing messages, etc.
typedef struct SensorState {
  errorlevel_t error,
  debuglevel_t debug,
  uint16_t age,       //How many milliseconds ago was this sensor data updated?
  t_datum* data,      //Array of datum
  uint8_t numdata
} SensorState;

// Wrapper for all sensor stuff - this is what gets passed to the main computer
typedef struct SensorData {
  sensor_t sensor,
  arduino_t arduino,
  SensorState state
} SensorData;

struct t_datasetup {
  uint8_t numdata,
  (const char*)* units
};

// Sensor class - note the difference in exposure and implementation in read/init and getRead/begin
class Sensor{
  private:
    uint16_t delta;         //The minimum time (milliseconds) between sensor updates.
    uint16_t lastread;      //The last time the sensor was read, milliseconds since program start.
  protected:
    // Sensor-specific functionality initialize and read functions - to be implemented by sensor classes
    virtual bool init() = 0;  //Connect to sensor, calibrate, set data count and units, and update state accordingly
    virtual void read() = 0;  //Get ALL data from the sensor hardware, update state (incl. data)

    // State and IDs
    SensorState state;   //Stores all the latest state data for this sensor.
    arduino_t arduino;
    sensor_t sensor;
  public:
    void update();  //Calls read(), manages delta, and wraps all t_datum
    void begin();   //Sets state in accordance with init()
    Sensor(sensor_t sensor, arduino_t arduino, t_datasetup setup, uint16_t delta);
};

#endif