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
int publishStatusToChannel(zcm_t* zcm, String sensorName, String statusMsg);

zcm_t* zcm_arduino = NULL;

void setup(void) {
  zcm_arduino = create_zcm(0);

  // Status LED
  pinMode(PIN_STATUS, OUTPUT);
  digitalWrite(PIN_STATUS, LOW);

  // Serial communications established, initialize sensors and actuators
  if (!post()) {
    // Failed POST, blink
    while (true) {
      delay(250);
      digitalWrite(PIN_STATUS, HIGH);
      delay(250);
      digitalWrite(PIN_STATUS, LOW);
    }
  }

  // Enable the watchdog timer
  #if ENABLE_WATCHDOG
    wdt_enable(WDTO_1S);
  #endif
  digitalWrite(PIN_STATUS, HIGH);
}

void loop(void) {
  if (zcm_arduino != NULL) {
    zcm_handle_nonblock(zcm_arduino);

    for (int i = 0; i < NUM_SENSORS; ++i) {
      SensorState* state = sensors[i]->update();
      String sensorName = sensorNames[sensors[i]->getID()];

      if (state->error == ERR_NONE) {
        if (state->debug == DS_SUCCESS) {
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

        publishStatusToChannel(zcm_arduino, sensorName, errorMessage);
      } else if (state->error == ERR_FATAL) {
        String errorMessage = String("Failed to read from sensor " + String(sensors[i]->getID()) + " (FATAL, SENSOR DISABLED!).");

        publishStatusToChannel(zcm_arduino, sensorName, errorMessage);
      }
    }
    for (int i = 0; i < NUM_ACTUATORS; ++i) {
      ActuatorState* state = actuators[i]->update();
      String actuatorName = actuatorNames[actuators[i]->getID()];

      if (state->error == ERR_WARNING) {
        String errorMessage = String("Failed to update actuator " + String(actuators[i]->getID()) + " (non-fatal).");

        publishStatusToChannel(zcm_arduino, actuatorName, errorMessage);
      } else if (state->error == ERR_FATAL) {
        String errorMessage = String("Failed to update actuator " + String(actuators[i]->getID()) + " (FATAL, ACTUATOR DISABLED!).");

        publishStatusToChannel(zcm_arduino, actuatorName, errorMessage);
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
    String sensorName = sensorNames[sensors[i]->getID()];

    bool latest = (state->debug >= DS_INITIALIZED && state->error == ERR_NONE);
    if (latest) {
      String statusMessage = String("Sensor " + String(sensors[i]->getID()) + " initialized successfully.");

      publishStatusToChannel(zcm_arduino, sensorName, statusMessage);
    } else {
      String errorMessage = String("Failed to initialize sensor " + String(sensors[i]->getID()) + ". Check wiring.");

      publishStatusToChannel(zcm_arduino, sensorName, errorMessage);  
    }
    success &= latest;
  }
  for (int i = 0; i < NUM_ACTUATORS; ++i) {
    ActuatorState* state = actuators[i]->begin();
    String actuatorName = actuatorNames[actuators[i]->getID()];

    bool latest = (state->debug >= DS_INITIALIZED && state->error == ERR_NONE);
    if (latest) {
      String statusMessage = String("Actuator " + String(actuators[i]->getID()) + " initialized successfully.");

      publishStatusToChannel(zcm_arduino, actuatorName, statusMessage);
    } else {
      String errorMessage = String("Failed to initialize actuator " + String(actuators[i]->getID()) + ". Check wiring.");

      publishStatusToChannel(zcm_arduino, actuatorName, errorMessage);    
    }
    success &= latest;
  }
  return success;
}


int publishStatusToChannel(zcm_t* zcm, String sensorName, String statusMsg) {
    channel_array message;
    message.arduino_id = PCB;
    message.sensor = (char *) sensorName.c_str();

    // TODO: Test whether ZCM can take a NULL value or it needs to be a defined value
    message.data = NULL;

    message.units = "";
    message.sz = 0;
    message.statusMsg = (char *) statusMsg.c_str();

    return channel_array_publish(zcm, sensorName.c_str(), &message);
}

int publishMessageToChannel(zcm_t* zcm, double values[], String sensorName, String units, int numData) {    
  channel_array message;
  message.arduino_id = PCB;
  message.sensor = (char *) sensorName.c_str();
  message.data = values;
  message.units = (char *) units.c_str();
  message.sz = numData;

  String noStatus = "";
  message.statusMsg = (char *) noStatus.c_str();

  return channel_array_publish(zcm, sensorName.c_str(), &message);
}

#endif