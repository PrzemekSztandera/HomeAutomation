//
// Created by Przemyslaw Sztandera on 22/05/2020.
//
#pragma once

#include <Adafruit_MCP23017.h>
#include "../Sensor/Sensor.hpp"

// MCP23017 expander declaration
uint8_t expanderAddresses[] = {0, 1, 2, 3, 4, 5, 6, 7};
const int numberOfExpanders = sizeof(expanderAddresses);
Adafruit_MCP23017 expander[numberOfExpanders];

class Relay {
private:
    uint8_t _pin;
    bool _lowLevelTrigger;
    bool _latching; // false if used with signal pin on RelaySensor
    uint8_t _expanderAddress;
    uint8_t _sensorId;

public:
    Relay(uint8_t pin, bool lowLevelTrigger, bool latching = false, uint8_t expanderAddress = 8, uint8_t sensorId = 0) {
        _pin = pin;
        _lowLevelTrigger = lowLevelTrigger;
        _latching = latching;
        _expanderAddress = expanderAddress;
        _sensorId = sensorId;
    }

    uint8_t getPin() {
        return _pin;
    }

    bool isLowLevelTrigger() {
        return _lowLevelTrigger;
    }

    bool isLatching() {
        return _latching;
    }

    uint8_t getExpanderAddress() {
        return _expanderAddress;
    }

    bool onExpander() {
        return _expanderAddress >= 0 && _expanderAddress < 8 ? true : false;
    }

    uint8_t readPin() {
        uint8_t relayState = 2;
        if (this->onExpander()) {
            relayState = expander[this->getExpanderAddress()].digitalRead(this->getPin());
        } else {
            relayState = digitalRead(this->getPin());
        }
        return relayState;
    }

    uint8_t getSensorId() {
        return _sensorId;
    }

    void initialize() {
        if (this->onExpander()) {
            expander[this->getExpanderAddress()].pinMode(this->getPin(), OUTPUT);
        } else {
            pinMode(this->getPin(), OUTPUT);
        }
    }

};



// Default relays declaration
// Relay(uint8_t pin, bool lowLevelTrigger = false, bool latching = false, expanderAddress = 8)
// Relays on MCP23017
Relay relays[] = {
        {0,  true, false, 0, SIGNAL_IN_11},
        {1,  true, false, 0, SIGNAL_IN_12},
        {2,  true, false, 0, SIGNAL_IN_13},
        {3,  true, false, 0, SIGNAL_IN_14},
        {4,  true, false, 0, SIGNAL_IN_15},
        {5,  true, false, 0, SIGNAL_IN_21},
        {6,  true, false, 0, SIGNAL_IN_22},
        {7,  true, false, 0, SIGNAL_IN_23},
        {8,  true, false, 0, SIGNAL_IN_24},
        {9,  true, false, 0, SIGNAL_IN_31},
        {10, true, false, 0, SIGNAL_IN_32},
        {11, true, false, 0, SIGNAL_IN_33},
        {12, true, false, 0, SIGNAL_IN_34},
        {13, true, false, 0, SIGNAL_IN_35},
        {14, true, false, 0, SIGNAL_IN_36},
        {15, true, false, 0, SIGNAL_IN_37},
        {0,  true, false, 1, SIGNAL_IN_38},
        {1,  true, false, 1, SIGNAL_IN_41},
        {2,  true, false, 1, SIGNAL_IN_42},
        {3,  true, false, 1, SIGNAL_IN_43},
        {4,  true, false, 1, SIGNAL_IN_44},
        {5,  true, false, 1, SIGNAL_IN_45},
        {6,  true, false, 1, SIGNAL_IN_46},
        {7,  true, false, 1, SIGNAL_IN_51},
        {8,  true, false, 1, SIGNAL_IN_52},
        {9,  true, false, 1, SIGNAL_IN_53},
        {10, true, false, 1, SIGNAL_IN_54},
        {11, true, false, 1, SIGNAL_IN_55},
        {12, true, false, 1, SIGNAL_IN_56},
        {13, true, false, 1, SIGNAL_IN_61},
        {14, true, false, 1, SIGNAL_IN_62},
        {15, true, false, 1, SIGNAL_IN_71},
        {57, true, true,  8, SIGNAL_IN_72}, // A3
//        {69, true, true,  8, SIGNAL_OUT_73}, // A15
};

const uint8_t numberOfRelays = sizeof(relays) / sizeof(Relay);

//    const uint8_t sensorId = static_cast<uint8_t>(reinterpret_cast<intptr_t>(pSensorId));

uint8_t getRelayIndex(uint8_t sensorId) {
    for (uint8_t i = 0; i < numberOfRelays; i++) {
        if (relays[i].getSensorId() == sensorId) return (i);
    }
    return (-1);
}

Relay getRelay(uint8_t sensorId) {
    uint8_t index = getRelayIndex(sensorId);
    return relays[index];
}