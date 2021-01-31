/**
 * Created by Przemyslaw Sztandera on 21/05/2020.
 */


// Functions prototypes

#pragma once

#include <EEPROM.h>
#include <BH1750.h>
#include "../Sensor/Sensor.hpp"
#include "../DiscoveryMQTT/MQTT_discovery.hpp"
#include "../Timer/Timer.hpp"

BH1750 lightMeter(0x23);

MyMessage sensorMessages[numberOfSensors];



void initializeTimers() {

#ifdef TIMER
    initializeTime();
#endif

    sensorsTimerHelper = getEpochInSeconds();
}

void initializeMCP23017() {
    for (uint8_t i = 0; i < numberOfExpanders; i++) {
        expander[i].begin(expanderAddresses[i]);
    }
}

// void initializeSensorPinsOnExpander() {
//     for (uint8_t i = 0; i < numberOfSensors; i++) {
//         Sensor sensor = sensors[i];
//         if (sensor.onExpander()) {
//             expander[sensor.getExpanderAddress()].pinMode(sensor.getPin(), INPUT);
//             expander[sensor.getExpanderAddress()].pullUp(sensor.getPin(), HIGH);
//         }
//     }
// }

void initializeAndSetRelays() {

    for (uint8_t i = 0; i < numberOfSensors; i++) {
        Sensor sensor = sensors[i];
        uint8_t sensorId = sensor.getId();

        sensorMessages[i] = MyMessage(sensorId, sensor.getVariableType());

        if (sensor.getVariableType() == V_STATUS && sensor.getPresentationType() == S_BINARY) {

            Relay relay = getRelay(sensorId);
            bool latching = relay.isLatching();
            bool lowLevel = relay.isLowLevelTrigger();
            bool onExpander = relay.onExpander();
            uint8_t relayPin = relay.getPin();
            relay.initialize();

            uint8_t currentSensorState = loadState(sensorId);

#ifdef SETUP_DEBUG
            Serial.print(F("Current sensor: "));
            Serial.println(sensorId);
            Serial.println(sensor.getDescription());
            Serial.print(F("Current sensor state in initialization: "));
            Serial.println(currentSensorState);
#endif

            // Check whether EEPROM cell was used before
            if (!(currentSensorState == 0 || currentSensorState == 1)) {
                currentSensorState = LOW;
                saveState(sensorId, currentSensorState);
            }

            uint8_t currentRelayState = loadState(sensorId);

            if (latching) {
                if (lowLevel) {
                    currentRelayState = !currentRelayState;
                }
            } else {
                currentRelayState = (lowLevel) ? HIGH : LOW;
            }

#ifdef SETUP_DEBUG
            Serial.print(F("Current relay state in initialization: "));
            Serial.println(currentRelayState);
#endif

            if (onExpander) {
                expander[relay.getExpanderAddress()].digitalWrite(relayPin, currentRelayState);
            } else {
                digitalWrite(relayPin, currentRelayState);
            }

#ifdef SETUP_DEBUG
            Serial.print(F("Current pin state in initialization: "));
            Serial.println(digitalRead(relayPin));
#endif

            myDelay(250);
        }
    }
    Serial.println(F("Relays initialized and set up!"));
}

void initializeEnvironmentSensors() {

    if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) {
        Serial.println(F("BH1750 Advanced begin"));
    } else {
        Serial.println(F("Error initialising BH1750"));
    }

    Serial.println(F("Environment sensors initialized!"));
}


void sendHomeAssistantDiscovery(uint8_t  sensorId) {

    char *topic = createTopic(sensorId, DISCOVERY_TOPIC);
    char topicArr[strlen(topic) + 1];
    strcpy(topicArr, topic);

    char *payload = createPayload(sensorId);
    char payloadArr[strlen(payload) + 1];
    strncpy(payloadArr, payload, strlen(payload) + 1);

    _MQTT_client.publish(topicArr, payloadArr, true);

#ifdef SETUP_DEBUG
    Serial.println(topicArr);
    Serial.println(payloadArr);
#endif
    myDelay(100);
}

void sendPresentation() {
    for (uint8_t i = 0; i < numberOfSensors; i++) {
        Sensor sensor = sensors[i];
        uint8_t sensorId = sensor.getId();
        uint8_t discoveryType = sensor.getDiscoveryType();

        if (discoveryType == HA_DISCOVERY) {
            sendHomeAssistantDiscovery(sensorId);
        } else if (discoveryType == MS_DISCOVERY) {
            present(sensorId, sensor.getPresentationType(), sensor.getDescription());
        } else {
#ifdef SETUP_DEBUG
            Serial.println(F("Choose presentation type!!!"));
#endif
        }
        if (sensor.getVariableType() == V_STATUS && sensor.getPresentationType() == S_BINARY) {
            updateRelayStateAndSendMessage(sensorId);
        }
    }

    Serial.println(F("sendPresentation() called...!"));
}

void printRelaySensorDetails() {
    for (uint8_t i = 0; i < numberOfSensors; i++) {
        Sensor sensor = sensors[i];
        uint8_t sensorId = sensor.getId();
        Relay relay = getRelay(sensorId);

        Serial.print(F("Current sensor: "));
        Serial.println(sensorId);
        Serial.print(F("Current sensor state in setup: "));
        Serial.println(loadState(sensorId));
        Serial.print(F("Current pin state in setup: "));
        Serial.println(digitalRead(relay.getPin()));
    }
}



void clearEeprom() {
  // initialize the LED pin as an output.
  pinMode(13, OUTPUT);
  
  /***
    Iterate through each byte of the EEPROM storage.

    Larger AVR processors have larger EEPROM sizes, E.g:
    - Arduno Duemilanove: 512b EEPROM storage.
    - Arduino Uno:        1kb EEPROM storage.
    - Arduino Mega:       4kb EEPROM storage.

    Rather than hard-coding the length, you should use the pre-provided length function.
    This will make your code portable to all AVR processors.
  ***/

  for (unsigned int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }

  // turn the LED on when we're done
  digitalWrite(13, HIGH);
}

