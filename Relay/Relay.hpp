//
// Created by Przemyslaw Sztandera on 22/05/2020.
//

class Relay {
private:
    uint8_t _pin;
    bool _lowLevelTrigger;
    uint8_t _expanderAddress;

public:
    Relay();
    Relay(uint8_t pin, bool lowLevelTrigger = false, uint8_t expanderAddress = 8) {
        _pin = pin;
        _lowLevelTrigger = lowLevelTrigger;
        _expanderAddress = expanderAddress;
    }

    uint8_t getPin() {
        return _pin;
    }

    bool isLowLevelTrigger() {
        return _lowLevelTrigger;
    }

    uint8_t getExpanderAddress() {
        return _expanderAddress;
    }

    bool onExpander() {

        if(_expanderAddress < 8 && _expanderAddress >= 0) {
            return true;
        } else {
            return false;
        }
    }
};
