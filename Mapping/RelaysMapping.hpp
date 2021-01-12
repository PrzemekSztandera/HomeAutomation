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

#include "../Relay/Relay.hpp"

// Default relays declaration
// Relay(uint8_t pin, bool lowLevelTrigger = false, bool latching = false, expanderAddress = 8)

// Relays on MCP23017
Relay relayE000(0, true, false, 0);
Relay relayE001(1, true, false, 0);
Relay relayE002(2, true, false, 0);
Relay relayE003(3, true, false, 0);
Relay relayE004(4, true, false, 0);
Relay relayE005(5, true, false, 0);
Relay relayE006(6, true, false, 0);
Relay relayE007(7, true, false, 0);
Relay relayE008(8, true, false, 0);
Relay relayE009(9, true, false, 0);
Relay relayE010(10, true, false, 0);
Relay relayE011(11, true, false, 0);
Relay relayE012(12, true, false, 0);
Relay relayE013(13, true, false, 0);
Relay relayE014(14, true, false, 0);
Relay relayE015(15, true, false, 0);
Relay relayE100(0, true, false, 1);
Relay relayE101(1, true, false, 1);
Relay relayE102(2, true, false, 1);
Relay relayE103(3, true, false, 1);
Relay relayE104(4, true, false, 1);
Relay relayE105(5, true, false, 1);
Relay relayE106(6, true, false, 1);
Relay relayE107(7, true, false, 1);
Relay relayE108(8, true, false, 1);
Relay relayE109(9, true, false, 1);
Relay relayE110(10, true, false, 1);
Relay relayE111(11, true, false, 1);
Relay relayE112(12, true, false, 1);
Relay relayE113(13, true, false, 1);
Relay relayE114(14, true, false, 1);
Relay relayE115(15, true, false, 1);

// Relays on Arduino
Relay relayArduino68(68, true, true, 8);
Relay relayArduino69(69, true, true, 8);


// CHILD_ID declaration of RelaysStruct
const uint8_t SIGNAL_IN_11 = 11;
const uint8_t SIGNAL_IN_12 = 12;
const uint8_t SIGNAL_IN_13 = 13;
const uint8_t SIGNAL_IN_14 = 14;
const uint8_t SIGNAL_IN_15 = 15;
const uint8_t SIGNAL_IN_21 = 21;
const uint8_t SIGNAL_IN_22 = 22;
const uint8_t SIGNAL_IN_23 = 23;
const uint8_t SIGNAL_IN_24 = 24;
const uint8_t SIGNAL_IN_31 = 31;
const uint8_t SIGNAL_IN_32 = 32;
const uint8_t SIGNAL_IN_33 = 33;
const uint8_t SIGNAL_IN_34 = 34;
const uint8_t SIGNAL_IN_35 = 35;
const uint8_t SIGNAL_IN_36 = 36;
const uint8_t SIGNAL_IN_37 = 37;
const uint8_t SIGNAL_IN_38 = 38;
const uint8_t SIGNAL_IN_41 = 41;
const uint8_t SIGNAL_IN_42 = 42;
const uint8_t SIGNAL_IN_43 = 43;
const uint8_t SIGNAL_IN_44 = 44;
const uint8_t SIGNAL_IN_45 = 45;
const uint8_t SIGNAL_IN_46 = 46;
const uint8_t SIGNAL_IN_51 = 51;
const uint8_t SIGNAL_IN_52 = 52;
const uint8_t SIGNAL_IN_53 = 53;
const uint8_t SIGNAL_IN_54 = 54;
const uint8_t SIGNAL_IN_55 = 55;
const uint8_t SIGNAL_IN_56 = 56;
const uint8_t SIGNAL_IN_61 = 61;
const uint8_t SIGNAL_IN_62 = 62;
const uint8_t SIGNAL_IN_71 = 71;
const uint8_t SIGNAL_OUT_72 = 72;
const uint8_t SIGNAL_OUT_73 = 73;


typedef struct {

    const uint8_t id;
    const mysensors_sensor_t presentationType;
    char *description;
    const uint8_t _signalPin; // pin for reading state of Hager(EPN524) relay or physical trigger button
    const uint8_t _expanderAddress;
    bool _hasPin; // true if for reading state of Hager(EPN524) relay
    Relay relay;


    uint8_t getId() {
        return id;
    }

    mysensors_sensor_t getPresentationType() {
        return presentationType;
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

// D0, D1, D2, D4, D10, D13 - D21, D49 - D54 : do not use
RelayStruct relaySensors[] = {

        // CHILD_ID / presentation / description / pin / expanderAddress / hasSignalPin / relay
        {SIGNAL_IN_11, S_BINARY,  "Pin 22 - in",   22, 8, true,  relayE000},    // 0
        {SIGNAL_IN_12, S_BINARY,  "Pin 23 - in",   23, 8, true,  relayE001},    // 1
        {SIGNAL_IN_13, S_BINARY,  "Pin 24 - in",   24, 8, true,  relayE002},    // 2
        {SIGNAL_IN_14, S_BINARY,  "Pin 25 - in",   25, 8, true,  relayE003},    // 3
        {SIGNAL_IN_15, S_BINARY,  "Pin 26 - in",   26, 8, true,  relayE004},    // 4
        {SIGNAL_IN_21, S_BINARY,  "Pin 27 - in",   27, 8, true,  relayE005},    // 5
        {SIGNAL_IN_22, S_BINARY,  "Pin 28 - in",   28, 8, true,  relayE006},    // 6
        {SIGNAL_IN_23, S_BINARY,  "Pin 29 - in",   29, 8, true,  relayE007},    // 7
        {SIGNAL_IN_24, S_BINARY,  "Pin 30 - in",   30, 8, true,  relayE008},    // 8
        {SIGNAL_IN_31, S_BINARY,  "Pin 31 - in",   31, 8, true,  relayE009},    // 9
        {SIGNAL_IN_32, S_BINARY,  "Pin 32 - in",   32, 8, true,  relayE010},    // 10
        {SIGNAL_IN_33, S_BINARY,  "Pin 33 - in",   33, 8, true,  relayE011},    // 11
        {SIGNAL_IN_34, S_BINARY,  "Pin 34 - in",   34, 8, true,  relayE012},    // 12
        {SIGNAL_IN_35, S_BINARY,  "Pin 35 - in",   35, 8, true,  relayE013},    // 13
        {SIGNAL_IN_36, S_BINARY,  "Pin 36 - in",   36, 8, true,  relayE014},    // 14
        {SIGNAL_IN_37, S_BINARY,  "Pin 37 - in",   37, 8, true,  relayE015},    // 15
        {SIGNAL_IN_38, S_BINARY,  "Pin 38 - in",   38, 8, true,  relayE100},    // 16
        {SIGNAL_IN_41, S_BINARY,  "Pin 39 - in",   39, 8, true,  relayE101},    // 17
        {SIGNAL_IN_42, S_BINARY,  "Pin 40 - in",   40, 8, true,  relayE102},    // 18
        {SIGNAL_IN_43, S_BINARY,  "Pin 41 - in",   41, 8, true,  relayE103},    // 19
        {SIGNAL_IN_44, S_BINARY,  "Pin 42 - in",   42, 8, true,  relayE104},    // 20
        {SIGNAL_IN_45, S_BINARY,  "Pin 43 - in",   43, 8, true,  relayE105},    // 21
        {SIGNAL_IN_46, S_BINARY,  "Pin 44 - in",   44, 8, true,  relayE106},    // 22
        {SIGNAL_IN_51, S_BINARY,  "Pin 45 - in",   45, 8, true,  relayE107},    // 23
        {SIGNAL_IN_52, S_BINARY,  "Pin 46 - in",   46, 8, true,  relayE108},    // 24
        {SIGNAL_IN_53, S_BINARY,  "Pin 47 - in",   47, 8, true,  relayE109},    // 25
        {SIGNAL_IN_54, S_BINARY,  "Pin 48 - in",   48, 8, true,  relayE110},    // 26
        {SIGNAL_IN_55, S_BINARY,  "Pin A8 - in",   62, 8, true,  relayE111},    // 27
        {SIGNAL_IN_56, S_BINARY,  "Pin A9 - in",   63, 8, true,  relayE112},    // 28
        {SIGNAL_IN_61, S_BINARY,  "Pin A10 - in",  64, 8, true,  relayE113},    // 29
        {SIGNAL_IN_62, S_BINARY,  "Pin A11 - in",  65, 8, true,  relayE114},    // 30
        {SIGNAL_IN_71, S_BINARY,  "Pin A12 - in",  66, 8, true,  relayE115},    // 31

        {SIGNAL_OUT_72, S_BINARY, "Pin A14 - out, A3 - in",  57,  8, false, relayArduino68}, // 32
        {SIGNAL_OUT_73, S_BINARY, "Pin A15 - out, A13 - in", 67,  8, false, relayArduino69}, // 33
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