/**
 *
 * @file AutomationMega.hpp
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Automation for buttons & sensors
 * @license GPL V2
 *
 */
#pragma once

bool updateRelayStateAndSendMessage(const uint8_t sensorId);

void switchRelay(const byte sensorId);

void updateEnvironmentSensors();

int freeRam ();

void printTime();

void receiveData();

#include "../Initialization/Initialization.hpp"
#include "../Button/ButtonsInitialization.hpp"
#include "../Timer/Timer.hpp"


bool updateRelayStateAndSendMessage(const uint8_t sensorId) {

    bool pullUpActive = true;
    bool updatedFlag;
    uint8_t oldState = loadState(sensorId);

    Sensor sensor = getSensor(sensorId);
    uint8_t signalState = sensor.readPin();

    Relay relay = getRelay(sensorId);
    uint8_t relayState = relay.readPin();
    uint8_t pinState = -1;

// save state of signal pin to EEPROM
    if (sensor.getPinType() == SIGNAL_PIN) {
        if (pullUpActive) {
            saveState(sensorId, !signalState);
        } else {
            saveState(sensorId, signalState);
        }
        pinState = signalState;
// save state of relay pin to EEPROM
    } else if (sensor.getPinType() == TRIGGER_PIN) {
        if (relay.isLatching()) {
            if (relay.isLowLevelTrigger()) {
                saveState(sensorId, !relayState);
            } else {
                saveState(sensorId, relayState);
            }
        } else {
            saveState(sensorId, oldState);
        }
        pinState = relayState;
    }


    uint8_t newState = loadState(sensorId);
    send(sensorMessages[getIndex(sensorId)].set(newState));

    if (oldState == newState) {
        updatedFlag = true;
        Serial.print(F("updateRelayStateAndSendMessage() called and message send for sensor: "));
        Serial.print(sensorId);
        Serial.print(F(" New sensor state: "));
        Serial.print(newState);
        Serial.print(F(", new pin state: "));
        Serial.println(pinState);
    } else {
        updatedFlag = false;
        Serial.print(F("updateRelayStateAndSendMessage() called and message send for sensor: "));
        Serial.print(sensorId);
        Serial.print(F(" New sensor state: "));
        Serial.print(newState);
        Serial.print(F(", new pin state: "));
        Serial.println(pinState);
    }
    return updatedFlag;
}


void switchRelay(const uint8_t sensorId) {

    Serial.print(F("Calling switchRelay() for sensor: "));
    Serial.println(sensorId);

    Relay relay = getRelay(sensorId);
    uint8_t relayPin = relay.getPin();
    uint8_t relayState = relay.readPin();

// save new sensor state to EEPROM
    uint8_t sensorState = !loadState(sensorId);
    saveState(sensorId, sensorState);

    if (relay.isLatching()) {
        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relayPin, !relayState);
        } else {
            digitalWrite(relayPin, !relayState);
        }
    } else {
        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relayPin, !relayState);
            delay(125);
            expander[relay.getExpanderAddress()].digitalWrite(relayPin, relayState);
        } else {
            digitalWrite(relayPin, !relayState);
            delay(125);
            digitalWrite(relayPin, relayState);
        }

    }

    myDelay(150);

    if (updateRelayStateAndSendMessage(sensorId)) {

        Serial.print(F("switchRelay() called for sensor: "));
        Serial.print(sensorId);
        Serial.print(F(", new sensor state: "));
        Serial.print(loadState(sensorId));
        Serial.println(F(", Switch successful."));
    } else {
        Serial.print(F("switchRelay() called for sensor: "));
        Serial.print(sensorId);
        Serial.print(F(", new sensor state: "));
        Serial.print(loadState(sensorId));
        Serial.println(F(", Switch unsuccessful!!!"));
    }

}

void updateEnvironmentSensors() {

    if (timer(60)) {

        send(sensorMessages[getIndex(ARDUINO_TIMER)].set(millis()));
        send(sensorMessages[getIndex(ARDUINO_TEMP)].set((float)rtc.getTemperature(), 1));

        // printTimeAndTemp();
        printDateAndTime();
 
        if (lightMeter.measurementReady()) {
            float lux = lightMeter.readLightLevel();
            Serial.print(F("Light: "));
            Serial.print(lux);
            Serial.println(F(" lx"));
            send(sensorMessages[getIndex(ARDUINO_LIGHT)].set(lux, 1));
        }

        Serial.print(F("Free RAM: "));
        Serial.print(freeRam());
        Serial.println(F(" bytes"));
        Serial.println();


        Serial.print(F("Modem info: "));
        Serial.println(modem.getModemInfo());
        Serial.print(F("IP: "));
        Serial.println(modem.getLocalIP());
        Serial.print(F("Signal quality: "));
        Serial.print(modem.getSignalQuality());
        Serial.println(F(" dB"));
        Serial.println();
        
    }
}

int freeRam () {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

bool resetMiniPro = true;

void resetArduinoMiniPro(int resetPin) {
    pinMode(resetPin, OUTPUT);
    digitalWrite(resetPin, LOW);
    if (resetMiniPro) {
        // reset pin
        digitalWrite(resetPin, HIGH);
        resetMiniPro = false;
    }
}