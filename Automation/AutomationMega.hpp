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

#include "../Mapping/MappingMega.hpp"

void myDelay(long interval) {
    unsigned long currentMillis = millis();
    while(millis() - currentMillis < interval){}
}

// relay acts as a click button
void relayAsPushButton(int pin, bool activeLow) {
  uint8_t state = digitalRead(pin);
  unsigned long currentMillis = millis();
  digitalWrite(pin, !state);
  myDelay(125);
  digitalWrite(pin, state);
}

// relay acts as a latch button
void relayAsLatchButton(int pin, bool activeLow) {
  uint8_t pinState = digitalRead(pin);
  digitalWrite(pin, !pinState);
}



void setOutput(const uint8_t& sensorId, const uint8_t& cmd = Toggle::FLIP, bool actAsPushButton = true) {

// Sensors[index]
//  uint8_t index = getIndex(sensorId);
//  auto sensor = Sensors[index];

  auto sensor = getSensor(sensorId);
  uint8_t index = getIndex(sensor.id);

  // check whether flip state of sensor
  uint8_t state = (cmd == Toggle::FLIP) ? !loadState(sensor.id) : cmd; // OFF -> ON
//  saveState(sensor.id, state); // ON

    if(cmd == Toggle::FLIP && actAsPushButton) {
        relayAsPushButton(sensor.pin, sensor.activelow);
        state = !loadState(sensor.id); // ON -> OFF / OFF -> ON
    } else if (cmd == Toggle::FLIP && !actAsPushButton) {
        relayAsLatchButton(sensor.pin, sensor.activelow);
    }
//    else {
//        saveState(sensor.id, state);
//        send(msgs[index].set(state));
//    }

    saveState(sensor.id, state);
    send(msgs[index].set(state)); // OFF / ON
}

void checkRelayState(const uint8_t& sensorId, bool actAsPushButton = true) {
//    const uint8_t sensorId = static_cast<uint8_t>(reinterpret_cast<intptr_t>(pSensorId));
//    uint8_t index = getIndex(sensorId);
//    auto sensor = Sensors[index];

    auto sensor = getSensor(sensorId);
    uint8_t index = getIndex(sensor.id);

//    uint8_t state = loadState(sensor.id);
    uint8_t state = loadState(sensorId);
//    const uint8_t buttonsSize = sizeof(getButtons(sensor.id)) / sizeof(OneButton);
    const uint8_t buttonsSize = sizeof(getButtons(sensorId)) / sizeof(OneButton);
    if(actAsPushButton) {
        for (uint8_t i = 0; i < buttonsSize; i++) {
//            OneButton buttons [buttonsSize] = getButtons(sensor.id);
            OneButton buttons [buttonsSize] = getButtons(sensorId);
            if(!buttons[i].isLongPressed()) {
//                saveState(sensor.id, !state);
                saveState(sensorId, !state);
                send(msgs[index].set(!state));
            }
        }
    } else {
        if((sensor.activelow && digitalRead(sensor.pin) == HIGH) || (!sensor.activelow && digitalRead(sensor.pin) == LOW)) {
//            saveState(sensor.id, !state);
            saveState(sensorId, !state);
            send(msgs[index].set(!state));
        }
    }
}

//void setState(const uint8_t& sensorId, const uint8_t& cmd = Toggle::OFF, bool actAsPushButton = true) {
//    // Sensors[index]
//    uint8_t index = getIndex(sensorId);
//    auto sensor = Sensors[index];
//    // check whether flip state of sensor
//    uint8_t state = (cmd == Toggle::FLIP) ? !loadState(sensor.id) : cmd;
//
//    saveState(sensor.id, state);
//    send(msgs[index].set(state));
//}

void setStateON(void* pSensorId) {
//  auto sensor = getSensor(pSensorId);
//    setOutput(sensor.id, Toggle::ON);
    setOutput(pSensorId, Toggle::ON);
}

void setStateOFF(void* pSensorId) {
//  auto sensor = getSensor(pSensorId);
//    setOutput(sensor.id, Toggle::OFF);
    setOutput(pSensorId, Toggle::OFF);
}

void flipSwitch(void* pSensorId) {
//  auto sensor = getSensor(pSensorId);
//  setOutput(sensor.id);
  setOutput(pSensorId);
}

void latchSwitch(void* pSensorId) {
//  auto sensor = getSensor(pSensorId);
//  setOutput(sensor.id, Toggle::FLIP, false);
  setOutput(pSensorId, Toggle::FLIP, false);
}

void setupButtons() {
  // Setup the button.
  saloonMain.attachLongPressStart(setStateON, SALOON_1_ID);
  saloonMain.attachLongPressStop(setStateOFF, SALOON_1_ID);
  saloonMain.attachClick(flipSwitch, SALOON_1_ID);
    
  saloonSide.attachLongPressStart(setStateON, SALOON_2_ID);
  saloonSide.attachLongPressStop(setStateOFF, SALOON_2_ID);
    
  diningRoomMain.attachLongPressStart(setStateON, DINING_ROOM_1_ID);
  diningRoomMain.attachLongPressStop(setStateOFF, DINING_ROOM_1_ID);
    
}












