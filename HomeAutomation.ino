#include <OneButton.h>

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
#include "./Automation/AutomationMega.hpp"
#include "./Initialization/InitializationMega.hpp"
#include "./I2C/I2C_scanner.hpp"

void before() {
  Serial.begin(115200);
    scanI2cDevices();
    createButtons();
    initializeMCP23017();
    initializeRelays();
    initializeSensors();
    initializeTimers();
}

void setup() {
    setupClickButtons();
    setupSignalButtons();
//    initializeMcpPinsAsSignalPinsForRelays(); only when expander pins are used as input
    printSensorDetails();
    Serial.println("Setup() called");
}

void presentation() {
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Gateway", "1.5");
    sendPresentation();
}

void loop() {
    readButtons();
    checkSignalAndRelayState();
    readSensors();
}

void receive(const MyMessage &message) {
    Serial.println("Calling receive()");
    if (message.type == V_STATUS) {
        switchRelay(message.getSensor());
    }
    Serial.println("Receive() called");
}
