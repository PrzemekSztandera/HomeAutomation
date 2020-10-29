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

OneButton buttons[numberOfRelayStruct];
const uint8_t numberOfButtons = sizeof(buttons) / sizeof(OneButton);

// Buttons declaration; add new button to readButtons()
// OneButton(int pin, int active = LOW, bool pullUpActive = true) - default constructor;
void createButtons() {
    for (uint8_t i = 0; i < numberOfRelayStruct; i++) {
        buttons[i] = OneButton(relaySensors[i].getPin());
    }
}