#ifndef DCT500_H
#define DCT500_H

#include "Sensor.h"

//the header file
class DCT500 : public Sensor {
    public:
        DCT500(uint8_t pin, arduino_t arduino);     // which pin & arduino it is attached to 
    private:
        errorlevel_t read(t_datum* data, uint8_t numdata) override;
        errorlevel_t init() override;
        /**
         * Analog input pin.
         * */
        uint8_t pin;
};

#endif