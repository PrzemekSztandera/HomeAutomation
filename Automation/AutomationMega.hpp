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

uint8_t readRelaySensorPin(RelaySensor relaySensor) {
    return relaySensor.readPin();
}

void updateRelayStateAndSendMessage(const uint8_t sensorId, bool pullUpActive = true) {

    auto relaySensor = getRelaySensor(sensorId);
    auto relay = getRelay(sensorId);

    uint8_t signalState = readRelaySensorPin(relaySensor);
    uint8_t relayState = readRelayPin(relay);
    uint8_t sensorState = loadState(sensorId);

// save state of signal pin to EEPROM
    if (relaySensor.getPinType() == SIGNAL_PIN) {
        if (pullUpActive) {
            saveState(sensorId, !signalState);
        } else {
            saveState(sensorId, signalState);
        }
// save state of relay pin to EEPROM
    } else if (relaySensor.getPinType() == TRIGGER_PIN) {
        if (relay.isLatching()) {
            if (relay.isLowLevelTrigger()) {
                saveState(sensorId, !relayState);
            } else {
                saveState(sensorId, relayState);
            }
        }
    }

    uint8_t index = getIndex(relaySensor.getId());
    uint8_t state = loadState(sensorId);
    send(relaySensorMessages[index].set(state));
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
            delay(125);
            expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), relayState);
        } else {
            digitalWrite(relay.getPin(), !relayState);
            delay(125);
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
void readEnvironmentSensors() {
    unsigned long timer1 = millis() - currentSensorMillis;
    if (timer1 > 60000 || timer1 <= 0) {

        send(environmentSensorMessages[0].set(millis() / 1000));
// Bosh sensor BME280
        float temp(NAN), hum(NAN), pres(NAN);
        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_Pa);
        bme.read(pres, temp, hum, tempUnit, presUnit);
        send(environmentSensorMessages[1].set(temp, 1));
        send(environmentSensorMessages[2].set(pres, 0));
        send(environmentSensorMessages[3].set(hum, 1));


// Dallas temp sensor DS18B20
//        dallasSensors.requestTemperatures();
//        for (uint8_t i = 0; i < numberOfEnvironmentSensors; i++) {
//            auto environmentSensor = environmentSensors[i];
//            if (environmentSensor.getId() == DALLAS_TEMP) {
//                send(sensorMsgs[i].set(dallasSensors.getTempC(sensor1), 1));
//            }
//        }

//        Serial.println("Server update");
        currentSensorMillis = millis();
        myDelay(1000);
    }
}

// Buttons automation
void checkSignalAndRelayState() {
    unsigned long timer = millis() - currentButtonMillis;
    if (timer > 1500 || timer <= 0) {
        for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
            auto relaySensor = relaySensors[i];
            updateRelayStateAndSendMessage(relaySensor.getId());

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
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        buttons[i].tick();
    }
}

void pressButton() {
//    switchRelay(...);
}

void readAndUpdateState(uint8_t sensorId) {
    updateRelayStateAndSendMessage(sensorId);
}

void setupButtons() {
    bool flag = false;
    for (uint8_t i = 0; i < numberOfRelaySensors; i++) {
        auto relay = getRelay(relaySensors[i].getId());

        if (relaySensors[i].getPinType() == TRIGGER_PIN) {
            if (relay.isLatching()) {
                buttons[i].setDebounceTicks(100);
                buttons[i].attachLongPressStart(switchRelay, relaySensors[i].getId());
//                buttons[i].attachLongPressStop(switchRelay, relaySensors[i].getId());
                buttons[i].setPressTicks(275);
            } else {
                Serial.print("Error: Wrong relay type for relay sensor: ");
                Serial.print(relaySensors[i].getId());
                Serial.println(" Only latching relay allowed when sensor pin set to: TRIGGER");
                Serial.println("Refactor code and load again...");
                flag = true;

            }
        } else if (relaySensors[i].getPinType() == SIGNAL_PIN) {
            if(!relay.isLatching()) {
                buttons[i].attachLongPressStart(readAndUpdateState, relaySensors[i].getId());
                buttons[i].attachLongPressStop(readAndUpdateState, relaySensors[i].getId());
            } else {
                Serial.print("Error: Wrong relay type for relay sensor: ");
                Serial.print(relaySensors[i].getId());
                Serial.println(" Only non-latching relay allowed when sensor pin set to: SIGNAL");
                Serial.println("Refactor code and load again...");
                flag = true;
            }
        }
    }
    while (flag) {
        Serial.println("waiting...");
        delay(5000);
    }
}







