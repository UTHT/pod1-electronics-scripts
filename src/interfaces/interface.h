#ifndef UTHT_INTERFACES_INTERFACE_H_
#define UTHT_INTERFACES_INTERFACE_H_

#include <Arduino.h>

#include <utils/base.h>
#include <sensors/sensor.h>
#include <actuators/actuator.h>

// Interface identifiers
typedef enum interfaceid_t {
  INTERFACE_NULL,
  INTERFACE_INVERTER
} interfaceid_t;

class Interface : public Sensor, public Actuator {
  public:
    Interface(interfaceid_t id, const char* name, const SensorDataSetup* setup, uint32_t delta, float failtarget);
};

#endif