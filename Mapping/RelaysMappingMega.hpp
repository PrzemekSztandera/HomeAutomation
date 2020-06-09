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

namespace Toggle {
    const uint8_t OFF = 0;
    const uint8_t ON = 1;
    const uint8_t FLIP = 2;
}

// MCP23017 expander declaration
uint8_t expanderAddresses[] = {0,1,2,3,4,5,6,7};
const int numberOfExpanders = sizeof(expanderAddresses);
Adafruit_MCP23017 expander[numberOfExpanders];

// Push buttons declaration
// OneButton(int pin, int active = LOW, bool pullupActive = true);
OneButton button2(2);
OneButton button3(3);
OneButton button4(4);
OneButton masterButton7(7);


// Relays declaration
// Relay(uint8_t pin, bool lowLevelTrigger = false, expanderAddress = 8)
Relay relay10(10, true);
Relay relay11(11);
Relay relay12(12, true);
// On MCP23017 relays
Relay relay0(0, true, 0);


// Child ID declaration of RelaysStruct
const uint8_t SALOON_1_ID = 11;
const uint8_t SALOON_2_ID = 12;
const uint8_t DINING_ROOM_1_ID = 13;
const uint8_t DINING_ROOM_2_ID = 14;
const uint8_t BEDROOM_1_ID = 21;
//const uint8_t BEDROOM_2_ID = 22;
//const uint8_t KID1_BEDROOM_ID = 23;
//const uint8_t KID2_BEDROOM_ID = 24;
//const uint8_t BATHROOM_1_ID = 31;
//const uint8_t BATHROOM_2_ID = 32;
//const uint8_t FAN_BATHROOM_1_ID = 41;
//const uint8_t FAN_BATHROOM_2_ID = 42;
//const uint8_t KITCHEN_1_ID = 51;
//const uint8_t KITCHEN_2_ID = 52;
//const uint8_t ENTRANCE_ID = 61;
//const uint8_t LANDING_ID = 62;
//const uint8_t CORRIDOR_ID = 63;
//const uint8_t HALL_ID = 64;
//const uint8_t TERRACE_MAIN_ID = 71;
//const uint8_t TERRACE_BACK_ID = 72;
//const uint8_t HOUSE_OUTSIDE_ID = 73;
//const uint8_t GARDEN_ID = 74;
//const uint8_t GATE_ID = 75;
//const uint8_t ATTIC_1_ID = 81;
//const uint8_t ATTIC_2_ID = 82;
//const uint8_t MASTER = 83;

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

        if(expanderAddress < 8 && expanderAddress >= 0) {
            return true;
        } else {
            return false;
        }
    }
} RelayStruct;

RelayStruct Relays[] = {
//  Child ID               description   signalPin / hasSignalPin / relay / expanderAddress
//        {SALOON_1_ID,      "Salon Glowne",      -1, false, relay9}, // 23
        {SALOON_2_ID,      "Salon Kinkiety",    3,  true, relay10, 8}, // 25
        {DINING_ROOM_1_ID, "Jadalnia Glowne",   4,  true, relay11, 8}, // 27
        {DINING_ROOM_2_ID, "Jadalnia Kinkiety", 5,  true, relay12, 8}, // 29
        {BEDROOM_1_ID,     "Sypialnia Glowne",  -1, false, relay0, 8}, // 31
//  { BEDROOM_2_ID,      "Sypialnia Kinkiety",    33, false },
//  { KID1_BEDROOM_ID,   "Pokoj Wojtek",          35, false },
//  { KID2_BEDROOM_ID,   "Pokoj Marcel",          37, false },
//  { BATHROOM_1_ID,     "Lazienka",              39, false },
//  { BATHROOM_2_ID,     "Toaleta",               41, false },
//  { FAN_BATHROOM_1_ID, "Lazienka Wentylator",   43, false },
//  { FAN_BATHROOM_2_ID, "Toaleta Wentylator",    45, false },
//  { KITCHEN_1_ID,      "Kuchnia Glowne",        47, false },
//  { KITCHEN_2_ID,      "Kuchnia Blat",          49, false },
//  { ENTRANCE_ID,       "Wejscie",               0, false },
//  { LANDING_ID,        "Wiatrolap",             0, false },
//  { CORRIDOR_ID,       "Przedpokoj",            0, false },
//  { HALL_ID,           "Hall",                  0, false },
//  { TERRACE_MAIN_ID,   "Taras Glowny",          0, false },
//  { TERRACE_BACK_ID,   "Taras Ogrod",           0, false },
//  { HOUSE_OUTSIDE_ID,  "Swiatla zewnetrzne",    0, false },
//  { GARDEN_ID,         "Ogrod",                 0, false },
//  { GATE_ID,           "Brama",                 0, false },
//  { ATTIC_1_ID,        "Strych 1",              0, false },
//  { ATTIC_2_ID,        "Strych 2",              0, false },
//  { MASTER,            "Master",                0, false },
};
const uint8_t maxRelays = sizeof(Relays) / sizeof(RelayStruct);
MyMessage msgs[maxRelays];

//    const uint8_t sensorId = static_cast<uint8_t>(reinterpret_cast<intptr_t>(pSensorId));

byte getIndex(uint8_t sensorId) {
    for (uint8_t i = 0; i < maxRelays; i++) {
        if (Relays[i].getId() == sensorId) return (i);
    }
    return (-1);
}

RelayStruct getRelayStruct(uint8_t sensorId) {
    uint8_t index = getIndex(sensorId);
    return Relays[index];
}

Relay getRelay(uint8_t sensorId) {
    auto relayStruct = getRelayStruct(sensorId);
    return relayStruct.relay;
}
