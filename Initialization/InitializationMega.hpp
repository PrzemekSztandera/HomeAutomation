/**
 * Created by Przemyslaw Sztandera on 21/05/2020.
 */

#pragma once

#include "../Mapping/RelaysMappingMega.hpp"
#include "../Mapping/SensorsMappingMega.hpp"

void initializeRelays() {
    for (uint8_t i = 0; i < maxRelays; i++) {
        auto relayStruct = Relays[i];
        auto relay = getRelay(relayStruct.getId());
        pinMode(relay.getPin(), OUTPUT);
        msgs[i] = MyMessage(relayStruct.getId(), V_STATUS);
        uint8_t currentState = loadState(relayStruct.getId());

        // Check whether EEPROM cell was used before
        if (!(currentState == 0 || currentState == 1)) {
            currentState = LOW;
            saveState(relayStruct.getId(), currentState);
        }

        // Inverse state if sensors/relay is Active Low
        bool bState = (relay.isLowLevelTrigger()) ? !currentState : currentState;
        if (relayStruct.hasSignalPin()) {
            bState = (relay.isLowLevelTrigger()) ? HIGH : LOW;
        }
        digitalWrite(relay.getPin(), bState);
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
    while (!bme.begin()) {
        Serial.println("Could not find BME280 sensor!");
        wait(1000);
    }

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

unsigned long currentMillis = 0;
unsigned long currentMillis2 = 0;

void initializeTimers() {
    currentMillis = millis();
    currentMillis2 = millis();
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
