/**
 * Created by Przemyslaw Sztandera on 30/10/2020.
 */

#pragma once

#include <Wire.h>

void scanI2cDevices() {
    Wire.begin();
    Serial.println(F("\nI2C Scanner"));

    byte error, address;
    int nDevices;

    nDevices = 0;
    for(address = 1; address < 127; address++ )
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            Serial.print(F("I2C device found at address 0x"));
            if (address<16)
                Serial.print(F("0"));
            Serial.print(address,HEX);
            Serial.println(F("  !"));

            nDevices++;
        }
        else if (error==4)
        {
            Serial.print(F("Unknown error at address 0x"));
            if (address<16)
                Serial.print(F("0"));
            Serial.println(address,HEX);
        }
    }
    if (nDevices == 0)
        Serial.println(F("No I2C devices found\n"));
    else
        Serial.println(F("Done\n"));
}