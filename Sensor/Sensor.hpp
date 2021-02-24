/**
 *
 * @file Sensor.hpp
 * @author Przemyslaw Sztandera
 *
 */

#pragma once
#include "../Relay/Relay.hpp"


// RelaySensor pin type
#define SIGNAL_PIN (uint8_t) 1
#define TRIGGER_PIN (uint8_t) 2
#define NO_PIN (uint8_t) 3


class Sensor {
private:
    uint8_t id;
    mysensors_data_t variableType;
    mysensors_sensor_t presentationType;
    uint8_t discoveryType;
    char const *description;
    uint8_t pin;
    uint8_t pinType;
    uint8_t expanderAddress;

public:
    Sensor(uint8_t id, mysensors_data_t dataType, mysensors_sensor_t sensorType, uint8_t discovery,
           char const *description, uint8_t pin = -1, uint8_t pinType = NO_PIN, uint8_t expander = 8) {
        this->id = id;
        this->variableType = dataType;
        this->presentationType = sensorType;
        this->discoveryType = discovery;
        this->description = description;
        this->pin = pin;
        this->pinType = pinType;
        this->expanderAddress = expander;
    }

    uint8_t getId() {
        return id;
    }

    mysensors_data_t getVariableType() {
        return variableType;
    }

    mysensors_sensor_t getPresentationType() {
        return presentationType;
    }

    uint8_t getDiscoveryType() {
        return discoveryType;
    }

    char const *getDescription() {
        return description;
    }

    uint8_t getPin() {
        return pin;
    }

    uint8_t getPinType() {
        return pinType;
    }

    uint8_t getExpanderAddress() {
        return expanderAddress;
    }

    bool onExpander() {
        return (expanderAddress >= 0 && expanderAddress < 8);
    }

    uint8_t readPin() {
        return this->onExpander() ? expander[this->getExpanderAddress()].digitalRead(this->getPin()) : digitalRead(
                this->getPin());
    }
};

// D0, D1, D2, D4, D10, D13 - D21, D49 - D54 : do not use
Sensor sensors[] = {

        // CHILD_ID / variable / presentation / discovery / description / pin / pin type / expanderAddress / hasSignalPin / relay
        {SIGNAL_IN_11,  V_STATUS,   S_BINARY, HA_DISCOVERY, "22",     22, SIGNAL_PIN,  8}, // 1
        {SIGNAL_IN_12,  V_STATUS,   S_BINARY, HA_DISCOVERY, "23",     23, SIGNAL_PIN,  8}, // 2
        {SIGNAL_IN_13,  V_STATUS,   S_BINARY, HA_DISCOVERY, "24",     24, SIGNAL_PIN,  8}, // 3
        {SIGNAL_IN_14,  V_STATUS,   S_BINARY, HA_DISCOVERY, "25",     25, SIGNAL_PIN,  8}, // 4
        {SIGNAL_IN_15,  V_STATUS,   S_BINARY, HA_DISCOVERY, "26",     26, SIGNAL_PIN,  8}, // 5
        {SIGNAL_IN_21,  V_STATUS,   S_BINARY, HA_DISCOVERY, "27",     27, SIGNAL_PIN,  8}, // 6
        {SIGNAL_IN_22,  V_STATUS,   S_BINARY, HA_DISCOVERY, "28",     28, SIGNAL_PIN,  8}, // 7
        {SIGNAL_IN_23,  V_STATUS,   S_BINARY, HA_DISCOVERY, "29",     29, SIGNAL_PIN,  8}, // 8
        {SIGNAL_IN_24,  V_STATUS,   S_BINARY, HA_DISCOVERY, "30",     30, SIGNAL_PIN,  8}, // 9
        {SIGNAL_IN_31,  V_STATUS,   S_BINARY, HA_DISCOVERY, "31",     31, SIGNAL_PIN,  8}, // 10
        {SIGNAL_IN_32,  V_STATUS,   S_BINARY, HA_DISCOVERY, "32",     32, SIGNAL_PIN,  8}, // 11
        {SIGNAL_IN_33,  V_STATUS,   S_BINARY, HA_DISCOVERY, "33",     33, SIGNAL_PIN,  8}, // 12
        {SIGNAL_IN_34,  V_STATUS,   S_BINARY, HA_DISCOVERY, "34",     34, SIGNAL_PIN,  8}, // 13
        {SIGNAL_IN_35,  V_STATUS,   S_BINARY, HA_DISCOVERY, "35",     35, SIGNAL_PIN,  8}, // 14
        {SIGNAL_IN_36,  V_STATUS,   S_BINARY, HA_DISCOVERY, "36",     36, SIGNAL_PIN,  8}, // 15
        {SIGNAL_IN_37,  V_STATUS,   S_BINARY, HA_DISCOVERY, "37",     37, SIGNAL_PIN,  8}, // 16
        {SIGNAL_IN_38,  V_STATUS,   S_BINARY, HA_DISCOVERY, "38",     38, SIGNAL_PIN,  8}, // 17
        {SIGNAL_IN_41,  V_STATUS,   S_BINARY, HA_DISCOVERY, "39",     39, SIGNAL_PIN,  8}, // 18
        {SIGNAL_IN_42,  V_STATUS,   S_BINARY, HA_DISCOVERY, "40",     40, SIGNAL_PIN,  8}, // 19
        {SIGNAL_IN_43,  V_STATUS,   S_BINARY, HA_DISCOVERY, "41",     41, SIGNAL_PIN,  8}, // 20
        {SIGNAL_IN_44,  V_STATUS,   S_BINARY, HA_DISCOVERY, "42",     42, SIGNAL_PIN,  8}, // 21
        {SIGNAL_IN_45,  V_STATUS,   S_BINARY, HA_DISCOVERY, "43",     43, SIGNAL_PIN,  8}, // 22
        {SIGNAL_IN_46,  V_STATUS,   S_BINARY, HA_DISCOVERY, "44",     44, SIGNAL_PIN,  8}, // 23
        {SIGNAL_IN_51,  V_STATUS,   S_BINARY, HA_DISCOVERY, "45",     45, SIGNAL_PIN,  8}, // 24
        {SIGNAL_IN_52,  V_STATUS,   S_BINARY, HA_DISCOVERY, "46",     46, SIGNAL_PIN,  8}, // 25
        {SIGNAL_IN_53,  V_STATUS,   S_BINARY, HA_DISCOVERY, "47",     47, SIGNAL_PIN,  8}, // 26
        {SIGNAL_IN_54,  V_STATUS,   S_BINARY, HA_DISCOVERY, "48",     48, SIGNAL_PIN,  8}, // 27
        {SIGNAL_IN_55,  V_STATUS,   S_BINARY, HA_DISCOVERY, "A8",     62, SIGNAL_PIN,  8}, // 28
        {SIGNAL_IN_56,  V_STATUS,   S_BINARY, HA_DISCOVERY, "A9",     63, SIGNAL_PIN,  8}, // 29
        {SIGNAL_IN_61,  V_STATUS,   S_BINARY, HA_DISCOVERY, "A10",    64, SIGNAL_PIN,  8}, // 30
        {SIGNAL_IN_62,  V_STATUS,   S_BINARY, HA_DISCOVERY, "A11",    65, SIGNAL_PIN,  8}, // 31
        {SIGNAL_IN_71,  V_STATUS,   S_BINARY, HA_DISCOVERY, "A12",    66, SIGNAL_PIN,  8}, // 32
        {SIGNAL_IN_72,  V_STATUS,   S_BINARY, HA_DISCOVERY, "A13/A0", 67, TRIGGER_PIN, 8}, // 33 use only with latching relays

        {SIGNAL_IN_73,  V_TRIPPED,  S_MOTION, HA_DISCOVERY, "A14",    68, SIGNAL_PIN,  8},
        {SIGNAL_IN_74,  V_TRIPPED,  S_MOTION, HA_DISCOVERY, "A15",    69, SIGNAL_PIN,  8},

        // Environment sensors
        {ARDUINO_TIMER, V_TEXT,     S_INFO,         HA_DISCOVERY, "TIMER"},
        {ARDUINO_LIGHT, V_LEVEL,    S_LIGHT_LEVEL,  HA_DISCOVERY, "LIGHT"},
        {BME280_TEMP,   V_TEMP,     S_TEMP,         HA_DISCOVERY, "BME_T"},
        {BME280_BARO,   V_PRESSURE, S_BARO,         HA_DISCOVERY, "BME_P"},
        {BME280_HUM,    V_HUM,      S_HUM,          HA_DISCOVERY, "BME_H"},
};

const uint8_t numberOfSensors = sizeof(sensors) / sizeof(Sensor);

//    const uint8_t sensorId = static_cast<uint8_t>(reinterpret_cast<intptr_t>(pSensorId));

uint8_t getIndex(uint8_t sensorId) {
    for (uint8_t i = 0; i < numberOfSensors; i++) {
        if (sensors[i].getId() == sensorId) return (i);
    }
    return (-1);
}

Sensor getSensor(uint8_t sensorId) {
    uint8_t index = getIndex(sensorId);
    return sensors[index];
}