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
};

// D0, D1, D2, D4, D10, D13 - D21, D49 - D54 : do not use
RelaySensor relaySensors[] = {

        // CHILD_ID / variable / presentation / discovery / description / pin / pin type / expanderAddress / hasSignalPin / relay
        {SIGNAL_IN_11,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 22 in",            22, SIGNAL_PIN,  8},
        {SIGNAL_IN_12,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 23 in",            23, SIGNAL_PIN,  8},
        {SIGNAL_IN_13,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 24 in",            24, SIGNAL_PIN,  8},
        {SIGNAL_IN_14,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 25 in",            25, SIGNAL_PIN,  8},
        {SIGNAL_IN_15,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 26 in",            26, SIGNAL_PIN,  8},
        {SIGNAL_IN_21,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 27 in",            27, SIGNAL_PIN,  8},
        {SIGNAL_IN_22,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 28 in",            28, SIGNAL_PIN,  8},
        {SIGNAL_IN_23,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 29 in",            29, SIGNAL_PIN,  8},
        {SIGNAL_IN_24,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 30 in",            30, SIGNAL_PIN,  8},
        {SIGNAL_IN_31,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 31 in",            31, SIGNAL_PIN,  8},
        {SIGNAL_IN_32,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 32 in",            32, SIGNAL_PIN,  8},
        {SIGNAL_IN_33,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 33 in",            33, SIGNAL_PIN,  8},
        {SIGNAL_IN_34,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 34 in",            34, SIGNAL_PIN,  8},
        {SIGNAL_IN_35,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 35 in",            35, SIGNAL_PIN,  8},
        {SIGNAL_IN_36,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 36 in",            36, SIGNAL_PIN,  8},
        {SIGNAL_IN_37,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 37 in",            37, SIGNAL_PIN,  8},
        {SIGNAL_IN_38,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 38 in",            38, SIGNAL_PIN,  8},
        {SIGNAL_IN_41,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 39 in",            39, SIGNAL_PIN,  8},
        {SIGNAL_IN_42,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 40 in",            40, SIGNAL_PIN,  8},
        {SIGNAL_IN_43,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 41 in",            41, SIGNAL_PIN,  8},
        {SIGNAL_IN_44,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 42 in",            42, SIGNAL_PIN,  8},
        {SIGNAL_IN_45,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 43 in",            43, SIGNAL_PIN,  8},
        {SIGNAL_IN_46,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 44 in",            44, SIGNAL_PIN,  8},
        {SIGNAL_IN_51,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 45 in",            45, SIGNAL_PIN,  8},
        {SIGNAL_IN_52,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 46 in",            46, SIGNAL_PIN,  8},
        {SIGNAL_IN_53,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 47 in",            47, SIGNAL_PIN,  8},
        {SIGNAL_IN_54,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin 48 in",            48, SIGNAL_PIN,  8},
        {SIGNAL_IN_55,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A8 in",            62, SIGNAL_PIN,  8},
        {SIGNAL_IN_56,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A9 in",            63, SIGNAL_PIN,  8},
        {SIGNAL_IN_61,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A10 in",           64, SIGNAL_PIN,  8},
        {SIGNAL_IN_62,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A11 in",           65, SIGNAL_PIN,  8},
        {SIGNAL_IN_71,  V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A12 in",           66, SIGNAL_PIN,  8},
// use only with latching relays
        {SIGNAL_OUT_72, V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A13 in - A15 out", 67, TRIGGER_PIN, 8},
        {SIGNAL_OUT_73, V_STATUS, S_BINARY, HA_DISCOVERY, "Pin A14 in - A3 out",  68, TRIGGER_PIN, 8},
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