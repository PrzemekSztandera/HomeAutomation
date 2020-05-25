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
//#include "./Relay/Relay.hpp"
// TODO: as this should be fixed, refactor to keep index fixed
// sensor[0] -> msgs[0]
// sensor[1] -> msgs[1]
// etc.

#include <BME280I2C.h>
#include <Wire.h>

BME280I2C bme;

#define CHILD_ID_TEMP 101
#define CHILD_ID_BARO 102
#define CHILD_ID_BAT 103
#define CHILD_ID_HUM 104


unsigned long currentMillis = 0;
unsigned long currentMillis2 = 0;
MyMessage tempMsg(CHILD_ID_TEMP, V_TEMP);
MyMessage pressureMsg(CHILD_ID_BARO, V_PRESSURE);
//MyMessage pressureMsgPrefix(CHILD_ID_BARO, V_UNIT_PREFIX);
MyMessage humidityMsg(CHILD_ID_HUM, V_HUM);

void setup() {
    Serial.begin(115200);
    initializeRelayPins();
    setupClickButtons();
    setupPressButtons();
    Serial.println("Setup() function called");

    Wire.begin();

  while(!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    wait(1000);
  }

  // bme.chipID(); // Deprecated. See chipModel().
  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
  currentMillis = millis();
  currentMillis2 = millis();
}

void presentation() {

    // Send the sketch version information to the gateway and Controller
    sendSketchInfo("Gateway", "1.5");
    sendPresentation();

    // Register sensors to gw
  present(CHILD_ID_TEMP, S_TEMP, "temperature");
  present(CHILD_ID_BARO, S_BARO, "pressure");
  present(CHILD_ID_HUM, S_HUM, "humidity");

}


void loop() {

    readButtons();

    if(millis() - currentMillis2 > 2000) {
        for (uint8_t i = 0; i < maxSensors; i++) {
        auto sensor = Sensors[i];
        updateRelayStateAndSendMessage(sensor.id);
        }
        currentMillis2 = millis();
        return;
      }

    if(millis() - currentMillis > 10000) {
        float temp(NAN), hum(NAN), pres(NAN);
        BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
        BME280::PresUnit presUnit(BME280::PresUnit_Pa);
        bme.read(pres, temp, hum, tempUnit, presUnit);
        send(tempMsg.set(temp, 1));
        send(pressureMsg.set(pres, 0));
        send(humidityMsg.set(hum, 1));
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
