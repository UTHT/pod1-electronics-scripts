#include <interfaces/interface.h>

Interface::Interface(interfaceid_t id, const char* name, const SensorDataSetup* setup, uint32_t delta, float failtarget) :
  Sensor(SENSOR_NULL, name, setup, delta), 
  Actuator(ACTUATOR_NULL, name, failtarget) {
  
}