#ifndef SPTD25_20_1000H_H
#define SPTD25_20_1000H_H

#include "Sensor.h"

class SPTD25_20_1000H : public Sensor {
    public:
        SPTD25_20_1000H(uint8_t pin, arduino_t arduino);
    private:
        errorlevel_t read(t_datum* data, uint8_t numdata) override;
        errorlevel_t init() override;
        /**
         * Analog input pin.
         * */
        uint8_t pin;
};

#endif