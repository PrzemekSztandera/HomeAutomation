/**
 *
 * @file AutomationMega.hpp
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Automation for buttons & sensors.
 * @license GPL V2
 *
 */

#pragma once

#include "../Mapping/MappingMega.hpp"

void myDelay(long interval) {
    unsigned long currentMillis = millis();
    while (millis() - currentMillis < interval) {}
}

// relay acts as a click button
void relayAsPushButton(int pin) {
    uint8_t state = digitalRead(pin);
    unsigned long currentMillis = millis();
    digitalWrite(pin, !state);
    myDelay(125);
    digitalWrite(pin, state);
}

// relay acts as a latch button
void relayAsLatchButton(int pin) {
    uint8_t pinState = digitalRead(pin);
    digitalWrite(pin, !pinState);
}

//----------------------------------------------------------------------------------------------------

void updateRelayStateAndSendMessage(const uint8_t &sensorId, bool pullUpActive = true) {
    // Debug
    Serial.print("Calling updateRelayStateAndSendMessage() for sensor: ");
    Serial.print(sensorId);
    // End of Debug

    auto sensor = getSensor(sensorId);
    uint8_t index = getIndex(sensor.id);
    uint8_t state = 2;

    if (sensor.isButton) {
        state = digitalRead(sensor.latchRelayPin);
        if(pullUpActive) {
            saveState(sensorId, !state);
        } else {
            saveState(sensorId, state);
        }
        Serial.print(" and pin: ");
        Serial.println(sensor.latchRelayPin);
    } else {
        state = !digitalRead(sensor.switchPin);
//        if (sensor.activeLow && digitalRead(sensor.switchPin) == 1) {
//            state = Toggle::OFF;
//        } else if (!sensor.activeLow && digitalRead(sensor.switchPin) == 0) {
//            state = Toggle::OFF;
//        }

        saveState(sensorId, state);

        Serial.print(" and pin: ");
        Serial.println(sensor.switchPin);
    }
    send(msgs[index].set(loadState(sensorId)));

    // Debug
    Serial.print("Sensor State after updateRelayStateAndSendMessage(): ");
    if(loadState(sensorId) == 2) {
        Serial.println("FLIP");
    } else if (loadState(sensorId) == 1) {
        Serial.println("ON");
    } else if (loadState(sensorId) == 0) {
        Serial.println("OFF");
    }
    // End of Debug
}

//----------------------------------------------------------------------------------------------------

void switchRelay(const uint8_t &sensorId, const uint8_t &cmd = Toggle::FLIP) {

    // Debug
    Serial.print("Calling setOutput() for sensor: ");
    Serial.print(sensorId);
    Serial.print(" with toggle: ");
    if(cmd == 2) {
        Serial.print("FLIP");
    } else if (cmd == 1) {
        Serial.print("ON");
    } else if (cmd == 0) {
        Serial.print("OFF");
    }
    Serial.print("; Sensor State Before: ");
    if(loadState(sensorId) == 2) {
        Serial.println("FLIP");
    } else if (loadState(sensorId) == 1) {
        Serial.println("ON");
    } else if (loadState(sensorId) == 0) {
        Serial.println("OFF");
    }
    // End of Debug


    auto sensor = getSensor(sensorId);
    uint8_t index = getIndex(sensorId);

    // check whether flip state of sensor
    uint8_t state = (cmd == Toggle::FLIP) ? !loadState(sensorId) : cmd; // OFF -> ON
    saveState(sensorId, state);

    if (cmd == Toggle::FLIP && sensor.isButton) {
        relayAsPushButton(sensor.switchPin);

        // Debug
        Serial.print("Calling relayAsPushButton(); ");
        Serial.print("Sensor State: ");
        if(loadState(sensorId) == 2) {
            Serial.println("FLIP");
        } else if (loadState(sensorId) == 1) {
            Serial.println("ON");
        } else if (loadState(sensorId) == 0) {
            Serial.println("OFF");
        }
        // End of Debug

    } else if (cmd == Toggle::FLIP && !sensor.isButton) {
        relayAsLatchButton(sensor.switchPin);
//        saveState(sensorId, state); // ON

        // Debug
        Serial.print("Calling relayAsLatchButton(); ");
        Serial.print("Sensor State: ");
        if(loadState(sensorId) == 2) {
            Serial.println("FLIP");
        } else if (loadState(sensorId) == 1) {
            Serial.println("ON");
        } else if (loadState(sensorId) == 0) {
            Serial.println("OFF");
        }
        // End of Debug
    } else {
        saveState(sensorId, state);

        // Debug
        Serial.print("Setting state for OneButton.tick(): ");
        Serial.print(sensor.description);
        Serial.print(", ");
        Serial.print("Sensor State after Button.tick(): ");
        if(loadState(sensorId) == 2) {
            Serial.println("FLIP");
        } else if (loadState(sensorId) == 1) {
            Serial.println("ON");
        } else if (loadState(sensorId) == 0) {
            Serial.println("OFF");
        }
        // End of Debug
    }
    updateRelayStateAndSendMessage(sensorId);
}


void setRelayON(void *pSensorId) {
    switchRelay(pSensorId, Toggle::ON);
}

void setRelayOFF(void *pSensorId) {
    switchRelay(pSensorId, Toggle::OFF);
}

void setRelay(void *pSensorId) {
    switchRelay(pSensorId);
}

void setupButtons() {
    // Setup the button.
    saloonMain.attachLongPressStart(setRelayON, SALOON_1_ID);
    saloonMain.attachLongPressStop(setRelayOFF, SALOON_1_ID);
    saloonMain.attachClick(setRelay, SALOON_1_ID);

    saloonSide.attachLongPressStart(setRelayON, SALOON_2_ID);
    saloonSide.attachLongPressStop(setRelayOFF, SALOON_2_ID);

//    diningRoomMain.attachLongPressStart(setRelayON, DINING_ROOM_1_ID);
//    diningRoomMain.attachLongPressStop(setRelayOFF, DINING_ROOM_1_ID);
    diningRoomMain.attachClick(setRelay, DINING_ROOM_1_ID);
}











