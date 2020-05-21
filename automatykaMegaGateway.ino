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
#include "./Initialization/InitializationMega.hpp"
// TODO: as this should be fixed, refactor to keep index fixed
// sensor[0] -> msgs[0]
// sensor[1] -> msgs[1]
// etc.

void setup() {
    Serial.begin(115200);
    initializeRelayPins();
    setupClickButtons();
    setupPressButtons();
    Serial.println("Setup() function called");
}

void presentation() {

    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Gateway", "1.5");
    sendPresentation();
}

void loop() {

    readButtons();

}

void receive(const MyMessage &message) {
    Serial.println("Calling receive()");

    // We only expect one type of message from controller. But we better check anyway.
    if (message.type == V_STATUS) {
        auto sensor = message.getSensor();

        Serial.println("Calling setOutput() from receive()");
        switchRelay(sensor);
    }

    Serial.println("End of receive method");
}
