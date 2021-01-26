/**
 * Created by Przemyslaw Sztandera on 21/05/2020.
 */


// Functions prototypes

#pragma once

#include "../Sensor/Sensor.hpp"
#include "../DiscoveryMQTT/MQTT_discovery.hpp"

unsigned long currentButtonMillis = 0;
unsigned long currentSensorMillis = 0;

MyMessage sensorMessages[numberOfSensors];

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

    Serial.println(F("Environment sensors initialized!"));
}

void initializeSensorPinsOnExpander() {
    for (uint8_t i = 0; i < numberOfSensors; i++) {
        Sensor sensor = sensors[i];
        if (sensor.onExpander()) {
            expander[sensor.getExpanderAddress()].pinMode(sensor.getPin(), INPUT);
            expander[sensor.getExpanderAddress()].pullUp(sensor.getPin(), HIGH);
        }
    }
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
        uint8_t discoveryType = sensor.getDiscoveryType();

        if (discoveryType == HA_DISCOVERY) {
            sendHomeAssistantDiscovery(sensor.getId());
        } else if (discoveryType == MS_DISCOVERY) {
            present(sensor.getId(), sensor.getPresentationType(), sensor.getDescription());
        } else {
#ifdef SETUP_DEBUG
            Serial.println(F("Choose presentation type!!!"));
#endif
        }
        if (sensor.getVariableType() == V_STATUS && sensor.getPresentationType() == S_BINARY) {
            send(sensorMessages[i].set(loadState(sensor.getId())));
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
