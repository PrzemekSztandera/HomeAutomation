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
//#define MY_GATEWAY_SERIAL


// Enable gateway ethernet module type
#define MY_GATEWAY_W5100
#define MY_MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
//#define MY_IP_ADDRESS 192,168,1,74
// If using static ip you need to define Gateway and Subnet address as well
//#define MY_IP_GATEWAY_ADDRESS 192,168,178,1
//#define MY_IP_SUBNET_ADDRESS 255,255,255,0
#define MY_PORT 5003

#define MY_GATEWAY_MQTT_CLIENT
#define MY_CONTROLLER_IP_ADDRESS 192, 168, 1, 73
#define MY_PORT 1883
#define MY_MQTT_USER "HAMqTT"
#define MY_MQTT_PASSWORD "home&assistantMqTT4"


#define MY_MQTT_PUBLISH_TOPIC_PREFIX "arduino-out"
#define MY_MQTT_SUBSCRIBE_TOPIC_PREFIX "arduino-in"
#define MY_MQTT_CLIENT_ID "arduino-mega"

//#define MY_DEBUG
#define USE_EXPANDER
//#define USE_EXPANDER_AS_INPUT

// Define a lower baud rate for Arduino's running on 8 MHz (Arduino Pro Mini 3.3V & SenseBender)
#if F_CPU == 16000000L
#define MY_BAUD_RATE 115200
#endif

// Remember to add library to Arduino path
#include <Ethernet.h>
#include <MySensors.h>
#include "./Automation/Automation.hpp"
#include "./Initialization/Initialization.hpp"
#include "./I2C/I2C_scanner.hpp"

void before() {
  // Initialize reset trigger pin to HIGH
    digitalWrite(relaySensors[0].getRelay().getPin(), HIGH);
    
    Serial.begin(115200);
    scanI2cDevices();
    createButtons();
#ifdef USE_EXPANDER
    initializeMCP23017();
#endif
    initializeRelays();
    initializeEnvironmentSensors();
    initializeTimers();
    Serial.println("before() called...!");
}

void setup() {
    setupButtons();
#ifdef USE_EXPANDER_AS_INPUT
    initializeMcpPinsAsSignalPinsForRelays();
#endif
    printRelaySensorDetails();
    Serial.println("setup() called...!");
}

void presentation() {
    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Arduino", "MQTT");
    sendPresentation();
    Serial.println("presentation() called...!");
}

void loop() {
    resetTimers();
    readButtons();
    updateEnvironmentSensors();
    resetTimers();
}

void receive(const MyMessage &message) {
    uint8_t sensorId = message.getSensor();
    Serial.print("Calling receive() for sensor: ");
    Serial.println(sensorId);
    if (message.type == getRelaySensor(sensorId).getVariableType()) {
        switchRelay(sensorId);
    }
    Serial.print("receive() called for sensor: ");
    Serial.println(message.getSensor());
}
