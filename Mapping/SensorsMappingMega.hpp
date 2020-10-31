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
#define ONE_WIRE_BUS 69
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
// Address of 1 DS18B20s
uint8_t sensor1[8] = {0x28, 0xC8, 0xF3, 0x79, 0xA2, 0x00, 0x03, 0xA8};


// Child ID declaration of RelaysStruct
//const uint8_t SALOON_TEMP = 101;
//const uint8_t SALOON_BARO = 102;
//const uint8_t SALOON_HUM = 103;
const uint8_t SALOON_DALLAS_TEMP = 104;

typedef struct {
    const uint8_t id;
    char *description;
    const mysensors_data_t variableType;
    const mysensors_sensor_t presentationType;

    uint8_t getId() {
        return id;
    }

    char *getDescription() {
        return description;
    }

    mysensors_data_t getVariableType() {
        return variableType;
    }

    mysensors_sensor_t getPresentationType() {
        return presentationType;
    }
} SensorStruct;

SensorStruct environmentSensors[] = {
//        {SALOON_TEMP,        "Salon Temperatura", V_TEMP,     S_TEMP},
//        {SALOON_BARO,        "Salon Cisnienie",   V_PRESSURE, S_BARO},
//        {SALOON_HUM,         "Salon Wilgotnosc",  V_HUM,      S_HUM},
        {SALOON_DALLAS_TEMP, "Salon Temperatura", V_TEMP, S_TEMP},
};

const uint8_t maxSensors = sizeof(environmentSensors) / sizeof(SensorStruct);
MyMessage sensorMsgs[maxSensors];

byte getSensorIndex(uint8_t sensorId) {
    for (uint8_t i = 0; i < maxSensors; i++) {
        if (environmentSensors[i].getId() == sensorId) return (i);
    }
    return (-1);
}

SensorStruct getSensorStruct(uint8_t sensorId) {
    uint8_t index = getSensorIndex(sensorId);
    return environmentSensors[index];
}

