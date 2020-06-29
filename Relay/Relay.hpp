//
// Created by Przemyslaw Sztandera on 22/05/2020.
//

class Relay {
private:
    uint8_t _pin;
    bool _lowLevelTrigger;
    bool _momentary;
    uint8_t _expanderAddress;

public:
    Relay();

    Relay(uint8_t pin, bool lowLevelTrigger = false, bool momentary = true, uint8_t expanderAddress = 8) {
        _pin = pin;
        _lowLevelTrigger = lowLevelTrigger;
        _momentary = momentary;
        _expanderAddress = expanderAddress;
    }

    uint8_t getPin() {
        return _pin;
    }

    bool isLowLevelTrigger() {
        return _lowLevelTrigger;
    }

    bool isMomentary() {
        return _momentary;
    }

    uint8_t getExpanderAddress() {
        return _expanderAddress;
    }

    bool onExpander() {
        if (_expanderAddress >= 0 && _expanderAddress < 8) {
            return true;
        } else {
            return false;
        }
    }
};
