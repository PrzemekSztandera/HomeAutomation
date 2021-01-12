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

#include "../Initialization/InitializationMega.hpp"

// Relays automation

void myDelay(long interval) {
    unsigned long currentMillis = millis();
    while (millis() - currentMillis < interval) {}
}

uint8_t readRelayPin(Relay relay) {
    return relay.readPin();
}

uint8_t readSignalPin(RelayStruct relayStruct) {
    return relayStruct.readPin();
}

void updateRelayStateAndSendMessage(const uint8_t sensorId, bool pullUpActive = true) {

    auto relayStruct = getRelayStruct(sensorId);
    auto relay = getRelay(sensorId);

    uint8_t signalState = readSignalPin(relayStruct);
    uint8_t relayState = readRelayPin(relay);
    uint8_t sensorState = loadState(sensorId);

// save state of signal pin to EEPROM
    if (relayStruct.hasSignalPin()) {
        if (pullUpActive) {
            saveState(sensorId, !signalState);
        } else {
            saveState(sensorId, signalState);
        }
// save state of relay pin to EEPROM
    } else {
        if (relay.isLatching()) {
            if (relay.isLowLevelTrigger()) {
                saveState(sensorId, !relayState);
            } else {
                saveState(sensorId, relayState);
            }
        }
    }

    uint8_t index = getIndex(relayStruct.getId());
    uint8_t state = loadState(sensorId);
    send(msgs[index].set(state));
    if(sensorState != state) {
        Serial.print("Message send and new state updated: ");
        Serial.print(state);
        Serial.print(" for sensor: ");
        Serial.println(sensorId);
    }
}


void switchRelay(const uint8_t sensorId) {

    Serial.print("Calling switchRelay() for sensor: ");
    Serial.println(sensorId);

    auto relay = getRelay(sensorId);
    uint8_t relayState = readRelayPin(relay);

// save new sensor state to EEPROM
    uint8_t sensorState = !loadState(sensorId);
    saveState(sensorId, sensorState);

    if (relay.isLatching()) {
        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), !relayState);
        } else {
            digitalWrite(relay.getPin(), !relayState);
        }
    } else {
        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), !relayState);
            myDelay(125);
            expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), relayState);
        } else {
            digitalWrite(relay.getPin(), !relayState);
            myDelay(125);
            digitalWrite(relay.getPin(), relayState);
        }

    }

    updateRelayStateAndSendMessage(sensorId);
    Serial.print("updateRelayStateAndSendMessage() and ");
    Serial.print("switchRelay() called for sensor: ");
    Serial.print(sensorId);
    Serial.print(", new state: ");
    Serial.println(loadState(sensorId));
}

// Sensors automation
void readSensors() {
    if (millis() - currentSensorMillis > 300000) {
// Bosh sensor BME280
//        float temp(NAN), hum(NAN), pres(NAN);
//        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
//        BME280::PresUnit presUnit(BME280::PresUnit_Pa);
//        bme.read(pres, temp, hum, tempUnit, presUnit);
//        send(sensorMsgs[0].set(temp, 1));
//        send(sensorMsgs[1].set(pres, 0));
//        send(sensorMsgs[2].set(hum, 1));

// Dallas temp sensor DS18B20
        dallasSensors.requestTemperatures();
        for (uint8_t i = 0; i < maxSensors; i++) {
            auto sensorStruct = environmentSensors[i];
            if (sensorStruct.getId() == DALLAS_TEMP) {
                send(sensorMsgs[i].set(dallasSensors.getTempC(sensor1), 1));
            }
        }

//        Serial.println("Server update");
        currentSensorMillis = millis();
        return;
    }
}

// Buttons automation
void checkSignalAndRelayState() {
    if (millis() - currentButtonMillis > 1500) {
        for (uint8_t i = 0; i < numberOfRelayStruct; i++) {
            auto relayStruct = relaySensors[i];
            updateRelayStateAndSendMessage(relayStruct.getId());

//            Serial.print("Current sensor: ");
//            Serial.println(relayStruct.id);
//            Serial.print("Current sensor state: ");
//            Serial.println(loadState(relayStruct.getId()));
//            auto relay = getRelay(relayStruct.getId());
//            Serial.print("Current pin state: ");
//            Serial.println(digitalRead(relay.getPin()));

        }
        currentButtonMillis = millis();
        return;
    }
}

void readButtons() {
    for (uint8_t i = 0; i < numberOfRelayStruct; i++) {
        buttons[i].tick();
    }
}

void pressButton() {
//    switchRelay(...);
}

// Setup the buttons and relays. Do not assign LongPress and Click to the same sensor
void setupClickButtons() {
    for (uint8_t i = 0; i < numberOfButtons; i++) {
        if (!relaySensors[i].hasSignalPin()) {
            buttons[i].attachLongPressStart(switchRelay, relaySensors[i].getId());
//            buttons[i].attachLongPressStart(pressButton);
            buttons[i].setPressTicks(275);
        }
    }
}

void readAndUpdateState(uint8_t sensorId) {
    updateRelayStateAndSendMessage(sensorId);
}

void setupSignalButtons() {
    for (uint8_t i = 0; i < numberOfButtons; i++) {
        if (relaySensors[i].hasSignalPin()) {
            buttons[i].attachLongPressStart(readAndUpdateState, relaySensors[i].getId());
            buttons[i].attachLongPressStop(readAndUpdateState, relaySensors[i].getId());
        }
    }
}











