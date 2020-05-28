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

// Relay acts as a click button
void clickRelay(int pin) {
    uint8_t state = digitalRead(pin);
    unsigned long currentMillis = millis();
    digitalWrite(pin, !state);
    myDelay(125);
    digitalWrite(pin, state);
}

// Relay acts as a press button
void pressRelay(int pin) {
    uint8_t pinState = digitalRead(pin);
    digitalWrite(pin, !pinState);
}

////----------------------------------------------------------------------------------------------------


void updateRelayStateAndSendMessage(const uint8_t sensorId, bool pullUpActive = true) {
    // Debug
    Serial.print("Calling updateRelayStateAndSendMessage() for sensor: ");
    Serial.print(sensorId);
    // End of Debug

    auto relayStruct = getRelayStruct(sensorId);
    auto relay = getRelay(sensorId);

    uint8_t signalState = digitalRead(relayStruct.getPin());
    uint8_t relayState = digitalRead(relay.getPin());


    if (relayStruct.hasSignalPin()) {
        if (pullUpActive) {
            saveState(sensorId, !signalState);
        } else {
            saveState(sensorId, signalState);
        }
        Serial.print(" and pin: ");
        Serial.println(relayStruct.getPin());
    } else {
        if (relay.isLowLevelTrigger()) {
            relayState = !relayState;
        }
        saveState(sensorId, relayState);

        Serial.print(" and pin: ");
        Serial.println(relay.getPin());
    }
    uint8_t index = getIndex(relayStruct.getId());
    send(msgs[index].set(loadState(sensorId)));

//        // Debug
//        Serial.print("Sensor State after updateRelayStateAndSendMessage(): ");
//        if (loadState(sensorId) == 2) {
//            Serial.println("FLIP");
//        } else if (loadState(sensorId) == 1) {
//            Serial.println("ON");
//        } else if (loadState(sensorId) == 0) {
//            Serial.println("OFF");
//        }
//        // End of Debug
//    }

}

void checkButtonsState() {
    if(millis() - currentMillis2 > 2000) {
        for (uint8_t i = 0; i < maxRelays; i++) {
            auto relayStruct = Relays[i];
            updateRelayStateAndSendMessage(relayStruct.getId());
        }
        currentMillis2 = millis();
        return;
    }
}

////----------------------------------------------------------------------------------------------------

void readButtons() {
    button2.tick();
    button3.tick();
    button4.tick();
    masterButton7.tick();
}

void readSensors() {
    if (millis() - currentMillis > 10000) {
// Bosh sensor BME280
        float temp(NAN), hum(NAN), pres(NAN);
        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_Pa);
        bme.read(pres, temp, hum, tempUnit, presUnit);
        send(sensorMsgs[0].set(temp, 1));
        send(sensorMsgs[1].set(pres, 0));
        send(sensorMsgs[2].set(hum, 1));

// Dallas temp sensor DS18B20
        sensors.requestTemperatures();
        send(sensorMsgs[3].set(sensors.getTempC(sensor1), 1));

        Serial.println("Server update");
        currentMillis = millis();
        return;
    }
}

////----------------------------------------------------------------------------------------------------

void switchRelay(const uint8_t sensorId, const uint8_t &cmd = Toggle::FLIP) {

//    // Debug
//    Serial.print("Calling setOutput() for sensor: ");
//    Serial.print(sensorId);
//    Serial.print(" with toggle: ");
//    if (cmd == 2) {
//        Serial.print("FLIP");
//    } else if (cmd == 1) {
//        Serial.print("ON");
//    } else if (cmd == 0) {
//        Serial.print("OFF");
//    }
//    Serial.print("; Sensor State Before: ");
//    if (loadState(sensorId) == 2) {
//        Serial.println("FLIP");
//    } else if (loadState(sensorId) == 1) {
//        Serial.println("ON");
//    } else if (loadState(sensorId) == 0) {
//        Serial.println("OFF");
//    }
//    // End of Debug


    auto relayStruct = getRelayStruct(sensorId);
    uint8_t index = getIndex(sensorId);
    auto relay = getRelay(sensorId);

    // check whether flip state of sensor
    uint8_t state = (cmd == Toggle::FLIP) ? !loadState(sensorId) : cmd; // OFF -> ON
    saveState(sensorId, state);

    if (cmd == Toggle::FLIP && relayStruct.hasSignalPin()) {
        clickRelay(relay.getPin());

//        // Debug
//        Serial.print("Calling relayAsPushButton(); ");
//        Serial.print("Sensor State: ");
//        if (loadState(sensorId) == 2) {
//            Serial.println("FLIP");
//        } else if (loadState(sensorId) == 1) {
//            Serial.println("ON");
//        } else if (loadState(sensorId) == 0) {
//            Serial.println("OFF");
//        }
//        // End of Debug

    } else if (cmd == Toggle::FLIP && !relayStruct.hasSignalPin()) {
        pressRelay(relay.getPin());

//        // Debug
//        Serial.print("Calling relayAsLatchButton(); ");
//        Serial.print("Sensor State: ");
//        if (loadState(sensorId) == 2) {
//            Serial.println("FLIP");
//        } else if (loadState(sensorId) == 1) {
//            Serial.println("ON");
//        } else if (loadState(sensorId) == 0) {
//            Serial.println("OFF");
//        }
//        // End of Debug
    }

    updateRelayStateAndSendMessage(sensorId);
}

void readAndUpdateState(uint8_t sensorId) {
    updateRelayStateAndSendMessage(sensorId);
//    // Debug
//    auto sensor = getSensor(pSensorId);
//    Serial.print("Setting state for OneButton.tick(): ");
//    Serial.print(sensor.description);
//    Serial.print(", ");
//    Serial.print("Sensor State after Button.tick(): ");
//    if (loadState(sensor.id) == 2) {
//        Serial.println("FLIP");
//    } else if (loadState(sensor.id) == 1) {
//        Serial.println("ON");
//    } else if (loadState(sensor.id) == 0) {
//        Serial.println("OFF");
//    }
//    // End of Debug
}

void flipOutput(uint8_t sensorId) {
    switchRelay(sensorId);
}

void masterClickButton() {
    flipOutput(SALOON_1_ID);
    flipOutput(SALOON_2_ID);
    flipOutput(DINING_ROOM_1_ID);
}

// Setup the buttons and relays. Do not assign LongPress and Click to the same sensor
void setupClickButtons() {

    button2.attachClick(flipOutput, SALOON_1_ID);
    button4.attachClick(flipOutput, DINING_ROOM_1_ID);
    masterButton7.attachClick(masterClickButton);
}

void setupPressButtons() {

    button3.attachLongPressStart(readAndUpdateState, SALOON_2_ID);
    button3.attachLongPressStop(readAndUpdateState, SALOON_2_ID);
}











