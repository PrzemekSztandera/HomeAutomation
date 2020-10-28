/**
 *
 * @file MappingMega.hpp
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Mappings.
 * @license GPL V2
 *
 */

#pragma once

//#include <Wire.h>
#include <OneButton.h>
#include "../Relay/Relay.hpp"




// Relays declaration
// Relay(uint8_t pin, bool lowLevelTrigger = false, bool momentary = true, expanderAddress = 8)
// MCP23017 relays
Relay relay000(0, true, true, 0);
Relay relay001(1, true, true, 0);
Relay relay002(2, true, true, 0);
Relay relay003(3, true, true, 0);
Relay relay004(4, true, true, 0);
Relay relay005(5, true, true, 0);
Relay relay006(6, true, true, 0);
Relay relay007(7, true, true, 0);
Relay relay008(8, true, true, 0);
Relay relay009(9, true, true, 0);
Relay relay010(10, true, true, 0);
Relay relay011(11, true, true, 0);
Relay relay012(12, true, true, 0);
Relay relay013(13, true, true, 0);
Relay relay014(14, true, true, 0);
Relay relay015(15, true, true, 0);
Relay relay100(0, true, true, 1);
Relay relay101(1, true, true, 1);
Relay relay102(2, true, true, 1);
Relay relay103(3, true, true, 1);
Relay relay104(4, true, true, 1);
Relay relay105(5, true, true, 1);
Relay relay106(6, true, true, 1);
Relay relay107(7, true, true, 1);
Relay relay108(8, true, true, 1);
Relay relay109(9, true, true, 1);
Relay relay110(10, true, true, 1);
Relay relay111(11, true, true, 1);
Relay relay112(12, true, true, 1);
Relay relay113(13, true, true, 1);
Relay relay114(14, true, true, 1);
Relay relay115(15, true, true, 1);
// Arduino relays
Relay relayArduino9(9, true, false, 8);


// Child ID declaration of RelaysStruct
const uint8_t SALOON_1_ID = 11;
const uint8_t SALOON_2_ID = 12;
const uint8_t SALOON_3_ID = 13;
const uint8_t DINING_ROOM_1_ID = 14;
const uint8_t DINING_ROOM_2_ID = 15;
const uint8_t BEDROOM_1_ID = 21;
const uint8_t BEDROOM_2_ID = 22;
const uint8_t ROOM_1_ID = 23;
const uint8_t ROOM_2_ID = 24;
const uint8_t BATHROOM_ID = 31;
const uint8_t KITCHEN_1_ID = 32;
const uint8_t KITCHEN_2_ID = 33;
const uint8_t ENTRANCE_ID = 34;
const uint8_t LANDING_ID = 35;
const uint8_t HALL_ID = 36;
const uint8_t CORRIDOR_ID = 37;
const uint8_t UTILITY_ROOM = 38;
const uint8_t TERRACE_1_ID = 41;
const uint8_t TERRACE_2_ID = 42;
const uint8_t HOUSE_1_ID = 43;
const uint8_t HOUSE_2_ID = 44;
const uint8_t HOUSE_3_ID = 45;
const uint8_t GARDEN_1_ID = 51;
const uint8_t GARDEN_2_ID = 52;
const uint8_t ATTIC_1_ID = 53;
const uint8_t ATTIC_2_ID = 54;
const uint8_t ATTIC_3_ID = 55;
const uint8_t ATTIC_4_ID = 56;
const uint8_t FAN_1_ID = 61;
const uint8_t FAN_2_ID = 62;
const uint8_t SPARE_SIGNAL_1 = 71;
const uint8_t SPARE_SIGNAL_2 = 72;
const uint8_t SPARE_BUTTON = 73;


typedef struct {

    const uint8_t id;
    char *description;
    const uint8_t _signalPin; // pin to read the state of latch relay-button for relayStruct
    const uint8_t _expanderAddress;
    bool _hasPin; // true if has latch relay-button assign to read the state from
    Relay relay;


    uint8_t getId() {
        return id;
    }

    char *getDescription() {
        return description;
    }

    uint8_t getPin() {
        return _signalPin;
    }

    // State of the relayStruct(sensor) is based on the state of signal pin
    bool hasSignalPin() {
        return _hasPin;
    }

    uint8_t getExpanderAddress() {
        return _expanderAddress;
    }

    bool onExpander() {

        if (_expanderAddress >= 0 && _expanderAddress < 8) {
            return true;
        } else {
            return false;
        }
    }

    uint8_t readPin() {
        uint8_t signalState = 2;
        if (this -> onExpander()) {
            signalState = expander[_expanderAddress].digitalRead(this -> getPin());
        } else {
            signalState = digitalRead(this -> getPin());
        }
        return signalState;
    }
} RelayStruct;

// D0, D1, D10, D13, D50, D51, D52, D53 - do not use
RelayStruct relaySensors[] = {
//  Child ID               description   signalPin(or button) / expanderAddress / hasSignalPin / relay
        // Sensors using Button as signal pin  IN-signal       OUT - relay on expander
        {SALOON_1_ID,      "Salon - Glowne",            22, 8, true,  relay000},    // 0
        {SALOON_2_ID,      "Salon - Kinkiety(Sofa)",    23, 8, true,  relay001},    // 1
        {SALOON_3_ID,      "Salon - Kinkiety(TV)",      24, 8, true,  relay002},    // 2
        {DINING_ROOM_1_ID, "Jadalnia - Glowne",         25, 8, true,  relay003},    // 3
        {DINING_ROOM_2_ID, "Jadalnia - Kinkiety",       26, 8, true,  relay004},    // 4
        {BEDROOM_1_ID,     "Sypialnia - Glowne",        27, 8, true,  relay005},    // 5
        {BEDROOM_2_ID,     "Sypialnia - Kinkiety",      28, 8, true,  relay006},    // 6
        {ROOM_1_ID,        "Pokoj - Prawy",             29, 8, true,  relay007},    // 7
        {ROOM_2_ID,        "Pokoj - Lewy",              30, 8, true,  relay008},    // 8
        {BATHROOM_ID,      "Lazienka",                  31, 8, true,  relay009},    // 9
        {KITCHEN_1_ID,     "Kuchnia - Glowne",          32, 8, true,  relay010},    // 10
        {KITCHEN_2_ID,     "Kuchnia - Dodatkowe",       33, 8, true,  relay011},    // 11
        {ENTRANCE_ID,      "Wejscie",                   34, 8, true,  relay012},    // 12
        {LANDING_ID,       "Wiatrolap",                 35, 8, true,  relay013},    // 13
        {HALL_ID,          "Przedpokoj",                36, 8, true,  relay014},    // 14
        {CORRIDOR_ID,      "Korytarz",                  37, 8, true,  relay015},    // 15
        {UTILITY_ROOM,     "Kotlownia",                 38, 8, true,  relay100},    // 16
        {TERRACE_1_ID,     "Taras - Zachod",            39, 8, true,  relay101},    // 17
        {TERRACE_2_ID,     "Taras - Poludnie",          40, 8, true,  relay102},    // 18
        {HOUSE_1_ID,       "Dom - Zachod",              41, 8, true,  relay103},    // 19
        {HOUSE_2_ID,       "Dom - Wschod",              42, 8, true,  relay104},    // 20
        {HOUSE_3_ID,       "Dom - Christmass",          43, 8, true,  relay105},    // 21
        {GARDEN_1_ID,      "Ogrod - Przod",             44, 8, true,  relay106},    // 22
        {GARDEN_2_ID,      "Ogrod - Tyl",               45, 8, true,  relay107},    // 23
        {ATTIC_1_ID,       "Strych - Wejscie",          46, 8, true,  relay108},    // 24
        {ATTIC_2_ID,       "Strych 1",                  47, 8, true,  relay109},    // 25
        {ATTIC_3_ID,       "Strych 2",                  48, 8, true,  relay110},    // 26
        {ATTIC_4_ID,       "Strych 3",                  49, 8, true,  relay111},    // 27
        {FAN_1_ID,         "Wentylator - Lazienka",      2, 8, true,  relay112},    // 28
        {FAN_2_ID,         "Wentylator - Kotlownia",     3, 8, true,  relay113},    // 29
        {SPARE_SIGNAL_1,   "Spare signal on pin 4",      4, 8, true,  relay114},    // 30
        {SPARE_SIGNAL_2,   "Spare signal on pin 5",      5, 8, true,  relay115},    // 31
        // Sensors using Button as button      IN-button       OUT - relay on Arduino
        {SPARE_BUTTON,     "Spare button on pin 6",      6,  8, false, relayArduino9}, // 32
};
const uint8_t numberOfRelayStruct = sizeof(relaySensors) / sizeof(RelayStruct);
MyMessage msgs[numberOfRelayStruct];

//    const uint8_t sensorId = static_cast<uint8_t>(reinterpret_cast<intptr_t>(pSensorId));

byte getIndex(uint8_t sensorId) {
    for (uint8_t i = 0; i < numberOfRelayStruct; i++) {
        if (relaySensors[i].getId() == sensorId) return (i);
    }
    return (-1);
}

RelayStruct getRelayStruct(uint8_t sensorId) {
    uint8_t index = getIndex(sensorId);
    return relaySensors[index];
}

Relay getRelay(uint8_t sensorId) {
    auto relayStruct = getRelayStruct(sensorId);
    return relayStruct.relay;
}

// Buttons declaration; add new button to readButtons()
// OneButton(int pin, int active = LOW, bool pullUpActive = true) - default constructor;
OneButton buttons[numberOfRelayStruct];
const uint8_t numberOfButtons = sizeof(buttons) / sizeof(OneButton);

void createButtons() {
    for (uint8_t i = 0; i < numberOfButtons; i++) {
        buttons[i] = OneButton(relaySensors[i].getPin());
    }
}