#include <Arduino.h>
#include "ExampleSensor.h"

void setup(){
    init();
}

void loop(){
    int x = getValue();
    delay(1000);
}