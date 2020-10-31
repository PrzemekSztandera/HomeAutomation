/**
 * Created by Przemyslaw Sztandera on 21/05/2020.
 */

#pragma once
#define USE_EXPANDER
#define MY_DEBUG

#include "../Mapping/RelaysMappingMega.hpp"
#include "../Mapping/SensorsMappingMega.hpp"
#include "../Automation/AutomationMega.hpp"
#include "../Button/ButtonsInitialization.hpp"

unsigned long currentButtonMillis = 0;
unsigned long currentSensorMillis = 0;

void initializeMCP23017() {
#ifdef USE_EXPANDER
    for (int i = 0; i < numberOfExpanders; i++) {
        expander[i].begin(expanderAddresses[i]);
    }
#endif
}

void initializeTimers() {
    currentButtonMillis = millis();
    currentSensorMillis = millis();
}

void initializeRelays() {
    for (uint8_t i = 0; i < numberOfRelayStruct; i++) {
        auto relayStruct = relaySensors[i];
        auto relay = getRelay(relayStruct.getId());

        relay.initialize();

        msgs[i] = MyMessage(relayStruct.getId(), V_STATUS);
        uint8_t currentSensorState = loadState(relayStruct.getId());

#ifdef MY_DEBUG
        Serial.print("Current sensor: ");
        Serial.println(relayStruct.id);
        Serial.print("Current sensor state: ");
        Serial.println(currentSensorState);
#endif

        // Check whether EEPROM cell was used before
        if (!(currentSensorState == 0 || currentSensorState == 1)) {
            currentSensorState = LOW;
            saveState(relayStruct.getId(), currentSensorState);
        }

#ifdef MY_DEBUG
        Serial.print("Current sensor state2: ");
        Serial.println(loadState(relayStruct.getId()));
#endif

        uint8_t currentRelayState = !loadState(relayStruct.getId());

        if (relay.isLatching()) {
            if (relay.isLowLevelTrigger()) {
                currentRelayState = !currentRelayState;
            }
        } else {
            currentRelayState = (relay.isLowLevelTrigger()) ? HIGH : LOW;
        }

#ifdef MY_DEBUG
        Serial.print("Current relay state: ");
        Serial.println(currentRelayState);
#endif

        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), currentRelayState);
        } else {
            digitalWrite(relay.getPin(), currentRelayState);
        }

#ifdef MY_DEBUG
        Serial.print("Current pin state: ");
        Serial.println(digitalRead(relay.getPin()));
#endif

    }
    Serial.println("Relays initialized");
}

void initializeSensors() {
    for (uint8_t i = 0; i < maxSensors; i++) {
        auto sensorStruct = environmentSensors[i];
        sensorMsgs[i] = MyMessage(environmentSensors[i].getId(), environmentSensors[i].getVariableType());
    }

    // Bosh sensor BME280
    Wire.begin();
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
    sensors.begin();

    Serial.println("Sensors initialized");
}

void initializeMcpPinsAsSignalPinsForRelays() {
    for (uint8_t i = 0; i < numberOfRelayStruct; i++) {
        if (relaySensors[i].onExpander()) {
            expander[relaySensors[i].getExpanderAddress()].pinMode(relaySensors[i].getPin(), INPUT);
            expander[relaySensors[i].getExpanderAddress()].pullUp(relaySensors[i].getPin(), HIGH);
        }
    }
}

void sendPresentation() {
    for (uint8_t i = 0; i < numberOfRelayStruct; i++) {
        auto relayStruct = relaySensors[i];
        present(relayStruct.getId(), S_BINARY, relayStruct.getDescription());
        send(msgs[i].set(loadState(relayStruct.getId())));
    }
    for (uint8_t i = 0; i < maxSensors; i++) {
        auto sensorStruct = environmentSensors[i];
        present(sensorStruct.getId(), sensorStruct.getPresentationType(), sensorStruct.getDescription());
    }

    Serial.println("Presentation() called");
}

void printSensorDetails() {
    for (uint8_t i = 0; i < numberOfRelayStruct; i++) {
        auto relayStruct = relaySensors[i];
        auto relay = getRelay(relayStruct.getId());
        Serial.print("Current sensor: ");
        Serial.println(relayStruct.id);
        Serial.print("Current sensor state: ");
        Serial.println(loadState(relayStruct.getId()));
        Serial.print("Current pin state: ");
        Serial.println(digitalRead(relay.getPin()));
    }
}
