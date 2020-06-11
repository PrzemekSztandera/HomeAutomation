/**
 * Created by Przemyslaw Sztandera on 21/05/2020.
 */

#pragma once
#define USE_EXPANDER

#include "../Mapping/RelaysMappingMega.hpp"
#include "../Mapping/SensorsMappingMega.hpp"

void initializeRelays() {
    for (uint8_t i = 0; i < maxRelays; i++) {
        auto relayStruct = Relays[i];
        auto relay = getRelay(relayStruct.getId());
        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].pinMode(relay.getPin(), OUTPUT);
        } else {
            pinMode(relay.getPin(), OUTPUT);
        }

        msgs[i] = MyMessage(relayStruct.getId(), V_STATUS);
        uint8_t currentState = loadState(relayStruct.getId());

        // Check whether EEPROM cell was used before
        if (!(currentState == 0 || currentState == 1)) {
            currentState = LOW;
            saveState(relayStruct.getId(), currentState);
        }

        // Inverse state if relay is Active Low and relayStruct uses relay as "press button" - bi stable relay
        bool bState = (relay.isLowLevelTrigger()) ? !currentState : currentState;

        // Assign state if relayStruct uses relay as "click button" - mono stable relay
        if (relayStruct.hasSignalPin()) {
            bState = (relay.isLowLevelTrigger()) ? HIGH : LOW;
        }
        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), bState);
        } else {
            digitalWrite(relay.getPin(), bState);
        }

    }
    Serial.println("Relays initialized");
}

void initializeSensors() {
    for (uint8_t i = 0; i < maxSensors; i++) {
        auto sensorStruct = Sensors[i];
        sensorMsgs[i] = MyMessage(Sensors[i].getId(), Sensors[i].getVariableType());
    }

    // Bosh sensor BME280
    Wire.begin();
    if (!bme.begin()) {
        Serial.println("Could not find BME280 sensor!");
    }
//    while (!bme.begin()) {
//        Serial.println("Could not find BME280 sensor!");
//        wait(1000);
//    }

    // bme.chipID(); // Deprecated. See chipModel().
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
    for (uint8_t i = 0; i < maxRelays; i++) {
        if (Relays[i].onExpander()) {
            expander[Relays[i].getExpanderAddress()].pinMode(Relays[i].getPin(), INPUT);
            expander[Relays[i].getExpanderAddress()].pullUp(Relays[i].getPin(), HIGH);
        }
    }
}

unsigned long currentMillis = 0;
unsigned long currentMillis2 = 0;

void initializeTimers() {
    currentMillis = millis();
    currentMillis2 = millis();
}

void initializeMCP23017() {
#ifdef USE_EXPANDER
    for (int i = 0; i < numberOfExpanders; i++) {
        expander[i].begin(expanderAddresses[i]);
    }
#endif
}

void sendPresentation() {

    for (uint8_t i = 0; i < maxRelays; i++) {
        auto relayStruct = Relays[i];
        present(relayStruct.getId(), S_BINARY, relayStruct.getDescription());
        send(msgs[i].set(loadState(relayStruct.getId())));
    }

    for (uint8_t i = 0; i < maxSensors; i++) {
        auto sensorStruct = Sensors[i];
        present(sensorStruct.getId(), sensorStruct.getPresentationType(), sensorStruct.getDescription());
    }

    Serial.println("Presentation() called");
}
