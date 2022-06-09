#include <utils/zcm.h>

#include <utils/zcm/arduino_cobs_serial_transport.hpp>
#include <utils/zcm/transport.h>
#include <utils/zcm/channel_array_msg.h>


zcm_t* ZCM::create(int port) {
    zcm_trans_t* zcm_trans = arduino_transport_create(port);
    return zcm_create_from_trans(zcm_trans);
}

int ZCM::publish(zcm_t* zcm, uint8_t arduino, double* values, uint8_t numData, String sensorName, String units) {    
    channel_array message;
    message.arduino_id = arduino;
    message.sensor = (char *) sensorName.c_str();
    message.data = values;
    message.units = (char *) units.c_str();
    message.sz = (int64_t)numData;

    return channel_array_publish(zcm, sensorName.c_str(), &message);
}