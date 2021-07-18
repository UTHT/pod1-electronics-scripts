#ifndef Sensor_H
#define Sensor_H

#include "Base.h"

// Sensor identifiers
typedef enum sensors_t {
  S_OPT2002,
  S_VN200,
  S_PROSENSERTD,
  S_TMP006,
  S_MPU9250,
  S_DCT500,
  S_SPTD25_20_1000H,
  S_SPTD25_20_0200A,
  S_CANBUS
} sensors_t;

// Single datapoint
typedef struct t_datum {
  float data;
  const char* units;
} t_datum;

// All sensor state info + latest datum - used as the "cache" in the sensor object
// TODO: Replace state enums with state structs containing messages, etc.
typedef struct SensorState {
  errorlevel_t error;
  debuglevel_t debug;
  uint16_t timestamp; //Last data update (millis since start)
  t_datum* data;      //Array of datum
  uint8_t numdata;
} SensorState;

// Wrapper for all sensor stuff - this is what gets passed to the main computer
typedef struct SensorData {
  sensors_t sensor;
  arduino_t arduino;
  SensorState state;
} SensorData;

struct t_datasetup {
  uint8_t numdata;
  const char** units;
};

// Sensor class - note the difference in exposure and implementation in read/init and getRead/begin
class Sensor{
  private:
    unsigned long delta;         //The minimum time (milliseconds) between sensor updates.
    unsigned long lastread;      //Timestamp of last read ATTEMPT
  protected:
    // Sensor-specific functionality initialize and read functions - to be implemented by sensor classes
    virtual errorlevel_t init() = 0;                                  //Connect to sensor, calibrate, set data count and units, and update state accordingly
    virtual errorlevel_t read(t_datum* data, uint8_t numdata) = 0;    //Get ALL data from the sensor hardware -> data[i in numdata]

    // State and IDs
    SensorState state;   //Stores all the latest state data for this sensor.
  public:
    arduino_t arduino;
    sensors_t sensor;
    // Wrappers return pointer to updated state
    SensorState* update();  //Calls read(), manages delta, and wraps all t_datum
    SensorState* begin();   //Sets state in accordance with init()
    Sensor(sensors_t sensor, arduino_t arduino, t_datasetup setup, unsigned long delta);
};

#endif