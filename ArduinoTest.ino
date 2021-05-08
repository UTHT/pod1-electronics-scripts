#include "Sensor.h"
#include "SPTD25-20-1000H/SPTD25-20-1000H.h"

#define NUM_SENSORS 1

SPTD25_20_1000H sptd25_20_1000h(A0, ARDUINO_ONE);

Sensor* sensors[NUM_SENSORS] = {
    &sptd25_20_1000h
};

void setup(){
    Serial.begin(115200);
    for(int i = 0; i < NUM_SENSORS; i++){
        SensorState* state = sensors[i]->begin();
        if(state->debug != DS_INIT){
            Serial.print("Failed to initialize sensor - ID: ");
            Serial.println(sensors[i]->sensor);
        } else {
            Serial.print("Sensor ID: ");
            Serial.print(sensors[i]->sensor);
            Serial.println(" initialized successfully!");
        }
    }
}

void loop(){
    for(int i = 0; i < NUM_SENSORS; i++){
        SensorState* state = sensors[i]->update();
        if(state->debug == DS_NEWREAD){
            Serial.print("Data from sensor ID: ");
            Serial.print(sensors[i]->sensor);
            Serial.print(": ");
            printData(state->data, state->numdata);
        } else if(state->debug != DS_WAITING) {
            Serial.print("Sensor ID: ");
            Serial.print(sensors[i]->sensor);
            Serial.println(" failed! Disabling.");
        }
    }
}

void printData(t_datum* data, uint8_t numdata){
    for(int i = 0; i < numdata-1; i++){
        Serial.print(data[i].data, 3);
        Serial.print(data[i].units);
        Serial.print(", ");
    }
    Serial.print(data[numdata-1].data, 3);
    Serial.println(data[numdata-1].units);
}