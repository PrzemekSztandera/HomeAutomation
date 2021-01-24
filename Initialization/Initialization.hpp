/**
 * Created by Przemyslaw Sztandera on 21/05/2020.
 */


// Functions prototypes

#pragma once

#include "../Mapping/RelaySensorsMapping.hpp"
#include "../Mapping/EnvironmentSensorsMapping.hpp"
#include "../DiscoveryMQTT/MQTT_discovery.hpp"

unsigned long currentButtonMillis = 0;
unsigned long currentSensorMillis = 0;

MyMessage relaySensorMessages[numberOfRelaySensors];
MyMessage environmentSensorMessages[numberOfEnvironmentSensors];

void initializeTimers() {
    currentButtonMillis = millis();
    currentSensorMillis = millis();
}

void resetTimers() {
    if (millis() == 0) {
        currentButtonMillis = millis();
        currentSensorMillis = millis();
    }
}

void initializeMCP23017() {
    for (uint8_t i = 0; i < numberOfExpanders; i++) {
        expander[i].begin(expanderAddresses[i]);
    }
}

void initializeRelays() {

    for (byte i = 0; i < numberOfRelaySensors; i++) {
        auto relaySensor = relaySensors[i];
        Relay relay = relaySensor.getRelay();

        relay.initialize();

        relaySensorMessages[i] = MyMessage(relaySensor.getId(), relaySensor.getVariableType());

        uint8_t currentSensorState = loadState(relaySensor.getId());

#ifdef SETUP_DEBUG
        Serial.print(F("Current sensor: "));
        Serial.println(relaySensor.getId());
        Serial.println(relaySensor.getDescription());
        Serial.print(F("Current sensor state in initialization: "));
        Serial.println(currentSensorState);
#endif

        // Check whether EEPROM cell was used before
        if (!(currentSensorState == 0 || currentSensorState == 1)) {
            currentSensorState = LOW;
            saveState(relaySensor.getId(), currentSensorState);
        }

        uint8_t currentRelayState = loadState(relaySensor.getId());

        if (relay.isLatching()) {
            if (relay.isLowLevelTrigger()) {
                currentRelayState = !currentRelayState;
            }
        } else {
            currentRelayState = (relay.isLowLevelTrigger()) ? HIGH : LOW;
        }

#ifdef SETUP_DEBUG
        Serial.print(F("Current relay state in initialization: "));
        Serial.println(currentRelayState);
#endif

        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), currentRelayState);
        } else {
            digitalWrite(relay.getPin(), currentRelayState);
        }

#ifdef SETUP_DEBUG
        Serial.print(F("Current pin state in initialization: "));
        Serial.println(digitalRead(relay.getPin()));
#endif

        myDelay(250);

    }
    Serial.println(F("initializeRelays() called...!"));
}

void initializeEnvironmentSensors() {

    for (uint8_t i = 0; i < numberOfEnvironmentSensors; i++) {
        auto environmentSensor = environmentSensors[i];
        environmentSensorMessages[i] = MyMessage(environmentSensor.getId(), environmentSensor.getVariableType());
    }

    // Bosh sensor BME280
    if (!bme.begin()) {
        Serial.println(F("Could not find BME280 sensor!"));
    }

    switch (bme.chipModel()) {
        case BME280::ChipModel_BME280:
            Serial.println(F("Found BME280 sensor! Success."));
            break;
        case BME280::ChipModel_BMP280:
            Serial.println(F("Found BMP280 sensor! No Humidity available."));
            break;
        default:
            Serial.println(F("Found UNKNOWN sensor! Error!"));
    }
// Dallas temp sensor DS18B20

//    Wire.begin();
//    dallasSensors.begin();

    Serial.println(F("Sensors initialized"));
}

void initializeMcpPinsAsSignalPinsForRelays() {
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        auto relaySensor = relaySensors[i];
        if (relaySensor.onExpander()) {
            expander[relaySensor.getExpanderAddress()].pinMode(relaySensor.getPin(), INPUT);
            expander[relaySensor.getExpanderAddress()].pullUp(relaySensor.getPin(), HIGH);
        }
    }
}

void presentToHA(uint8_t  sensorId) {

    char *topic = createTopic(sensorId, DISCOVERY_TOPIC);
    char topicArr[strlen(topic) + 1];
    strcpy(topicArr, topic);

    char *payload = createPayload(sensorId);
    char payloadArr[strlen(payload) + 1];
    strncpy(payloadArr, payload, strlen(payload) + 1);

    _MQTT_client.publish(topicArr, payloadArr, true);

#ifdef SETUP_DEBUG
    Serial.println(topic);
    Serial.println(payload);
#endif
    myDelay(100);
}

void sendPresentation() {
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        auto relaySensor = relaySensors[i];
        uint8_t discoveryType = relaySensor.getDiscoveryType();

        if (discoveryType == HA_DISCOVERY) {
            presentToHA(relaySensor.getId());
        } else if (discoveryType == MS_DISCOVERY) {
            present(relaySensor.getId(), relaySensor.getPresentationType(), relaySensor.getDescription());
        } else {
#ifdef SETUP_DEBUG
            Serial.println(F("Choose presentation type!!!"));
#endif
        }
        send(relaySensorMessages[i].set(loadState(relaySensor.getId())));
    }

    for (uint8_t i = 0; i < numberOfEnvironmentSensors; i++) {
        auto environmentSensor = environmentSensors[i];
        uint8_t discoveryType = environmentSensor.getDiscoveryType();

        if (discoveryType == HA_DISCOVERY) {
            presentToHA(environmentSensor.getId());
        } else if (discoveryType == MS_DISCOVERY) {
            present(environmentSensor.getId(), environmentSensor.getPresentationType(), environmentSensor.getDescription());
        } else {
#ifdef SETUP_DEBUG
            Serial.println(F("Choose presentation type!!!"));
#endif
        }
    }

    send(environmentSensorMessages[0].set(millis() / 1000));

    Serial.println(F("sendPresentation() called...!"));
}


void printRelaySensorDetails() {
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        auto relaySensor = relaySensors[i];
        Relay relay = relaySensor.getRelay();
        Serial.print(F("Current sensor: "));
        Serial.println(relaySensor.id);
        Serial.print(F("Current sensor state in setup: "));
        Serial.println(loadState(relaySensor.getId()));
        Serial.print(F("Current pin state in setup: "));
        Serial.println(digitalRead(relay.getPin()));
    }
}
