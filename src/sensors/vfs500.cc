#include <sensors/vfs500.h>

#include <Arduino.h>

#include <utils/base.h>
#include <sensors/sensor.h>

static const char* labels[2] = {
  "Hz", 
  "GPM"
};

static const SensorDataSetup datasetup = {
  .numdata = 2, 
  .labels = labels
};

VFS500::VFS500(uint8_t pin) : Sensor(SENSOR_VFS500, VFS500_NAME, &datasetup, 250) {
    this->pin = pin;
}

errorlevel_t VFS500::initialize(void) {
    pinMode(pin, INPUT);
    return ERR_NONE;
}

errorlevel_t VFS500::read(float* data, uint8_t numdata) {
    unsigned long ontime = pulseIn(pin, HIGH);
    unsigned long offtime = pulseIn(pin, LOW);
    unsigned long period = ontime + offtime;

    unsigned long freq = 1000000.0 / period;
    // unsigned long duty = (ontime / period) * 100;
    
    float flow = (freq - VFS500_IN_MIN) / (VFS500_IN_MAX - VFS500_IN_MIN) * (VFS500_OUT_MAX - VFS500_OUT_MIN) + VFS500_OUT_MIN;
    
    if(flow <= 0.26) {
        return ERR_FATAL;        //Flow is at or below minimum value of 0.26 gpm
    }
    data[0] = (float)freq;
    data[1] = (float)flow;

    // TODO: other error conditions?
    return ERR_NONE;
}
