#include <ExampleSensor.h>

void init(){
    example = Example();
}

int getValue(){
    example.read();
}