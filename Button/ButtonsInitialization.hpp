/**
 *
 * @file ButtonsInitialization.hpp
 * @author Przemyslaw Sztandera
 * Buttons
 * @license GPL V2
 *
 */

#pragma once

#include <OneButton.h>

OneButton buttons[numberOfRelaySensors];
const uint8_t numberOfButtons = sizeof(buttons) / sizeof(OneButton);

// Buttons declaration; add new button to readButtons()
// OneButton(int pin, int active = LOW, bool pullUpActive = true) - default constructor - connected to GND;
void createButtons() {
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        buttons[i] = OneButton(relaySensors[i].getPin(), true, true);
    }
}


void readButtons() {
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        buttons[i].tick();
    }
}

void pressButton() {
//    switchRelay(...);
}

void readAndUpdateStateHelper(uint8_t sensorId) {
    updateRelayStateAndSendMessage(sensorId);
}

void setupButtons() {
    bool flag = false;
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        Relay relay = relaySensors[i].getRelay();

        if (relaySensors[i].getPinType() == TRIGGER_PIN) {
            if (relay.isLatching()) {
                buttons[i].setDebounceTicks(50);
                buttons[i].attachClick(switchRelay, relaySensors[i].getId());
//                buttons[i].attachLongPressStart(switchRelay, relaySensors[i].getId());
//                buttons[i].attachLongPressStop(switchRelay, relaySensors[i].getId());
                buttons[i].setPressTicks(1000);
                Serial.print("Trigger button set for sensor: ");
                Serial.println(relaySensors[i].getId());
            } else if (!relay.isLatching() && relay.getPin() == 57) {
                buttons[i].setDebounceTicks(50);
                buttons[i].attachClick(switchRelay, relaySensors[i].getId());
                buttons[i].setPressTicks(1000);
                Serial.print("Trigger button set for reset sensor: ");
                Serial.println(relaySensors[i].getId());
            } else {
                Serial.print("Error: Wrong relay type for RelaySensor: ");
                Serial.print(relaySensors[i].getId());
                Serial.println(" Only latching relay allowed when sensor pin set to: TRIGGER");
                Serial.println("Refactor code and load again...");
                flag = true;

            }
        } else if (relaySensors[i].getPinType() == SIGNAL_PIN) {
            if (!relay.isLatching()) {
                buttons[i].attachLongPressStart(readAndUpdateStateHelper, relaySensors[i].getId());
                buttons[i].attachLongPressStop(readAndUpdateStateHelper, relaySensors[i].getId());
                Serial.print("Signal button set for sensor: ");
                Serial.println(relaySensors[i].getId());
            } else {
                Serial.print("Error: Wrong relay type for RelaySensor: ");
                Serial.print(relaySensors[i].getId());
                Serial.println(" Only non-latching relay allowed when sensor pin set to: SIGNAL");
                Serial.println("Refactor code and load again...");
                flag = true;
            }
        }
    }
    while (flag) {
        Serial.println("waiting...");
        myDelay(5000);
    }
}