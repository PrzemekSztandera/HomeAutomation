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
  uint8_t index = getIndex(sensorId);
  auto sensor = Sensors[index];

  // check whether flip state of sensor
  uint8_t state = (cmd == Toggle::FLIP) ? !loadState(sensor.id) : cmd;
//  saveState(sensor.id, state);

    if(cmd == Toggle::FLIP && actAsPushButton) {
        
        relayAsPushButton(sensor.pin, sensor.activelow);
        send(msgs[index].set(!state));
    } else if (cmd == Toggle::FLIP && !actAsPushButton) {
        relayAsLatchButton(sensor.pin, sensor.activelow);
        send(msgs[index].set(state));

    } else {
        saveState(sensor.id, state);
        send(msgs[index].set(state));
    }
//    const uint8_t buttonsSize = sizeof(getButtons(sensor.id)) / sizeof(OneButton);
//    for (uint8_t i = 0; i < buttonsSize; i++) {
//        OneButton buttons [buttonsSize] = getButtons(sensor.id);
//        if(!buttons[i].isLongPressed()) {
//            state = !loadState(sensor.id);
//            saveState(sensor.id, state);
//            send(msgs[index].set(state));
//        }
//    }
}

void setStateON(void* pSensorId) {
  auto sensor = getSensor(pSensorId);
    setOutput(sensor.id, Toggle::ON);
}

void setStateOFF(void* pSensorId) {
  auto sensor = getSensor(pSensorId);
    setOutput(sensor.id, Toggle::OFF);
}

void flipSwitch(void* pSensorId) {
  auto sensor = getSensor(pSensorId);
  setOutput(sensor.id);
}

void latchSwitch(void* pSensorId) {
  auto sensor = getSensor(pSensorId);
  setOutput(sensor.id, Toggle::FLIP, false);
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












