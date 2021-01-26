/**
 *
 * @file automatykaMegaGateway.ino
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Implementation of the MySensors Gateway.
 * @license GPL V2
 *
 */


// CHILD_ID declaration of sensors
#define SIGNAL_IN_11 (uint8_t) 11
#define SIGNAL_IN_12 (uint8_t) 12
#define SIGNAL_IN_13 (uint8_t) 13
#define SIGNAL_IN_14 (uint8_t) 14
#define SIGNAL_IN_15 (uint8_t) 15
#define SIGNAL_IN_21 (uint8_t) 21
#define SIGNAL_IN_22 (uint8_t) 22
#define SIGNAL_IN_23 (uint8_t) 23
#define SIGNAL_IN_24 (uint8_t) 24
#define SIGNAL_IN_31 (uint8_t) 31
#define SIGNAL_IN_32 (uint8_t) 32
#define SIGNAL_IN_33 (uint8_t) 33
#define SIGNAL_IN_34 (uint8_t) 34
#define SIGNAL_IN_35 (uint8_t) 35
#define SIGNAL_IN_36 (uint8_t) 36
#define SIGNAL_IN_37 (uint8_t) 37
#define SIGNAL_IN_38 (uint8_t) 38
#define SIGNAL_IN_41 (uint8_t) 41
#define SIGNAL_IN_42 (uint8_t) 42
#define SIGNAL_IN_43 (uint8_t) 43
#define SIGNAL_IN_44 (uint8_t) 44
#define SIGNAL_IN_45 (uint8_t) 45
#define SIGNAL_IN_46 (uint8_t) 46
#define SIGNAL_IN_51 (uint8_t) 51
#define SIGNAL_IN_52 (uint8_t) 52
#define SIGNAL_IN_53 (uint8_t) 53
#define SIGNAL_IN_54 (uint8_t) 54
#define SIGNAL_IN_55 (uint8_t) 55
#define SIGNAL_IN_56 (uint8_t) 56
#define SIGNAL_IN_61 (uint8_t) 61
#define SIGNAL_IN_62 (uint8_t) 62
#define SIGNAL_IN_71 (uint8_t) 71
#define SIGNAL_OUT_72 (uint8_t) 72
#define SIGNAL_OUT_73 (uint8_t) 73

#define ARDUINO_TIMER (uint8_t) 100
#define BME_TEMP (uint8_t) 101
#define BME_BARO (uint8_t) 102
#define BME_HUM (uint8_t) 103
//#define DALLAS_TEMP (uint8_t) 104

// Enable serial gateway
//#define MY_GATEWAY_SERIAL

 
#define MY_GATEWAY_TINYGSM
#define TINY_GSM_MODEM_ESP8266
// Use Hardware Serial on Mega, Leonardo, Micro
#define SerialAT Serial1
#define MY_GSM_SSID "BTHub6-G83T"
#define MY_GSM_PSW "UcXJ3Fk4uQXR"


// Enable gateway ethernet module type
#define MY_GATEWAY_W5100


#define MY_MAC_ADDRESS 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
#define MY_IP_ADDRESS 192,168,1,86
// If using static ip you need to define Gateway and Subnet address as well
#define MY_IP_GATEWAY_ADDRESS 192,168,1,254
#define MY_IP_SUBNET_ADDRESS 255,255,255,0
#define MY_PORT 5003


#define MY_GATEWAY_MQTT_CLIENT
#define MY_CONTROLLER_IP_ADDRESS 192, 168, 1, 73
#define MY_PORT 1883
#define MY_MQTT_USER "HAMqTT"
#define MY_MQTT_PASSWORD "home&assistantMqTT4"
#define MY_MQTT_PUBLISH_TOPIC_PREFIX "arduino-out"
#define MY_MQTT_SUBSCRIBE_TOPIC_PREFIX "arduino-in"
#define MY_MQTT_CLIENT_ID "arduino-mega"


#define MY_DEBUG
#define SETUP_DEBUG
#define USE_EXPANDER
//#define USE_EXPANDER_AS_INPUT

#define HA_DISCOVERY 1
#define MS_DISCOVERY 2

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
  
    Serial.begin(115200);
    scanI2cDevices();
    createAndSetButtons();
#ifdef USE_EXPANDER
    initializeMCP23017();
#endif
    initializeAndSetRelays();
    initializeEnvironmentSensors();
    initializeTimers();
    Serial.println(F("before() called...!"));
}

void setup() {
  
#ifdef USE_EXPANDER_AS_INPUT
    initializeSensorPinsOnExpander();
#endif

#ifdef SETUP_DEBUG
    printRelaySensorDetails();
#endif
    Serial.println(F("setup() called...!"));
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
    uint8_t sensorId = message.sensor;
    Serial.print(F("Calling receive() for sensor: "));
    Serial.println(sensorId);
    if (message.type == getSensor(sensorId).getVariableType()) {
        switchRelay(sensorId);
    }
    Serial.print(F("receive() called for sensor: "));
    Serial.println(sensorId);
}
