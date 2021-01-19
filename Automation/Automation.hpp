/**
 *
 * @file AutomationMega.hpp
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Automation for buttons & sensors
 * @license GPL V2
 *
 */

void switchRelay(const uint8_t sensorId);

bool updateRelayStateAndSendMessage(const uint8_t sensorId, bool pullUpActive = true);

void myDelay(long interval);

void updateEnvironmentSensors();


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

    bool updated;

    auto relaySensor = getRelaySensor(sensorId);
    Relay relay = relaySensor.getRelay();
    uint8_t signalState = relaySensor.readPin();
    uint8_t relayState = relay.readPin();
    uint8_t oldState = loadState(sensorId);
    uint8_t pinState;

// save state of signal pin to EEPROM
    if (relaySensor.getPinType() == SIGNAL_PIN) {
        if (pullUpActive) {
            saveState(sensorId, !signalState);
        } else {
            saveState(sensorId, signalState);
        }
        pinState = signalState;
// save state of relay pin to EEPROM
    } else if (relaySensor.getPinType() == TRIGGER_PIN) {
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
    send(relaySensorMessages[getIndex(relaySensor.getId())].set(newState));

    if (oldState == newState) {
        updated = true;
        Serial.print("updateRelayStateAndSendMessage() called and message send for sensor: ");
        Serial.print(sensorId);
        Serial.print(" New sensor state: ");
        Serial.print(newState);
        Serial.print(", new pin state: ");
        Serial.println(pinState);
    } else {
        updated = false;
        Serial.print("updateRelayStateAndSendMessage() called and message send for sensor: ");
        Serial.print(sensorId);
        Serial.print(" New sensor state: ");
        Serial.print(newState);
        Serial.print(", new pin state: ");
        Serial.println(pinState);
    }
    return updated;
}


void switchRelay(const uint8_t sensorId) {

    Serial.print("Calling switchRelay() for sensor: ");
    Serial.println(sensorId);

    auto relaySensor = getRelaySensor(sensorId);
    Relay relay = relaySensor.getRelay();
    uint8_t relayState = relay.readPin();

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

    myDelay(250);

    if (updateRelayStateAndSendMessage(sensorId)) {

        Serial.print("switchRelay() called for sensor: ");
        Serial.print(sensorId);
        Serial.print(", new sensor state: ");
        Serial.print(loadState(sensorId));
        Serial.println(", Switch successful.");
    } else {
        Serial.print("switchRelay() called for sensor: ");
        Serial.print(sensorId);
        Serial.print(", new sensor state: ");
        Serial.print(loadState(sensorId));
        Serial.println(", Switch unsuccessful!!!");
    }

}

void updateEnvironmentSensors() {
    unsigned long timer = millis() - currentSensorMillis;
    if (timer > 60000 || timer <= 0) {

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
    }
}


