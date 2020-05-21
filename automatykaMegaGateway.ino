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
#define MY_DEBUG

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
  Serial.begin(115200);
  
  for(uint8_t i = 0; i < maxSensors; i++) {
    auto sensor = Sensors[i];
    pinMode(sensor.relayPin, OUTPUT);
    msgs[i] = MyMessage(sensor.id, V_STATUS);
    uint8_t currentState = loadState(sensor.id);
        
    // Check whether EEPROM cell was used before
    if (!(currentState == 0 || 1)) {
      currentState = LOW;
      saveState(sensor.id, currentState);
    }

    // inverse state if sensors/relay is Active Low
    bool bState = (sensor.activeLow) ? !currentState : currentState;
    if(sensor.hasSignalPin) {
      bState = (sensor.activeLow) ? HIGH : LOW;
    }
    digitalWrite(sensor.relayPin, bState);
  }
  setupClickButtons();
  setupPressButtons();
      Serial.println("Setup() function called");
}

void presentation() {
  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Gateway", "1.5");

  // Send actual states
  for (uint8_t i = 0; i < maxSensors; i++) {
    auto sensor = Sensors[i];
    present(sensor.id, S_BINARY, sensor.description);
    send(msgs[i].set(loadState(sensor.id)));
    Serial.println("Presentation() function called");

  }
}

void loop() {

  readButtons();
  
}

void receive(const MyMessage &message) {
  Serial.println("Calling receive()");
  
  // We only expect one type of message from controller. But we better check anyway.
  if (message.type==V_STATUS) {
    auto sensor = message.getSensor();
  
    Serial.println("Calling setOutput() from receive()");
    switchRelay(sensor);
  }
  
  Serial.println("End of receive method");
}
