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
const uint8_t ARDUINO_TIMER = 100;
const uint8_t BME_TEMP = 101;
const uint8_t BME_BARO = 102;
const uint8_t BME_HUM = 103;
//const uint8_t DALLAS_TEMP = 104;

typedef struct {
    const uint8_t id;
    const mysensors_data_t variableType;
    const mysensors_sensor_t presentationType;
    char *description;

    uint8_t getId() {
        return id;
    }

    mysensors_data_t getVariableType() {
        return variableType;
    }

    mysensors_sensor_t getPresentationType() {
        return presentationType;
    }

    char *getDescription() {
        return description;
    }
} EnvironmentSensor;

EnvironmentSensor environmentSensors[] = {
        {ARDUINO_TIMER, V_TEXT,     S_INFO, "Arduino Timer"},
        {BME_TEMP,      V_TEMP,     S_TEMP, "I2C BME_TEMP"},
        {BME_BARO,      V_PRESSURE, S_BARO, "I2C BME_BARO"},
        {BME_HUM,       V_HUM,      S_HUM,  "I2C BME_HUM"},
//        {DALLAS_TEMP, V_TEMP, S_TEMP, "Pin 7 - OneWire"},
};

const uint8_t numberOfEnvironmentSensors = sizeof(environmentSensors) / sizeof(EnvironmentSensor);

byte getSensorIndex(uint8_t sensorId) {
    for (uint8_t i = 0; i < numberOfEnvironmentSensors; i++) {
        if (environmentSensors[i].getId() == sensorId) return (i);
    }
    return (-1);
}

EnvironmentSensor getEnvironmentSensor(uint8_t sensorId) {
    uint8_t index = getSensorIndex(sensorId);
    return environmentSensors[index];
}

