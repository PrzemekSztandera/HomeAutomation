/**
 *
 * @file MappingMega.hpp
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Mappings
 * @license GPL V2
 *
 */

#pragma once

//#include <Wire.h>
#include "../Relay/Relay.hpp"


// Relays declaration
// Relay(uint8_t pin, bool lowLevelTrigger = false, bool latching = false, expanderAddress = 8)

// MCP23017 relays
Relay relayE000(0, false, false, 0);
Relay relayE001(1, false, false, 0);
Relay relayE002(2, false, false, 0);
Relay relayE003(3, false, false, 0);
Relay relayE004(4, false, false, 0);
Relay relayE005(5, false, false, 0);
Relay relayE006(6, false, false, 0);
Relay relayE007(7, false, false, 0);
Relay relayE008(8, false, false, 0);
Relay relayE009(9, false, false, 0);
Relay relayE010(10, false, false, 0);
Relay relayE011(11, false, false, 0);
Relay relayE012(12, false, false, 0);
Relay relayE013(13, false, false, 0);
Relay relayE014(14, false, false, 0);
Relay relayE015(15, false, false, 0);
Relay relayE100(0, false, false, 1);
Relay relayE101(1, false, false, 1);
Relay relayE102(2, false, false, 1);
Relay relayE103(3, false, false, 1);
Relay relayE104(4, false, false, 1);
Relay relayE105(5, false, false, 1);
Relay relayE106(6, false, false, 1);
Relay relayE107(7, false, false, 1);
Relay relayE108(8, false, false, 1);
Relay relayE109(9, false, false, 1);
Relay relayE110(10, false, false, 1);
Relay relayE111(11, false, false, 1);
Relay relayE112(12, false, false, 1);
Relay relayE113(13, false, false, 1);
Relay relayE114(14, false, false, 1);
Relay relayE115(15, false, false, 1);

// Arduino relays
Relay relayA08(8, false, true, 8);
Relay relayA09(9, true, true, 8);


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
const uint8_t SPARE_1_ID = 71;
const uint8_t SPARE_2_ID = 72;
const uint8_t CHARGER_ID = 73;
const uint8_t TEST_BUTTON = 74;


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
        return _expanderAddress >= 0 && _expanderAddress < 8 ? true : false;
    }

    uint8_t readPin() {
        uint8_t signalState = 2;
        if (this->onExpander()) {
            signalState = expander[this->getExpanderAddress()].digitalRead(this->getPin());
        } else {
            signalState = digitalRead(this->getPin());
        }
        return signalState;
    }
} RelayStruct;

// D0, D1, D10, D13, D50, D51, D52, D53 - do not use
RelayStruct relaySensors[] = {
//  Child ID               description   signalPin(or button) / expanderAddress / hasSignalPin / relay
        // Sensors using Button as signal pin  IN-signal       OUT - relay on expander
        {SALOON_1_ID,      "Salon - Glowne",         22, 8, true,  relayE000},    // 0
        {SALOON_2_ID,      "Salon - Kinkiety(Sofa)", 23, 8, true,  relayE001},    // 1
        {SALOON_3_ID,      "Salon - Kinkiety(TV)",   24, 8, true,  relayE002},    // 2
        {DINING_ROOM_1_ID, "Jadalnia - Glowne",      25, 8, true,  relayE003},    // 3
        {DINING_ROOM_2_ID, "Jadalnia - Kinkiety",    26, 8, true,  relayE004},    // 4
        {BEDROOM_1_ID,     "Sypialnia - Glowne",     27, 8, true,  relayE005},    // 5
        {BEDROOM_2_ID,     "Sypialnia - Kinkiety",   28, 8, true,  relayE006},    // 6
        {ROOM_1_ID,        "Pokoj - Prawy",          29, 8, true,  relayE007},    // 7
        {ROOM_2_ID,        "Pokoj - Lewy",           30, 8, true,  relayE008},    // 8
        {BATHROOM_ID,      "Lazienka",               31, 8, true,  relayE009},    // 9
        {KITCHEN_1_ID,     "Kuchnia - Glowne",       32, 8, true,  relayE010},    // 10
        {KITCHEN_2_ID,     "Kuchnia - Dodatkowe",    33, 8, true,  relayE011},    // 11
        {ENTRANCE_ID,      "Wejscie",                34, 8, true,  relayE012},    // 12
        {LANDING_ID,       "Wiatrolap",              35, 8, true,  relayE013},    // 13
        {HALL_ID,          "Przedpokoj",             36, 8, true,  relayE014},    // 14
        {CORRIDOR_ID,      "Korytarz",               37, 8, true,  relayE015},    // 15
        {UTILITY_ROOM,     "Kotlownia",              38, 8, true,  relayE100},    // 16
        {TERRACE_1_ID,     "Taras - Zachod",         39, 8, true,  relayE101},    // 17
        {TERRACE_2_ID,     "Taras - Poludnie",       40, 8, true,  relayE102},    // 18
        {HOUSE_1_ID,       "Dom - Zachod",           41, 8, true,  relayE103},    // 19
        {HOUSE_2_ID,       "Dom - Wschod",           42, 8, true,  relayE104},    // 20
        {HOUSE_3_ID,       "Dom - Christmass",       43, 8, true,  relayE105},    // 21
        {GARDEN_1_ID,      "Ogrod - Przod",          44, 8, true,  relayE106},    // 22
        {GARDEN_2_ID,      "Ogrod - Tyl",            45, 8, true,  relayE107},    // 23
        {ATTIC_1_ID,       "Strych - Wejscie",       46, 8, true,  relayE108},    // 24
        {ATTIC_2_ID,       "Strych 1",               47, 8, true,  relayE109},    // 25
        {ATTIC_3_ID,       "Strych 2",               48, 8, true,  relayE110},    // 26
        {ATTIC_4_ID,       "Strych 3",               49, 8, true,  relayE111},    // 27
        {FAN_1_ID,         "Wentylator - Lazienka",  2,  8, true,  relayE112},    // 28
        {FAN_2_ID,         "Wentylator - Kotlownia", 3,  8, true,  relayE113},    // 29
        {SPARE_1_ID,       "Spare signal on pin 4",  4,  8, true,  relayE114},    // 30
        {SPARE_2_ID,       "Spare signal on pin 5",  5,  8, true,  relayE115},    // 31
        // Sensors using Button as button      IN-button       OUT - relay on Arduino
        {CHARGER_ID,       "Ladowarka do tabletu",   6,  8, false, relayA08},     // 32
        {TEST_BUTTON,      "Test button on pin 6",   7,  8, false, relayA09},     // 33
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