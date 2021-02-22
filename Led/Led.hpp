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
    strip.setPixelColor(Mega1Led, strip.Color(50, 0, 0));
    
    strip.show();
}

void initializeLedsSetup() {
    strip.clear();
    strip.setPixelColor(Mega1Led, strip.Color(50, 30, 0));
    strip.setPixelColor(Mega2Led, strip.Color(50, 0, 0));
    strip.show();
}

void lightLeds() {
    
        // strip.clear(); // Set all pixel colors to 'off'

    
    if(heartBeatTimer(10)) {
        heartBeat = false;
        
        sendSerialMessage(F("RQ"), F("HB"), F(""), 1);
        strip.setPixelColor(Mega1Led, strip.Color(0, 0, 0));
        
    } else {
        strip.setPixelColor(Mega1Led, strip.Color(0, 50, 0));
    }

    if(heartBeat) {
        strip.setPixelColor(Mega2Led, strip.Color(0, 50, 0));
    }
           
    strip.show();
}