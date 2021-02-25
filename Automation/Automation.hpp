/**
 *
 * @file Automation.hpp
 * @author Przemyslaw Sztandera
 *
 */
#pragma once

bool updateRelayStateAndSendMessage(const uint8_t sensorId);

void switchRelay(const byte sensorId);

void updateEnvironmentSensors();

int freeRam ();

void printTime();

void receiveData();

#include "../Initialization/Initialization.hpp"
#include "../Button/ButtonsInitialization.hpp"

bool resetMiniMega = true;

bool updateRelayStateAndSendMessage(const uint8_t sensorId) {

    strip.setPixelColor(Led3, strip.Color(0, 5, 25));

    bool updated;
    bool pullUpActive = true;
    uint8_t oldState = loadState(sensorId);

    Sensor sensor = getSensor(sensorId);
    uint8_t signalState = sensor.readPin();

    Relay relay = getRelay(sensorId);
    uint8_t relayState = relay.readPin();
    uint8_t pinState = -1;

// save state of signal pin to EEPROM
    if (sensor.getPinType() == SIGNAL_PIN) {
        if (pullUpActive) {
            saveState(sensorId, !signalState);
        } else {
            saveState(sensorId, signalState);
        }
        pinState = signalState;
// save state of relay pin to EEPROM
    } else if (sensor.getPinType() == TRIGGER_PIN) {
        if (relay.isLatching()) {
            if (relay.isLowLevelTrigger()) {
                saveState(sensorId, !relayState);
            } else {
                saveState(sensorId, relayState);
            }
        } else {
            saveState(sensorId, oldState);
        }
        pinState = relayState;
    }

    uint8_t newState = loadState(sensorId);
    send(sensorMessages[getIndex(sensorId)].set(newState));
    // char id[4];
    // // itoa(sensorId, id, 4);
    // sprintf_P(id, (PGM_P)F("%05d"), sensorId);
    // sendSerialMessage("MS", "BI", id, newState);

    if (oldState == newState) {
        updated = true;
        Serial.print(F("updateRelayStateAndSendMessage() called and message send for sensor: "));
        Serial.print(sensorId);
        Serial.print(F(" New sensor state: "));
        Serial.print(newState);
        Serial.print(F(", new pin state: "));
        Serial.println(pinState);
    } else {
        updated = false;
        Serial.print(F("updateRelayStateAndSendMessage() called and message send for sensor: "));
        Serial.print(sensorId);
        Serial.print(F(" New sensor state: "));
        Serial.print(newState);
        Serial.print(F(", new pin state: "));
        Serial.println(pinState);
    }
    return updated;
}


void switchRelay(const uint8_t sensorId) {

    Serial.print(F("Calling switchRelay() for sensor: "));
    Serial.println(sensorId);

    Relay relay = getRelay(sensorId);
    uint8_t relayPin = relay.getPin();
    uint8_t relayState = relay.readPin();

// save new sensor state to EEPROM
    uint8_t sensorState = !loadState(sensorId);
    saveState(sensorId, sensorState);

    if (relay.isLatching()) {
        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relayPin, !relayState);
        } else {
            digitalWrite(relayPin, !relayState);
        }
    } else {
        if (relay.onExpander()) {
            expander[relay.getExpanderAddress()].digitalWrite(relayPin, !relayState);
            delay(125);
            expander[relay.getExpanderAddress()].digitalWrite(relayPin, relayState);
        } else {
            digitalWrite(relayPin, !relayState);
            delay(125);
            digitalWrite(relayPin, relayState);
        }
    }
    
    myDelay(150);

    if (updateRelayStateAndSendMessage(sensorId)) {

        Serial.print(F("switchRelay() called for sensor: "));
        Serial.print(sensorId);
        Serial.print(F(", new sensor state: "));
        Serial.print(loadState(sensorId));
        Serial.println(F(", Switch successful."));
    } else {
        Serial.print(F("switchRelay() called for sensor: "));
        Serial.print(sensorId);
        Serial.print(F(", new sensor state: "));
        Serial.print(loadState(sensorId));
        Serial.println(F(", Switch unsuccessful!!!"));
    }
}

void updateEnvironmentSensors() {

    strip.setPixelColor(Led3, strip.Color(0, 5, 25));

    send(sensorMessages[getIndex(ARDUINO_TIMER)].set(millis()));
 
    if (lightMeter.measurementReady()) {
        float lux = lightMeter.readLightLevel();
        Serial.print(F("Light: "));
        Serial.print(lux);
        Serial.println(F(" lx"));
        send(sensorMessages[getIndex(ARDUINO_LIGHT)].set(lux, 1));
    }

    // Bosh sensor BME280
       float temp(NAN), hum(NAN), pres(NAN);
       BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
       BME280::PresUnit presUnit(BME280::PresUnit_Pa);
       bme.read(pres, temp, hum, tempUnit, presUnit);
       send(sensorMessages[getIndex(BME280_TEMP)].set(temp, 1));
       send(sensorMessages[getIndex(BME280_BARO)].set(pres, 0));
       send(sensorMessages[getIndex(BME280_HUM)].set(hum, 1));


    Serial.println();
    Serial.print(F("Date & Time: "));
    currentTime = gmtime(&now);
    Serial.println(asctime(currentTime));
    Serial.println();
    Serial.print(F("Temperature: "));
    Serial.print(temp);
    Serial.println(F(" C"));
    Serial.println();
    Serial.print(F("Free RAM: "));
    Serial.print(freeRam());
    Serial.println(F(" bytes"));
    Serial.println();
    Serial.print(F("WI-FI: "));
    Serial.print(modem.getSignalQuality());
    Serial.println(F(" dB"));
    Serial.println();  
}

void resetArduinoMiniMega(int resetPin) {
    strip.setPixelColor(Mega2Led, strip.Color(25, 25, 0));
    strip.show();
    pinMode(resetPin, OUTPUT);
    if (resetMiniMega) {
        digitalWrite(resetPin, LOW);
        digitalWrite(resetPin, HIGH);
        resetMiniMega = false;
    } else {
        digitalWrite(resetPin, HIGH);
    }
}

int freeRam () {
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void clearEeprom() {
  // initialize the LED pin as an output.
  pinMode(13, OUTPUT);

  for (unsigned int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }

  // turn the LED on when we're done
  digitalWrite(13, HIGH);
}

