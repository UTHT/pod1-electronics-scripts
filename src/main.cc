#ifndef UNIT_TEST

#if PCB == 1
  #include <main_pcb1.h>
#endif
#if PCB == 2
  #include <main_pcb2.h>
#endif
#if PCB == 3
  #include <main_pcb3.h>
#endif

#include <Arduino.h>

#if ENABLE_WATCHDOG
  #include <avr/wdt.h>
#endif

#include <utils/instructions.h>
#include <utils/jsonmessenger.h>
#include <sensors/sensor.h>
#include <actuators/actuator.h>

#include <zcm_utht.h>
#include <transport.h>
#include <arduino_cobs_serial_transport.hpp>
#include <channel_array_msg.h>

bool post(void);
int publishMessageToChannel(zcm_t* zcm, double values[], String sensorName, String units, int numData);

zcm_t* zcm_arduino = NULL;

void setup(void) {
  // Status LED
  pinMode(PIN_STATUS, OUTPUT);
  digitalWrite(PIN_STATUS, LOW);

  // Await serial start
  Serial.begin(BAUDRATE);
  while (!Serial) {
    delay(1);
  }

  // Serial communications established, initialize sensors and actuators
  if (!post()) {
    // Failed POST, blink
    while (true) {
      delay(250);
      digitalWrite(PIN_STATUS, HIGH);
      delay(250);
      digitalWrite(PIN_STATUS, LOW);
    }
  } else {
    zcm_arduino = create_zcm(PCB);
  }

  // Enable the watchdog timer
  #if ENABLE_WATCHDOG
    wdt_enable(WDTO_1S);
  #endif
  digitalWrite(PIN_STATUS, HIGH);
}

void loop(void) {
  if (zcm_arduino != NULL) {
    for (int i = 0; i < NUM_SENSORS; ++i) {
      SensorState* state = sensors[i]->update();
      if (state->error == ERR_NONE) {
        if (state->debug == DS_SUCCESS) {
          String sensorName = sensorNames[sensors[i]->getID()];
          String labels = "";
          double values[state->numdata];

          // Append sensor values to array that would be sent via ZCM
          for (int x = 0; x < state->numdata; x++) {
              values[x] = state->data[x].value;
              labels += state->data[x].label;
              labels += ", ";
          }

          publishMessageToChannel(zcm_arduino, values, sensorName, labels, state->numdata);
        } // else do nothing
      } else if (state->error == ERR_WARNING) {
        String errorMessage = String("Failed to read from sensor " + String(sensors[i]->getID()) + " (non-fatal).");

        // TODO: 1) Add "statusMsg" field to channel_array_msg ZCM message
        //       2) Send this error message via ZCM  
      } else if (state->error == ERR_FATAL) {
        String errorMessage = String("Failed to read from sensor " + String(sensors[i]->getID()) + " (FATAL, SENSOR DISABLED!).");

        // TODO: 1) Add "statusMsg" field to channel_array_msg ZCM message
        //       2) Send this error message via ZCM   
      }
    }
    for (int i = 0; i < NUM_ACTUATORS; ++i) {
      ActuatorState* state = actuators[i]->update();
      if (state->error == ERR_WARNING) {
        String errorMessage = String("Failed to update actuator " + String(actuators[i]->getID()) + " (non-fatal).");

        // TODO: 1) Add "statusMsg" field to channel_array_msg ZCM message
        //       2) Send this error message via ZCM 
      } else if (state->error == ERR_FATAL) {
        String errorMessage = String("Failed to update actuator " + String(actuators[i]->getID()) + " (FATAL, ACTUATOR DISABLED!).");

        // TODO: 1) Add "statusMsg" field to channel_array_msg ZCM message
        //       2) Send this error message via ZCM 
      }
    }

    #if ENABLE_WATCHDOG
      wdt_enable(WDTO_1S);
    #endif
  } else {
      JSONMessenger::sendMessage(JSONMessenger::MESSAGE_DEBUG, String("Failed to initialize the ZCM serial transport layer."));
  }
}

bool post(void) {
  bool success = true;
  for (int i = 0; i < NUM_SENSORS; ++i) {
    SensorState* state = sensors[i]->begin();
    bool latest = (state->debug >= DS_INITIALIZED && state->error == ERR_NONE);
    if (latest) {
      String statusMessage = String("Sensor " + String(sensors[i]->getID()) + " initialized successfully.");

      // TODO: 1) Add "statusMsg" field to channel_array_msg ZCM message
      //       2) Send this status message via ZCM 
    } else {
      String errorMessage = String("Failed to initialize sensor " + String(sensors[i]->getID()) + ". Check wiring.");

      // TODO: 1) Add "statusMsg" field to channel_array_msg ZCM message
      //       2) Send this error message via ZCM     
    }
    success &= latest;
  }
  for (int i = 0; i < NUM_ACTUATORS; ++i) {
    ActuatorState* state = actuators[i]->begin();
    bool latest = (state->debug >= DS_INITIALIZED && state->error == ERR_NONE);
    if (latest) {
      String statusMessage = String("Actuator " + String(actuators[i]->getID()) + " initialized successfully.");

      // TODO: 1) Add "statusMsg" field to channel_array_msg ZCM message
      //       2) Send this status message via ZCM 
    } else {
      String errorMessage = String("Failed to initialize actuator " + String(actuators[i]->getID()) + ". Check wiring.");

      // TODO: 1) Add "statusMsg" field to channel_array_msg ZCM message
      //       2) Send this error message via ZCM     
    }
    success &= latest;
  }
  return success;
}

int publishMessageToChannel(zcm_t* zcm, double values[], String sensorName, String units, int numData) {    
  channel_array message;
  message.arduino_id = PCB;
  message.sensor = (char *) sensorName.c_str();
  message.data = values;
  message.units = (char *) units.c_str();
  message.sz = numData;

  return channel_array_publish(zcm, sensorName.c_str(), &message);
}

#endif