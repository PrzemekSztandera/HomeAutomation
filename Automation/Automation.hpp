/**
 *
 * @file AutomationMega.hpp
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Automation for buttons & sensors
 * @license GPL V2
 *
 */

void switchRelay(const byte sensorId);

bool updateRelayStateAndSendMessage(const uint8_t sensorId, bool pullUpActive = true);

void myDelay(long interval);

void updateEnvironmentSensors();

int freeRam ();


#pragma once

#include "../Initialization/Initialization.hpp"
#include "../Button/ButtonsInitialization.hpp"


void myDelay(long interval) {
    unsigned long currentMillis = millis();
    bool flag = true;
    while (flag) {
        if (millis() - currentMillis < interval && millis() - currentMillis >= 0) {
            flag = true;
        } else {
            flag = false;
        }
    }
}

bool updateRelayStateAndSendMessage(const uint8_t sensorId, bool pullUpActive = true) {

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

    myDelay(250);

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
    unsigned long timer = millis() - currentSensorMillis;
    if (timer > 60000 || timer <= 0) {

        send(sensorMessages[getIndex(ARDUINO_TIMER)].set(millis()));
// Bosh sensor BME280
        float temp(NAN), hum(NAN), pres(NAN);
        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_Pa);
        bmeSensor.read(pres, temp, hum, tempUnit, presUnit);
        send(sensorMessages[getIndex(BME_TEMP)].set(temp, 1));
        send(sensorMessages[getIndex(BME_BARO)].set(pres, 0));
        send(sensorMessages[getIndex(BME_HUM)].set(hum, 1));

        Serial.print(F("Free RAM: "));
        Serial.println(freeRam());

        currentSensorMillis = millis();
    }
}

int freeRam () {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


