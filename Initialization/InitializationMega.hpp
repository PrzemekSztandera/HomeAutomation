//
// Created by Przemyslaw Sztandera on 21/05/2020.
//

#pragma once

#include "../Mapping/MappingMega.hpp"

void initializeRelayPins() {
    for (uint8_t i = 0; i < maxSensors; i++) {
        auto sensor = Sensors[i];
        pinMode(sensor.relayPin, OUTPUT);
        msgs[i] = MyMessage(sensor.id, V_STATUS);
        uint8_t currentState = loadState(sensor.id);

        // Check whether EEPROM cell was used before
        if (!(currentState == 0 || currentState == 1)) {
            currentState = LOW;
            saveState(sensor.id, currentState);
        }

        // inverse state if sensors/relay is Active Low
        bool bState = (sensor.lowLevelTrigger) ? !currentState : currentState;
        if (sensor.hasSignalPin) {
            bState = (sensor.lowLevelTrigger) ? HIGH : LOW;
        }
        digitalWrite(sensor.relayPin, bState);
    }
}

void sendPresentation() {
    // Send actual states
    for (uint8_t i = 0; i < maxSensors; i++) {
        auto sensor = Sensors[i];
        present(sensor.id, S_BINARY, sensor.description);
        send(msgs[i].set(loadState(sensor.id)));

        Serial.println("Presentation() function called");
    }
}
