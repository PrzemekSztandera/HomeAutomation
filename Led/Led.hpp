/**
 *
 * @file Led.hpp
 * @author Przemyslaw Sztandera
 *
 */

#pragma once

#include <Adafruit_NeoPixel.h>

#define PIN 5
#define NUMPIXELS 4

#define Mega1Led (uint8_t) 0
#define Mega2Led (uint8_t) (uint8_t) 1
#define Led3 (uint8_t) 2
#define Led4 (uint8_t) 3

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void initializeLedsBefore() {
    strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.clear();
    strip.setPixelColor(Mega1Led, strip.Color(25, 0, 0));
    strip.setPixelColor(Mega2Led, strip.Color(25, 0, 0));
    strip.setPixelColor(Led3, strip.Color(25, 0, 0));
    strip.setPixelColor(Led4, strip.Color(25, 0, 0));
    strip.show();
}

void initializeLedsSetup() {
    strip.clear();
    strip.setPixelColor(Mega1Led, strip.Color(25, 25, 0));
    strip.setPixelColor(Mega2Led, strip.Color(25, 0, 0));
    strip.setPixelColor(Led3, strip.Color(25, 0, 0));
    strip.setPixelColor(Led4, strip.Color(25, 0, 0));
    strip.show();
}

void lightLeds() {
    strip.show();
    myDelay(50);
    if(heartBeatTimer(10)) {
        strip.setPixelColor(Mega1Led, strip.Color(0, 0, 0));
    } else {
        strip.setPixelColor(Mega1Led, strip.Color(0, 25, 0));
    }
    strip.setPixelColor(Led3, strip.Color(25, 25, 0));

    if(digitalRead(getRelay(SIGNAL_IN_72).getPin()) == LOW) {
        strip.setPixelColor(Led4, strip.Color(0, 5, 25));
    } else {
        strip.setPixelColor(Led4, strip.Color(25, 0, 0));
    }
    
}

void setMegaMiniStatus() {
    if(digitalRead(miniMegaStatusInterruptPin) == HIGH) {
        strip.setPixelColor(Mega2Led, strip.Color(0, 25, 0));
    } else if (digitalRead(miniMegaStatusInterruptPin) == LOW) {
        strip.setPixelColor(Mega2Led, strip.Color(0, 0, 0));
    }
}