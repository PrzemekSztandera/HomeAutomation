/**
 *
 * @file AutomationMega.hpp
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Automation for buttons & sensors.
 * @license GPL V2
 *
 */

#pragma once

#include "../Initialization/InitializationMega.hpp"

void myDelay(long interval) {
    unsigned long currentMillis = millis();
    while (millis() - currentMillis < interval) {}
}

uint8_t readRelayPin(Relay relay) {
//    uint8_t relayState = 2;
//    if (relay.onExpander()) {
//        relayState = expander[relay.getExpanderAddress()].digitalRead(relay.getPin());
//    } else {
//        relayState = digitalRead(relay.getPin());
//    }
//    return relayState;
    return relay.readPin();
}

uint8_t readSignalPin(RelayStruct relayStruct) {
//    uint8_t signalState = 2;
//    if (relayStruct.onExpander()) {
//        signalState = expander[relayStruct.getExpanderAddress()].digitalRead(relayStruct.getPin());
//    } else {
//        signalState = digitalRead(relayStruct.getPin());
//    }
//    return signalState;
    return relayStruct.readPin();
}

// Relay acts as a click button
void clickRelay(Relay relay) {
    uint8_t state = readRelayPin(relay);
    unsigned long currentMillis = millis();
    if (relay.onExpander()) {
        expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), !state);
        myDelay(125);
        expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), state);
    } else {
        digitalWrite(relay.getPin(), !state);
        myDelay(125);
        digitalWrite(relay.getPin(), state);
    }
}

// Relay acts as a press button
void pressRelay(Relay relay) {
    uint8_t state = readRelayPin(relay);
    if (relay.onExpander()) {
        expander[relay.getExpanderAddress()].digitalWrite(relay.getPin(), !state);
    } else {
        digitalWrite(relay.getPin(), !state);
    }
}

////----------------------------------------------------------------------------------------------------


void updateRelayStateAndSendMessage(const uint8_t sensorId, bool pullUpActive = true) {
    // Debug
//    Serial.print("Calling updateRelayStateAndSendMessage() for sensor: ");
//    Serial.print(sensorId);
    // End of Debug

    auto relayStruct = getRelayStruct(sensorId);
    auto relay = getRelay(sensorId);

    uint8_t signalState = readSignalPin(relayStruct);
    uint8_t relayState = readRelayPin(relay);


    if (relayStruct.hasSignalPin()) {
        if (pullUpActive) {
            saveState(sensorId, !signalState);
        } else {
            saveState(sensorId, signalState);
        }
//        Serial.print(" and pin: ");
//        Serial.println(relayStruct.getPin());
    } else {
        if (relay.isLowLevelTrigger()) {
            relayState = !relayState;
        }
        saveState(sensorId, relayState);

//        Serial.print(" and pin: ");
//        Serial.println(relay.getPin());
    }

    uint8_t index = getIndex(relayStruct.getId());
    send(msgs[index].set(loadState(sensorId)));
}

void checkButtonsState() {
    if (millis() - currentMillis2 > 2000) {
        for (uint8_t i = 0; i < numberOfRelayStruct; i++) {
            auto relayStruct = relaySensors[i];
            updateRelayStateAndSendMessage(relayStruct.getId());
        }
        currentMillis2 = millis();
        return;
    }
}

////----------------------------------------------------------------------------------------------------

void readButtons() {
    for (uint8_t i = 0; i < sizeof(buttons) / sizeof(OneButton); i++) {
        buttons[i].tick();
    }
}

void readSensors() {
    if (millis() - currentMillis > 10000) {
// Bosh sensor BME280
//        float temp(NAN), hum(NAN), pres(NAN);
//        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
//        BME280::PresUnit presUnit(BME280::PresUnit_Pa);
//        bme.read(pres, temp, hum, tempUnit, presUnit);
//        send(sensorMsgs[0].set(temp, 1));
//        send(sensorMsgs[1].set(pres, 0));
//        send(sensorMsgs[2].set(hum, 1));

// Dallas temp sensor DS18B20
        sensors.requestTemperatures();
        for (uint8_t i = 0; i < maxSensors; i++) {
            auto sensorStruct = environmentSensors[i];
            if (sensorStruct.getId() == SALOON_DALLAS_TEMP) {
                send(sensorMsgs[i].set(sensors.getTempC(sensor1), 1));
            }
        }

//        Serial.println("Server update");
        currentMillis = millis();
        return;
    }
}

////----------------------------------------------------------------------------------------------------

void switchRelay(const uint8_t sensorId) {

    auto relay = getRelay(sensorId);

    uint8_t state = !loadState(sensorId);
    saveState(sensorId, state);

    if (relay.isMomentary()) {
        clickRelay(relay);
    } else {
        pressRelay(relay);
    }
    updateRelayStateAndSendMessage(sensorId);
}

void readAndUpdateState(uint8_t sensorId) {
    updateRelayStateAndSendMessage(sensorId);
}

void masterClickButton() {
    switchRelay(SALOON_1_ID);
    switchRelay(SALOON_2_ID);
    switchRelay(DINING_ROOM_1_ID);
}

// Setup the buttons and relays. Do not assign LongPress and Click to the same sensor
void setupClickButtons() {

    for (uint8_t i = 0; i < numberOfButtons; i++) {
        if (!relaySensors[i].hasSignalPin()) {
            buttons[i].attachClick(switchRelay, relaySensors[i].getId());
        }
    }

//    masterButton7.attachClick(masterClickButton);
}

void setupPressButtons() {

    for (uint8_t i = 0; i < numberOfButtons; i++) {
        if (relaySensors[i].hasSignalPin()) {
            buttons[i].attachLongPressStart(readAndUpdateState, relaySensors[i].getId());
            buttons[i].attachLongPressStop(readAndUpdateState, relaySensors[i].getId());
        }
    }
}











