/**
 * Created by Przemyslaw Sztandera on 21/05/2020.
 */

#pragma once

#include <Wire.h>
#include <OneWire.h>
#include <BME280I2C.h>
#include <DallasTemperature.h>

// Bosh sensor BME280
BME280I2C bme;

// Dallas temp sensor DS18B20
#define ONE_WIRE_BUS 7
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature dallasSensors(&oneWire);
// Address of 1 DS18B20s
uint8_t sensor1[8] = {0x28, 0xC8, 0xF3, 0x79, 0xA2, 0x00, 0x03, 0xA8};

// CHILD_ID declaration of SensorStruct
#define ARDUINO_TIMER (uint8_t) 100
#define BME_TEMP (uint8_t) 101
#define BME_BARO (uint8_t) 102
#define BME_HUM (uint8_t) 103
//#define DALLAS_TEMP (uint8_t) 104


struct EnvironmentSensor {

    uint8_t id;
    mysensors_data_t variableType;
    mysensors_sensor_t presentationType;
    uint8_t discoveryType;
    char *description;

    uint8_t getId() {
        return id;
    }

    uint8_t getVariableType() {
        return variableType;
    }

    uint8_t getPresentationType() {
        return presentationType;
    }

    uint8_t getDiscoveryType() {
        return discoveryType;
    }

    char *getDescription() {
        return description;
    }
};

EnvironmentSensor environmentSensors[] = {
        {ARDUINO_TIMER, V_TEXT,     S_INFO, HA_DISCOVERY, "Arduino Timer"},
        {BME_TEMP,      V_TEMP,     S_TEMP, HA_DISCOVERY, "I2C BME_TEMP"},
        {BME_BARO,      V_PRESSURE, S_BARO, HA_DISCOVERY, "I2C BME_BARO"},
        {BME_HUM,       V_HUM,      S_HUM,  HA_DISCOVERY,  "I2C BME_HUM"},
//        {DALLAS_TEMP, V_TEMP, S_TEMP, HA_DISCOVERY, "Pin 7 - OneWire"},
};

const uint8_t numberOfEnvironmentSensors = sizeof(environmentSensors) / sizeof(EnvironmentSensor);

uint8_t getSensorIndex(uint8_t sensorId) {
    for (uint8_t i = 0; i < numberOfEnvironmentSensors; i++) {
        if (environmentSensors[i].getId() == sensorId) return (i);
    }
    return (-1);
}

EnvironmentSensor getEnvironmentSensor(uint8_t sensorId) {
    uint8_t index = getSensorIndex(sensorId);
    return environmentSensors[index];
}

