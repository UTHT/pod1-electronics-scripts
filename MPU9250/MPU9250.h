#ifndef MPU9250_H
#define MPU9250_H

#include "../Sensor.h"

class MPU9250 : public Sensor {
    public:
        MPU9250(uint8_t pin, arduino_t arduino);
    private:
        errorlevel_t read(t_datum* data, uint8_t numdata) override;
        errorlevel_t init() override;
        /**
         * Analog input pin.
         * */
        uint8_t pin;
};

#endif