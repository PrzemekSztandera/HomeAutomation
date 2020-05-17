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
void relayAsPushButton(int pin, bool activeLow) {
    uint8_t state = digitalRead(pin);
    unsigned long currentMillis = millis();
    digitalWrite(pin, !state);
    myDelay(125);
    digitalWrite(pin, state);
}

// relay acts as a latch button
void relayAsLatchButton(int pin, bool activeLow) {
    uint8_t pinState = digitalRead(pin);
    digitalWrite(pin, !pinState);
}


void setOutput(const uint8_t &sensorId, const uint8_t &cmd = Toggle::FLIP, const bool actAsPushButton = true) {
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
    Serial.print("; Sensor State: ");
    if(loadState(sensorId) == 2) {
        Serial.println("FLIP");
    } else if (loadState(sensorId) == 1) {
        Serial.println("ON");
    } else if (loadState(sensorId) == 0) {
        Serial.println("OFF");
    }
// Sensors[index]
    auto sensor = getSensor(sensorId);
    uint8_t index = getIndex(sensorId);

    // check whether flip state of sensor
    uint8_t state = (cmd == Toggle::FLIP) ? !loadState(sensorId) : cmd; // OFF -> ON
    saveState(sensorId, state);

    if (cmd == Toggle::FLIP && actAsPushButton) {
        Serial.print("Calling relayAsPushButton(); ");
        relayAsPushButton(sensor.pin, sensor.activeLow);
        state = !loadState(sensorId);
        saveState(sensorId, state);

        Serial.print("Sensor State: ");
        if(loadState(sensorId) == 2) {
            Serial.println("FLIP");
        } else if (loadState(sensorId) == 1) {
            Serial.println("ON");
        } else if (loadState(sensorId) == 0) {
            Serial.println("OFF");
        }
    } else if (cmd == Toggle::FLIP && !actAsPushButton) {
        relayAsLatchButton(sensor.pin, sensor.activeLow);
        saveState(sensorId, state); // ON
    } else {
        Serial.print("Setting state for OneButton.tick(): ");
        Serial.print(sensor.description);
        Serial.print(", ");

        saveState(sensorId, state);

        Serial.print("Sensor State after Button.tick(): ");
        if(loadState(sensorId) == 2) {
            Serial.println("FLIP");
        } else if (loadState(sensorId) == 1) {
            Serial.println("ON");
        } else if (loadState(sensorId) == 0) {
            Serial.println("OFF");
        }
    }
    send(msgs[index].set(state));

}

void checkRelayState(const uint8_t &sensorId, bool actAsPushButton = true) {
    Serial.println("checkRelayState() for sensor: ");
    Serial.print(sensorId);

// Sensors[index]
    auto sensor = getSensor(sensorId);
    uint8_t index = getIndex(sensor.id);

    uint8_t state = loadState(sensorId);

    if (actAsPushButton) {
        if (digitalRead(sensor.buttonPin) == HIGH) {
            state = Toggle::OFF;
            saveState(sensorId, state);
            send(msgs[index].set(state));
        }
//    } else {
//        if ((sensor.activeLow && digitalRead(sensor.pin) == HIGH) ||
//            (!sensor.activeLow && digitalRead(sensor.pin) == LOW)) {
//            state = Toggle::OFF;
//            saveState(sensorId, state);
//            send(msgs[index].set(state));
//        }
    }

    Serial.print("Sensor State after checkRelayState(): ");
    if(loadState(sensorId) == 2) {
        Serial.println("FLIP");
    } else if (loadState(sensorId) == 1) {
        Serial.println("ON");
    } else if (loadState(sensorId) == 0) {
        Serial.println("OFF");
    }
}

//void setState(const uint8_t& sensorId, const uint8_t& cmd = Toggle::OFF, bool actAsPushButton = true) {
//    // Sensors[index]
//    uint8_t index = getIndex(sensorId);
//    auto sensor = Sensors[index];
//    // check whether flip state of sensor
//    uint8_t state = (cmd == Toggle::FLIP) ? !loadState(sensor.id) : cmd;
//
//    saveState(sensor.id, state);
//    send(msgs[index].set(state));
//}

void setStateON(void *pSensorId) {
    setOutput(pSensorId, Toggle::ON);
}

void setStateOFF(void *pSensorId) {
    setOutput(pSensorId, Toggle::OFF);
}

void flipSwitch(void *pSensorId) {
    setOutput(pSensorId);
}

void latchSwitch(void *pSensorId) {
    setOutput(pSensorId, Toggle::FLIP, false);
}

void setupButtons() {
    // Setup the button.
    saloonMain.attachLongPressStart(setStateON, SALOON_1_ID);
    saloonMain.attachLongPressStop(setStateOFF, SALOON_1_ID);
    saloonMain.attachClick(flipSwitch, SALOON_1_ID);

    saloonSide.attachLongPressStart(setStateON, SALOON_2_ID);
    saloonSide.attachLongPressStop(setStateOFF, SALOON_2_ID);

    diningRoomMain.attachLongPressStart(setStateON, DINING_ROOM_1_ID);
    diningRoomMain.attachLongPressStop(setStateOFF, DINING_ROOM_1_ID);
    saloonMain.attachClick(flipSwitch, DINING_ROOM_1_ID);
}











