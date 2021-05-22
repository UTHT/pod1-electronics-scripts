#ifndef OPT2002_H
#define OPT2002_H

#include "Sensor.h"

class OPT2002 : public Sensor {
    public:
        OPT2002(uint8_t inpin, uint8_t errpin, arduino_t arduino);
    private:
        errorlevel_t read(t_datum* data, uint8_t numdata) override;
        errorlevel_t init() override;
        /**
         * Analog input pin.
         * */
        uint8_t inpin;
        uint8_t errpin;
};

#endif