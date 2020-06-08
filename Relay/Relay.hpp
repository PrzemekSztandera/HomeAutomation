//
// Created by Przemyslaw Sztandera on 22/05/2020.
//

class Relay {
private:
    uint8_t _pin;
    bool _lowLevelTrigger;
    bool _mcpPin;
public:
    Relay();
    Relay(uint8_t pin, bool lowLevelTrigger = false, bool mcpPin = false) {
        _pin = pin;
        _lowLevelTrigger = lowLevelTrigger;
        _mcpPin = mcpPin;
    }

    uint8_t getPin() {
        return _pin;
    }

    bool isLowLevelTrigger() {
        return _lowLevelTrigger;
    }

    bool isMcpPin() {
        return _mcpPin;
    }
};
