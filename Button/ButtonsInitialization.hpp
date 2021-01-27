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

OneButton buttons[numberOfSensors];
const uint8_t numberOfButtons = sizeof(buttons) / sizeof(OneButton);


void readAndUpdateStateHelper(uint8_t sensorId) {
    updateRelayStateAndSendMessage(sensorId);
}

// Buttons declaration; add new button to readButtons()
// OneButton(int pin, int active = LOW, bool pullUpActive = true) - default constructor - connected to GND;
void createAndSetButtons() {
    bool flag = false;
    for (uint8_t i = 0; i < numberOfSensors; i++) {

        Sensor sensor = sensors[i];
        uint8_t sensorId = sensor.getId();



        if (sensor.getVariableType() == V_STATUS && sensor.getPresentationType() == S_BINARY) {

            buttons[i] = OneButton(sensor.getPin(), true, true);

            Relay relay = getRelay(sensorId);
            bool latching = relay.isLatching();

            if (sensor.getPinType() == TRIGGER_PIN) {
                if (latching) {
                    buttons[i].setDebounceTicks(50);
                    buttons[i].attachClick(switchRelay, sensorId);
//                buttons[i].attachLongPressStart(switchRelay, sensorId);
//                buttons[i].attachLongPressStop(switchRelay, sensorId);
                    buttons[i].setPressTicks(1000);
#ifdef SETUP_DEBUG
                    Serial.print(F("Trigger button set for sensor: "));
                    Serial.println(sensorId);
#endif
                } else {
                    flag = true;
#ifdef SETUP_DEBUG
                    Serial.print(F("Error: Wrong relay type for RelaySensor: "));
                    Serial.print(sensorId);
                    Serial.println(F(" Only latching relay allowed when sensor pin set to: TRIGGER"));
                    Serial.println(F("Refactor code and load again..."));
#endif

                }
            } else if (sensor.getPinType() == SIGNAL_PIN) {
                if (!latching) {
                    buttons[i].attachLongPressStart(readAndUpdateStateHelper, sensorId);
                    buttons[i].attachLongPressStop(readAndUpdateStateHelper, sensorId);
#ifdef SETUP_DEBUG
                    Serial.print(F("Signal button set for sensor: "));
                    Serial.println(sensorId);
#endif
                } else {
                    flag = true;
#ifdef SETUP_DEBUG
                    Serial.print(F("Error: Wrong relay type for RelaySensor: "));
                    Serial.print(sensorId);
                    Serial.println(F(" Only non-latching relay allowed when sensor pin set to: SIGNAL"));
                    Serial.println(F("Refactor code and load again..."));
#endif
                }
            }
        } else if (sensor.getVariableType() == V_TRIPPED && sensor.getPresentationType() == S_MOTION) {
            buttons[i] = OneButton(sensor.getPin(), true, true);
            buttons[i].attachLongPressStart(readAndUpdateStateHelper, sensorId);
            buttons[i].attachLongPressStop(readAndUpdateStateHelper, sensorId);
        }
    }

    while (flag) {
#ifdef SETUP_DEBUG
        Serial.println(F("waiting..."));
#endif
        myDelay(5000);
    }
}


void readButtons() {
    for (uint8_t i = 0; i < 35; i++) {
        buttons[i].tick();
    }
}

void pressButton() {
//    switchRelay(...);
}