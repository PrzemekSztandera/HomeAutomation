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
        Relay relay = getRelay(relaySensors[i].getId());

        if (relaySensors[i].getPinType() == TRIGGER_PIN) {
            if (relay.isLatching()) {
                buttons[i].setDebounceTicks(50);
                buttons[i].attachClick(switchRelay, relaySensors[i].getId());
//                buttons[i].attachLongPressStart(switchRelay, relaySensors[i].getId());
//                buttons[i].attachLongPressStop(switchRelay, relaySensors[i].getId());
                buttons[i].setPressTicks(1000);
#ifdef SETUP_DEBUG
                Serial.print(F("Trigger button set for sensor: "));
                Serial.println(relaySensors[i].getId());
#endif
            } else if (!relay.isLatching() && relay.getPin() == 57) {
                buttons[i].setDebounceTicks(50);
                buttons[i].attachClick(switchRelay, relaySensors[i].getId());
                buttons[i].setPressTicks(1000);
#ifdef SETUP_DEBUG
                Serial.print(F("Trigger button set for sensor: "));
                Serial.println(relaySensors[i].getId());
#endif
            } else {
#ifdef SETUP_DEBUG
                Serial.print(F("Error: Wrong relay type for RelaySensor: "));
                Serial.print(relaySensors[i].getId());
                Serial.println(F(" Only latching relay allowed when sensor pin set to: TRIGGER"));
                Serial.println(F("Refactor code and load again..."));
#endif
                flag = true;

            }
        } else if (relaySensors[i].getPinType() == SIGNAL_PIN) {
            if (!relay.isLatching()) {
                buttons[i].attachLongPressStart(readAndUpdateStateHelper, relaySensors[i].getId());
                buttons[i].attachLongPressStop(readAndUpdateStateHelper, relaySensors[i].getId());
#ifdef SETUP_DEBUG
                Serial.print(F("Signal button set for sensor: "));
                Serial.println(relaySensors[i].getId());
#endif
            } else {
#ifdef SETUP_DEBUG
                Serial.print(F("Error: Wrong relay type for RelaySensor: "));
                Serial.print(relaySensors[i].getId());
                Serial.println(F(" Only non-latching relay allowed when sensor pin set to: SIGNAL"));
                Serial.println(F("Refactor code and load again..."));
#endif
                flag = true;
            }
        }
    }
    while (flag) {
#ifdef SETUP_DEBUG
        Serial.println(F("waiting..."));
#endif
        myDelay(5000);
    }
}