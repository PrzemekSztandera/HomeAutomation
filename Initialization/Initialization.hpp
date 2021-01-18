/**
 * Created by Przemyslaw Sztandera on 21/05/2020.
 */


// Functions prototypes

#pragma once

#include "../Mapping/RelaySensorsMapping.hpp"
#include "../Mapping/EnvironmentSensorsMapping.hpp"

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
    for (int i = 0; i < numberOfExpanders; i++) {
        expander[i].begin(expanderAddresses[i]);
    }
}

void initializeRelays() {

    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        auto relaySensor = relaySensors[i];
        Relay relay = relaySensor.getRelay();

        relay.initialize();

        relaySensorMessages[i] = MyMessage(relaySensor.getId(), relaySensor.getVariableType());

        uint8_t currentSensorState = loadState(relaySensor.getId());

        Serial.print("Current sensor: ");
        Serial.println(relaySensor.getId());
        Serial.println(relaySensor.getDescription());
        Serial.print("Current sensor state in initialization: ");
        Serial.println(currentSensorState);

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

        Serial.print("Current relay state in initialization: ");
        Serial.println(currentRelayState);

        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), currentRelayState);
        } else {
            digitalWrite(relay.getPin(), currentRelayState);
        }

        Serial.print("Current pin state in initialization: ");
        Serial.println(digitalRead(relay.getPin()));

        myDelay(250);

    }
    Serial.println("initializeRelays() called...!");
}

void initializeEnvironmentSensors() {

    for (uint8_t i = 0; i < numberOfEnvironmentSensors; i++) {
        auto environmentSensor = environmentSensors[i];
        environmentSensorMessages[i] = MyMessage(environmentSensor.getId(), environmentSensor.getVariableType());
    }

    // Bosh sensor BME280
    if (!bme.begin()) {
        Serial.println("Could not find BME280 sensor!");
    }

    switch (bme.chipModel()) {
        case BME280::ChipModel_BME280:
            Serial.println("Found BME280 sensor! Success.");
            break;
        case BME280::ChipModel_BMP280:
            Serial.println("Found BMP280 sensor! No Humidity available.");
            break;
        default:
            Serial.println("Found UNKNOWN sensor! Error!");
    }
// Dallas temp sensor DS18B20

//    Wire.begin();
//    dallasSensors.begin();

    Serial.println("Sensors initialized");
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

void sendPresentation() {
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        auto relaySensor = relaySensors[i];
        present(relaySensor.getId(), relaySensor.getPresentationType(), relaySensor.getDescription());
        send(relaySensorMessages[i].set(loadState(relaySensor.getId())));
    }
    for (uint8_t i = 0; i < numberOfEnvironmentSensors; i++) {
        auto environmentSensor = environmentSensors[i];
        present(environmentSensor.getId(), environmentSensor.getPresentationType(), environmentSensor.getDescription());
        send(environmentSensorMessages[0].set(millis() / 1000));
    }

    Serial.println("sendPresentation() called...!");
}

void printRelaySensorDetails() {
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        auto relaySensor = relaySensors[i];
        Relay relay = relaySensor.getRelay();
        Serial.print("Current sensor: ");
        Serial.println(relaySensor.id);
        Serial.print("Current sensor state in setup: ");
        Serial.println(loadState(relaySensor.getId()));
        Serial.print("Current pin state in setup: ");
        Serial.println(digitalRead(relay.getPin()));
    }
}
