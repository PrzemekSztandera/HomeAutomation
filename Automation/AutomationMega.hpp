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
void clickRelay(int pin) {
    uint8_t state = digitalRead(pin);
    unsigned long currentMillis = millis();
    digitalWrite(pin, !state);
    myDelay(125);
    digitalWrite(pin, state);
}

// relay acts as a press button
void pressRelay(int pin) {
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
    auto relay = getRelay(sensorId);
    uint8_t state = 2;

    if (sensor.hasSignalPin) {
        state = digitalRead(sensor.signalPin);
        if (pullUpActive) {
            saveState(sensorId, !state);
        } else {
            saveState(sensorId, state);
        }
        Serial.print(" and pin: ");
        Serial.println(sensor.signalPin);
    } else {
        state = digitalRead(relay.getPin());
        if (relay.isLowLevelTrigger()) {
            state = !state;
        }
        saveState(sensorId, state);

        Serial.print(" and pin: ");
        Serial.println(relay.getPin());
    }
    send(msgs[index].set(loadState(sensorId)));

    // Debug
    Serial.print("Sensor State after updateRelayStateAndSendMessage(): ");
    if (loadState(sensorId) == 2) {
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
    if (cmd == 2) {
        Serial.print("FLIP");
    } else if (cmd == 1) {
        Serial.print("ON");
    } else if (cmd == 0) {
        Serial.print("OFF");
    }
    Serial.print("; Sensor State Before: ");
    if (loadState(sensorId) == 2) {
        Serial.println("FLIP");
    } else if (loadState(sensorId) == 1) {
        Serial.println("ON");
    } else if (loadState(sensorId) == 0) {
        Serial.println("OFF");
    }
    // End of Debug


    auto sensor = getSensor(sensorId);
    uint8_t index = getIndex(sensorId);
    auto relay = getRelay(sensorId);

    // check whether flip state of sensor
    uint8_t state = (cmd == Toggle::FLIP) ? !loadState(sensorId) : cmd; // OFF -> ON
    saveState(sensorId, state);

    if (cmd == Toggle::FLIP && sensor.hasSignalPin) {
        clickRelay(relay.getPin());

        // Debug
        Serial.print("Calling relayAsPushButton(); ");
        Serial.print("Sensor State: ");
        if (loadState(sensorId) == 2) {
            Serial.println("FLIP");
        } else if (loadState(sensorId) == 1) {
            Serial.println("ON");
        } else if (loadState(sensorId) == 0) {
            Serial.println("OFF");
        }
        // End of Debug

    } else if (cmd == Toggle::FLIP && !sensor.hasSignalPin) {
        pressRelay(relay.getPin());

        // Debug
        Serial.print("Calling relayAsLatchButton(); ");
        Serial.print("Sensor State: ");
        if (loadState(sensorId) == 2) {
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


void setOutputON(void *pSensorId) {
    saveState(pSensorId, Toggle::ON);
    auto sensor = getSensor(pSensorId);
    // Debug
    Serial.print("Setting state for OneButton.tick(): ");
    Serial.print(sensor.description);
    Serial.print(", ");
    Serial.print("Sensor State after Button.tick(): ");
    if (loadState(sensor.id) == 2) {
        Serial.println("FLIP");
    } else if (loadState(sensor.id) == 1) {
        Serial.println("ON");
    } else if (loadState(sensor.id) == 0) {
        Serial.println("OFF");
    }
    // End of Debug
    updateRelayStateAndSendMessage(pSensorId);
}

void setOutputOFF(void *pSensorId) {
    saveState(pSensorId, Toggle::OFF);
    auto sensor = getSensor(pSensorId);
    // Debug
    Serial.print("Setting state for OneButton.tick(): ");
    Serial.print(sensor.description);
    Serial.print(", ");
    Serial.print("Sensor State after Button.tick(): ");
    if (loadState(sensor.id) == 2) {
        Serial.println("FLIP");
    } else if (loadState(sensor.id) == 1) {
        Serial.println("ON");
    } else if (loadState(sensor.id) == 0) {
        Serial.println("OFF");
    }
    // End of Debug
    updateRelayStateAndSendMessage(pSensorId);
}

void flipOutput(void *pSensorId) {
    switchRelay(pSensorId);
}

void masterClickButton() {
    flipOutput(SALOON_1_ID);
    flipOutput(SALOON_2_ID);
    flipOutput(DINING_ROOM_1_ID);
}

// Setup the buttons and relays. Do not assign LongPress and Click to the same sensor
void setupClickButtons() {

    button2.attachClick(flipOutput, SALOON_1_ID);
    button4.attachClick(flipOutput, DINING_ROOM_1_ID);
    masterButton7.attachClick(masterClickButton);
}

void setupPressButtons() {

    button3.attachLongPressStart(setOutputON, SALOON_2_ID);
    button3.attachLongPressStop(setOutputOFF, SALOON_2_ID);
}











