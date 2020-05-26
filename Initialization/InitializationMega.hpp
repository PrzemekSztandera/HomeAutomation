//
// Created by Przemyslaw Sztandera on 21/05/2020.
//

#pragma once

#include "../Mapping/RelaysMappingMega.hpp"
#include "../Mapping/SensorsMappingMega.hpp"

void initializeRelayPins() {
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

        // inverse state if sensors/relay is Active Low
        bool bState = (relay.isLowLevelTrigger()) ? !currentState : currentState;
        if (relayStruct.hasSignalPin()) {
            bState = (relay.isLowLevelTrigger()) ? HIGH : LOW;
        }
        digitalWrite(relay.getPin(), bState);
    }
}

void initializeSensors() {
    for (uint8_t i = 0; i < maxSensors; i++) {
        auto sensorStruct = Sensors[i];
        sensorMsgs[i] = MyMessage(Sensors[i].getId(), Sensors[i].getVariableType());

    }
}

void sendPresentation() {
    // Send actual states
    for (uint8_t i = 0; i < maxRelays; i++) {
        auto relayStruct = Relays[i];
        present(relayStruct.getId(), S_BINARY, relayStruct.getDescription());
        send(msgs[i].set(loadState(relayStruct.getId())));
    }

    for (uint8_t i = 0; i < maxSensors; i++) {
        auto sensorStruct = Sensors[i];
        present(sensorStruct.getId(), sensorStruct.getPresentationType(), sensorStruct.getDescription());
    }

    Serial.println("Presentation() function called");
}
