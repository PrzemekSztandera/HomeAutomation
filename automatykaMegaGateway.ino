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
#include "./Mapping/RelaysMappingMega.hpp"
#include "./Mapping/SensorsMappingMega.hpp"
#include "./Automation/AutomationMega.hpp"
#include "./Initialization/InitializationMega.hpp"

unsigned long currentMillis = 0;
unsigned long currentMillis2 = 0;

void setup() {
    Serial.begin(115200);
    initializeRelayPins();
    initializeSensors();
    setupClickButtons();
    setupPressButtons();
    Serial.println("Setup() function called");

// Bosh sensor BME280
    Wire.begin();
    while (!bme.begin()) {
        Serial.println("Could not find BME280 sensor!");
        wait(1000);
    }

    // bme.chipID(); // Deprecated. See chipModel().
    switch (bme.chipModel()) {
        case BME280::ChipModel_BME280:
            Serial.println("Found BME280 sensor! Success.");
            break;
        case BME280::ChipModel_BMP280:
            Serial.println("Found BMP280 sensor! No Humidity available.");
            break;
        default:
            Serial.println("Found UNKNOWN sensor! Error!");
    }
// Dallas temp sensor DS18B20
    sensors.begin();
    currentMillis = millis();
    currentMillis2 = millis();
}

void presentation() {

    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Gateway", "1.5");
    sendPresentation();
}


void loop() {

    readButtons();

    if(millis() - currentMillis2 > 2000) {
        for (uint8_t i = 0; i < maxRelays; i++) {
        auto relayStruct = Relays[i];
        updateRelayStateAndSendMessage(relayStruct.getId());
        }
        currentMillis2 = millis();
        return;
      }

    if (millis() - currentMillis > 10000) {
// Bosh sensor BME280
        float temp(NAN), hum(NAN), pres(NAN);
        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_Pa);
        bme.read(pres, temp, hum, tempUnit, presUnit);
        send(sensorMsgs[0].set(temp, 1));
        send(sensorMsgs[1].set(pres, 0));
        send(sensorMsgs[2].set(hum, 1));

// Dallas temp sensor DS18B20
        sensors.requestTemperatures();
        send(sensorMsgs[3].set(sensors.getTempC(sensor1), 1));

        Serial.println("Server update");
        currentMillis = millis();
        return;
    }

}

void receive(const MyMessage &message) {
    Serial.println("Calling receive()");

    // We only expect one type of message from controller. But we better check anyway.
    if (message.type == V_STATUS) {
        auto sensor = message.getSensor();
        switchRelay(sensor);
    }

    Serial.println("End of receive method");
}
