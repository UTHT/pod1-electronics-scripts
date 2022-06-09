#ifndef UTHT_ZCM_H_
#define UTHT_ZCM_H_

#include <utils/zcm/arduino_cobs_serial_transport.hpp>
#include <utils/zcm/transport.h>
#include <utils/zcm/channel_array_msg.h>

namespace ZCM {
  zcm_t* create(int port);
  int publish(zcm_t* zcm, uint8_t arduino, double* values, uint8_t numData, String sensorName, String units);
}

#endif