/**
 *
 * @file automatykaMegaGateway.ino
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Implementation of the MySensors Gateway.
 * @license GPL V2
 *
 */

// Enable serial gateway
#define MY_GATEWAY_SERIAL

// Define a lower baud rate for Arduino's running on 8 MHz (Arduino Pro Mini 3.3V & SenseBender)
#if F_CPU == 16000000L
#define MY_BAUD_RATE 115200
#endif

// Remember to add library to Arduino path
#include <MySensors.h>
#include "./Mapping/MappingMega.hpp"
#include "./Automation/AutomationMega.hpp"

// TODO: as this should be fixed, refactor to keep index fixed
// sensor[0] -> msgs[0]
// sensor[1] -> msgs[1]
// etc.
void setup() {
  for(uint8_t i = 0; i < maxSensors; i++) {
    auto sensor = Sensors[i];
    pinMode(sensor.pin, OUTPUT);
    msgs[i] = MyMessage(sensor.id, V_STATUS);
    uint8_t currentState = loadState(sensor.id);

    // Check whether EEPROM cell was used before
    if (!(currentState == 0 || 1)) {
      currentState = Toggle::OFF;
      saveState(sensor.id, currentState);
    }

    // inverse state if sensors/relay is Active Low
    bool bState = (sensor.activelow) ? Toggle::ON : Toggle::OFF;
    digitalWrite(sensor.pin, bState);
  }
  setupButtons();
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Gateway", "1.5");

  // Send actual states
  for (uint8_t i = 0; i < maxSensors; i++) {
    auto sensor = Sensors[i];
    present(sensor.id, S_BINARY, sensor.description);
    send(msgs[i].set(loadState(sensor.id)));
  }
}

void loop() {
//   Keep sensing buttons
  saloonMain.tick();
  saloonSide.tick();
  diningRoomMain.tick();
//  diningRoomSide.tick();
//  bedroomMain.tick();
//  bedroomSide.tick();
//  kid1Main.tick();
//  kid2Main.tick();
//  bathroom1.tick();
//  bathroom2.tick();
//  bathroom1Fan.tick();
//  bathroom2Fan.tick();
//  kitchenMain.tick();
//  kitchenSide.tick();
//  entrance.tick();
//  landing.tick();
//  corridor.tick();
//  hall.tick();
//  terraceMain.tick();
//  terraceBack.tick();
//  outside.tick();
//  garden.tick();
//  gate.tick();
//  attic1.tick();
//  attic2.tick();
}

void receive(const MyMessage &message) {
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type==V_STATUS) {

    // Store state in eeprom and send message
    flipSwitch(message.getSensor());
  }
}
