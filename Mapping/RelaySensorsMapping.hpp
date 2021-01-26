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
// Reset - don't change
Relay relayArduino57(57, true, false, 8);

// Relays on Arduino
Relay relayArduino69(69, true, true, 8);


// CHILD_ID declaration of RelaysStruct
#define SIGNAL_OUT_3 (uint8_t) 3
#define SIGNAL_IN_11 (uint8_t) 11
#define SIGNAL_IN_12 (uint8_t) 12
#define SIGNAL_IN_13 (uint8_t) 13
#define SIGNAL_IN_14 (uint8_t) 14
#define SIGNAL_IN_15 (uint8_t) 15
#define SIGNAL_IN_21 (uint8_t) 21
#define SIGNAL_IN_22 (uint8_t) 22
#define SIGNAL_IN_23 (uint8_t) 23
#define SIGNAL_IN_24 (uint8_t) 24
#define SIGNAL_IN_31 (uint8_t) 31
#define SIGNAL_IN_32 (uint8_t) 32
#define SIGNAL_IN_33 (uint8_t) 33
#define SIGNAL_IN_34 (uint8_t) 34
#define SIGNAL_IN_35 (uint8_t) 35
#define SIGNAL_IN_36 (uint8_t) 36
#define SIGNAL_IN_37 (uint8_t) 37
#define SIGNAL_IN_38 (uint8_t) 38
#define SIGNAL_IN_41 (uint8_t) 41
#define SIGNAL_IN_42 (uint8_t) 42
#define SIGNAL_IN_43 (uint8_t) 43
#define SIGNAL_IN_44 (uint8_t) 44
#define SIGNAL_IN_45 (uint8_t) 45
#define SIGNAL_IN_46 (uint8_t) 46
#define SIGNAL_IN_51 (uint8_t) 51
#define SIGNAL_IN_52 (uint8_t) 52
#define SIGNAL_IN_53 (uint8_t) 53
#define SIGNAL_IN_54 (uint8_t) 54
#define SIGNAL_IN_55 (uint8_t) 55
#define SIGNAL_IN_56 (uint8_t) 56
#define SIGNAL_IN_61 (uint8_t) 61
#define SIGNAL_IN_62 (uint8_t) 62
#define SIGNAL_IN_71 (uint8_t) 71
#define SIGNAL_OUT_72 (uint8_t) 72


// RelaySensor pin type
#define SIGNAL_PIN (uint8_t) 1
#define TRIGGER_PIN (uint8_t) 2


struct RelaySensor {

    uint8_t id;
    mysensors_data_t variableType;
    mysensors_sensor_t presentationType;
    uint8_t discoveryType;
    char *description;
    uint8_t _pin;
    uint8_t _pinType;
    uint8_t _expanderAddress;
    Relay relay;

    uint8_t getId() {
        return id;
    }

    uint8_t getVariableType() {
        return variableType;
    }

    uint8_t getPresentationType() {
        return presentationType;
    }

    uint8_t getDiscoveryType() {
        return discoveryType;
    }

    char *getDescription() {
        return description;
    }

    uint8_t getPin() {
        return _pin;
    }

    uint8_t getPinType() {
        return _pinType;
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

    Relay getRelay() {
        return relay;
    }
};

// D0, D1, D2, D4, D10, D13 - D21, D49 - D54 : do not use
RelaySensor relaySensors[] = {

        // CHILD_ID / variable / presentation / discovery / description / pin / pin type / expanderAddress / hasSignalPin / relay
        {SIGNAL_OUT_3,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A14 in - A3 out",  68, TRIGGER_PIN, 8, relayArduino57},
        {SIGNAL_IN_11,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 22 in",            22, SIGNAL_PIN,  8, relayE000},
        {SIGNAL_IN_12,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 23 in",            23, SIGNAL_PIN,  8, relayE001},
        {SIGNAL_IN_13,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 24 in",            24, SIGNAL_PIN,  8, relayE002},
        {SIGNAL_IN_14,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 25 in",            25, SIGNAL_PIN,  8, relayE003},
        {SIGNAL_IN_15,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 26 in",            26, SIGNAL_PIN,  8, relayE004},
        {SIGNAL_IN_21,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 27 in",            27, SIGNAL_PIN,  8, relayE005},
        {SIGNAL_IN_22,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 28 in",            28, SIGNAL_PIN,  8, relayE006},
        {SIGNAL_IN_23,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 29 in",            29, SIGNAL_PIN,  8, relayE007},
        {SIGNAL_IN_24,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 30 in",            30, SIGNAL_PIN,  8, relayE008},
        {SIGNAL_IN_31,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 31 in",            31, SIGNAL_PIN,  8, relayE009},
        {SIGNAL_IN_32,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 32 in",            32, SIGNAL_PIN,  8, relayE010},
        {SIGNAL_IN_33,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 33 in",            33, SIGNAL_PIN,  8, relayE011},
        {SIGNAL_IN_34,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 34 in",            34, SIGNAL_PIN,  8, relayE012},
        {SIGNAL_IN_35,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 35 in",            35, SIGNAL_PIN,  8, relayE013},
        {SIGNAL_IN_36,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 36 in",            36, SIGNAL_PIN,  8, relayE014},
        {SIGNAL_IN_37,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 37 in",            37, SIGNAL_PIN,  8, relayE015},
        {SIGNAL_IN_38,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 38 in",            38, SIGNAL_PIN,  8, relayE100},
        {SIGNAL_IN_41,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 39 in",            39, SIGNAL_PIN,  8, relayE101},
        {SIGNAL_IN_42,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 40 in",            40, SIGNAL_PIN,  8, relayE102},
        {SIGNAL_IN_43,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 41 in",            41, SIGNAL_PIN,  8, relayE103},
        {SIGNAL_IN_44,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 42 in",            42, SIGNAL_PIN,  8, relayE104},
        {SIGNAL_IN_45,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 43 in",            43, SIGNAL_PIN,  8, relayE105},
        {SIGNAL_IN_46,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 44 in",            44, SIGNAL_PIN,  8, relayE106},
        {SIGNAL_IN_51,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 45 in",            45, SIGNAL_PIN,  8, relayE107},
        {SIGNAL_IN_52,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 46 in",            46, SIGNAL_PIN,  8, relayE108},
        {SIGNAL_IN_53,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 47 in",            47, SIGNAL_PIN,  8, relayE109},
        {SIGNAL_IN_54,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 48 in",            48, SIGNAL_PIN,  8, relayE110},
        {SIGNAL_IN_55,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A8 in",            62, SIGNAL_PIN,  8, relayE111},
        {SIGNAL_IN_56,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A9 in",            63, SIGNAL_PIN,  8, relayE112},
        {SIGNAL_IN_61,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A10 in",           64, SIGNAL_PIN,  8, relayE113},
        {SIGNAL_IN_62,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A11 in",           65, SIGNAL_PIN,  8, relayE114},
        {SIGNAL_IN_71,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A12 in",           66, SIGNAL_PIN,  8, relayE115},
// use only with latching relays
        {SIGNAL_OUT_72, V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A13 in - A15 out", 67, TRIGGER_PIN, 8, relayArduino69},
};

const uint8_t numberOfRelaySensors = sizeof(relaySensors) / sizeof(RelaySensor);

//    const uint8_t sensorId = static_cast<uint8_t>(reinterpret_cast<intptr_t>(pSensorId));

uint8_t getIndex(uint8_t sensorId) {
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        if (relaySensors[i].getId() == sensorId) return (i);
    }
    return (-1);
}

RelaySensor getRelaySensor(uint8_t sensorId) {
    uint8_t index = getIndex(sensorId);
    return relaySensors[index];
}