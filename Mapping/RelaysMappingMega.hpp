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

#include <Wire.h>
#include <OneButton.h>
#include <Adafruit_MCP23017.h>
#include "../Relay/Relay.hpp"


// MCP23017 expander declaration
uint8_t expanderAddresses[] = {0, 1, 2, 3, 4, 5, 6, 7};
const int numberOfExpanders = sizeof(expanderAddresses);
Adafruit_MCP23017 expander[numberOfExpanders];

// Relays declaration
// Relay(uint8_t pin, bool lowLevelTrigger = false, expanderAddress = 8)
// MCP23017 relays
Relay relay000(0, true, 0);
Relay relay001(1, true, 0);
Relay relay002(2, true, 0);
Relay relay003(3, true, 0);
Relay relay004(4, true, 0);
Relay relay005(5, true, 0);
Relay relay006(6, true, 0);
Relay relay007(7, true, 0);
Relay relay008(8, true, 0);
Relay relay009(9, true, 0);
Relay relay010(10, true, 0);
Relay relay011(11, true, 0);
Relay relay012(12, true, 0);
Relay relay013(13, true, 0);
Relay relay014(14, true, 0);
Relay relay015(15, true, 0);
Relay relay100(0, true, 1);
Relay relay101(1, true, 1);
Relay relay102(2, true, 1);
Relay relay103(3, true, 1);
Relay relay104(4, true, 1);
Relay relay105(5, true, 1);
Relay relay106(6, true, 1);
Relay relay107(7, true, 1);
Relay relay108(8, true, 1);
Relay relay109(9, true, 1);
Relay relay110(10, true, 1);
Relay relay111(11, true, 1);
Relay relay112(12, true, 1);
Relay relay113(13, true, 1);
Relay relay114(14, true, 1);
Relay relay115(15, true, 1);
// Arduino relays
Relay relay805(5,true);

// Child ID declaration of RelaysStruct
const uint8_t SALOON_1_ID = 11;
const uint8_t SALOON_2_ID = 12;
const uint8_t DINING_ROOM_1_ID = 13;
const uint8_t DINING_ROOM_2_ID = 14;
const uint8_t BEDROOM_1_ID = 21;
const uint8_t BEDROOM_2_ID = 22;
const uint8_t KID_BEDROOM1_ID = 23;
const uint8_t KID_BEDROOM2_ID = 24;
const uint8_t BATHROOM_1_ID = 31;
const uint8_t BATHROOM_2_ID = 32;
const uint8_t KITCHEN_1_ID = 33;
const uint8_t KITCHEN_2_ID = 34;
const uint8_t ENTRANCE_ID = 41;
const uint8_t LANDING_ID = 42;
const uint8_t HALL_ID = 43;
const uint8_t CORRIDOR_ID = 44;
const uint8_t TERRACE_MAIN_ID = 51;
const uint8_t TERRACE_BACK_ID = 52;
const uint8_t GARDEN_FRONT_ID = 53;
const uint8_t GARDEN_BACK_ID = 54;
const uint8_t HOUSE_EAST_ID = 55;
const uint8_t HOUSE_WEST_ID = 56;
const uint8_t GATE_ID = 57;
const uint8_t ATTIC_1_ID = 61;
const uint8_t ATTIC_2_ID = 62;
const uint8_t FAN_BATHROOM_1_ID = 71;
const uint8_t FAN_BATHROOM_2_ID = 72;
const uint8_t SPARE = 81;

typedef struct {
    const uint8_t id;
    char *description;
    const uint8_t signalPin; // pin to read the state of latch relay-button for relayStruct
    bool hasPin; // true if has latch relay-button assign to read the state from
    Relay relay;
    const uint8_t expanderAddress;

    uint8_t getId() {
        return id;
    }

    char *getDescription() {
        return description;
    }

    uint8_t getPin() {
        return signalPin;
    }

    // State of the relayStruct(sensor) is based on the state of signal pin
    bool hasSignalPin() {
        return hasPin;
    }

    uint8_t getExpanderAddress() {
        return expanderAddress;
    }

    bool onExpander() {

        if (expanderAddress >= 0 && expanderAddress < 8) {
            return true;
        } else {
            return false;
        }
    }
} RelayStruct;

// D0, D1, D10, D13, D50, D51, D52, D53 - do not use
RelayStruct relaySensors[] = {
//  Child ID               description   signalPin(Button) / hasSignalPin / relay / expanderAddress
        {SALOON_1_ID,       "Salon Glowne",        22, true,  relay000, 8},
        {SALOON_2_ID,       "Salon Kinkiety",      23, true,  relay001, 8},
        {DINING_ROOM_1_ID,  "Jadalnia Glowne",     24, true,  relay002, 8},
        {DINING_ROOM_2_ID,  "Jadalnia Kinkiety",   25, true,  relay003, 8},
        {BEDROOM_1_ID,      "Sypialnia Glowne",    26, true,  relay004, 8},
        {BEDROOM_2_ID,      "Sypialnia Kinkiety",  27, true,  relay005, 8},
        {KID_BEDROOM1_ID,   "Pokoj Dzieci",        28, true,  relay006, 8},
        {KID_BEDROOM2_ID,   "Pokoj Dzieci",        29, true,  relay007, 8},
        {BATHROOM_1_ID,     "Lazienka",            30, true,  relay008, 8},
        {BATHROOM_2_ID,     "Toaleta - Gospod",    31, true,  relay009, 8},
        {KITCHEN_1_ID,      "Kuchnia Glowne",      32, true,  relay010, 8},
        {KITCHEN_2_ID,      "Kuchnia Nadblatowe",  33, true,  relay011, 8},
        {ENTRANCE_ID,       "Wejscie Glowne",      34, true,  relay012, 8},
        {LANDING_ID,        "Wiatrolap",           35, true,  relay013, 8},
        {HALL_ID,           "Przedpokoj",          36, true,  relay014, 8},
        {CORRIDOR_ID,       "Korytarz",            37, true,  relay015, 8},
        {TERRACE_MAIN_ID,   "Taras - Glowny",      38, true,  relay100, 8},
        {TERRACE_BACK_ID,   "Taras - Ogrod",       39, true,  relay101, 8},
        {GARDEN_FRONT_ID,   "Ogrod - Przod",       40, true,  relay102, 8},
        {GARDEN_BACK_ID,    "Ogrod - Tyl",         41, true,  relay103, 8},
        {HOUSE_EAST_ID,     "Dom - Wschod",        42, true,  relay104, 8},
        {HOUSE_WEST_ID,     "Dom - Zachod",        43, true,  relay105, 8},
        {GATE_ID,           "Brama",               44, true,  relay106, 8},
        {ATTIC_1_ID,        "Strych 1",            45, true,  relay107, 8},
        {ATTIC_2_ID,        "Strych 2",            46, true,  relay108, 8},
        {FAN_BATHROOM_1_ID, "Lazienka Wentylator", 2,  false, relay109, 8},
        {FAN_BATHROOM_2_ID, "Toaleta Wentylator",  3,  false, relay110, 8},
        {SPARE,             "Master",              4,  false, relay805, 8},
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