//
// Created by Przemyslaw Sztandera on 22/05/2020.
//
#include <Adafruit_MCP23017.h>

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

public:
    Relay();

    Relay(uint8_t pin, bool lowLevelTrigger, bool latching = false, uint8_t expanderAddress = 8) {
        _pin = pin;
        _lowLevelTrigger = lowLevelTrigger;
        _latching = latching;
        _expanderAddress = expanderAddress;
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

    void initialize() {
        if (this->onExpander()) {
            expander[this->getExpanderAddress()].pinMode(this->getPin(), OUTPUT);
        } else {
            pinMode(this->getPin(), OUTPUT);
        }
    }
};
