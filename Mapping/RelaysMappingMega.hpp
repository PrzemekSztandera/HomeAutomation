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

Relay relay112(12, true, false, 1);
Relay relay113(13, true, false, 1);
Relay relay114(14, true, false, 1);
Relay relay115(15, true, false, 1);
Relay relayArduino9(9, true, false, 8);


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
const uint8_t SPARE_1 = 81;
const uint8_t SPARE_2 = 82;
const uint8_t SPARE_3 = 83;
const uint8_t SPARE_4 = 84;
const uint8_t SPARE_5 = 85;
const uint8_t SPARE_6 = 86;
const uint8_t SPARE_7 = 87;
const uint8_t SPARE_8 = 88;
const uint8_t SPARE_9 = 89;

typedef struct {
    const uint8_t id;
    char *description;
    const uint8_t signalPin; // pin to read the state of latch relay-button for relayStruct
    const uint8_t expanderAddress;
    bool hasPin; // true if has latch relay-button assign to read the state from
    Relay relay;

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
//  Child ID               description   signalPin(Button) / expanderAddress / hasSignalPin / relay
        // Sensors using Button as signal pin  IN-signal       OUT - relay on expander
        {SALOON_1_ID,      "Salon Glowne",       22, 8, true,  relay000}, // 1
        {SALOON_2_ID,      "Salon Kinkiety",     23, 8, true,  relay001}, // 2
        {DINING_ROOM_1_ID, "Jadalnia Glowne",    24, 8, true,  relay002}, // 3
        {DINING_ROOM_2_ID, "Jadalnia Kinkiety",  25, 8, true,  relay003}, // 4
        {BEDROOM_1_ID,     "Sypialnia Glowne",   26, 8, true,  relay004}, // 5
        {BEDROOM_2_ID,     "Sypialnia Kinkiety", 27, 8, true,  relay005}, // 6
        {KID_BEDROOM1_ID,  "Pokoj Dzieci",       28, 8, true,  relay006}, // 7
        {KID_BEDROOM2_ID,  "Pokoj Dzieci",       29, 8, true,  relay007}, // 8
        {BATHROOM_1_ID,    "Lazienka",           30, 8, true,  relay008}, // 9
        {BATHROOM_2_ID,    "Toaleta - Gospod",   31, 8, true,  relay009}, // 10
        {KITCHEN_1_ID,     "Kuchnia Glowne",     32, 8, true,  relay010}, // 11
        {KITCHEN_2_ID,     "Kuchnia Nadblatowe", 33, 8, true,  relay011}, // 12
        {ENTRANCE_ID,      "Wejscie Glowne",     34, 8, true,  relay012}, // 13
        {LANDING_ID,       "Wiatrolap",          35, 8, true,  relay013}, // 14
        {HALL_ID,          "Przedpokoj",         36, 8, true,  relay014}, // 15
        {CORRIDOR_ID,      "Korytarz",           37, 8, true,  relay015}, // 16
        {TERRACE_MAIN_ID,  "Taras - Glowny",     38, 8, true,  relay100}, // 17
        {TERRACE_BACK_ID,  "Taras - Ogrod",      39, 8, true,  relay101}, // 18
        {GARDEN_FRONT_ID,  "Ogrod - Przod",      40, 8, true,  relay102}, // 19
        {GARDEN_BACK_ID,   "Ogrod - Tyl",        41, 8, true,  relay103}, // 20
        {HOUSE_EAST_ID,    "Dom - Wschod",       42, 8, true,  relay104}, // 21
        {HOUSE_WEST_ID,    "Dom - Zachod",       43, 8, true,  relay105}, // 22
        {GATE_ID,          "Brama",              44, 8, true,  relay106}, // 23
        {ATTIC_1_ID,       "Strych 1",           45, 8, true,  relay107}, // 24
        {ATTIC_2_ID,       "Strych 2",           46, 8, true,  relay108}, // 25
// ---------------------------------------------------------------------------- //
        {SPARE_1,          "Spare 1",            47, 8, true,  relay109}, // 26
        {SPARE_2,          "Spare 2",            48, 8, true,  relay110}, // 27
        {SPARE_3,          "Spare 3",            49, 8, true,  relay111}, // 28
        // Sensors using Button as button      IN-button       OUT - relay on expander
        {SPARE_4,          "Spare 4",            2,  8, false, relay112}, // 29
        {SPARE_5,          "Spare 5",            3,  8, false, relay113}, // 30
        {SPARE_6,          "Spare 6",            4,  8, false, relay114}, // 31
        {SPARE_7,          "Spare 7",            5,  8, false, relay115}, // 32
        {SPARE_8,          "Spare 8",            6,  8, false, relay000}, // 33
        // Sensors using Button as button      IN-button       OUT - relay on Arduino
        {SPARE_9,          "Spare 9",            8,  8, false, relayArduino9}, // 34
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