/**
 *
 * @file Timer.hpp
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Automation for buttons & sensors
 * @license GPL V2
 *
 */
#pragma once

#include <RTClib.h>
#include <time.h>

RTC_DS3231 rtc;
const uint8_t tiemerInterruptPin = 2;

DateTime nowRTC;
time_t now;
struct tm *currentTime;

unsigned long timer1helper;
unsigned long timer2helper;
unsigned long timer3helper;
unsigned long timer4helper;

void myDelay(unsigned long interval) {
    unsigned long current = millis();
    while ((millis() - current < interval) && (millis() - current >= 0)) {}
}

void rtc_interrupt()
{
  Serial.println(F("second"));
}

void initializeTime() {

    if(!rtc.begin()) {
        Serial.println(F("Couldn't find RTC!"));
    }


    //we don't need the 32K Pin, so disable it
    rtc.disable32K();

    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2021, 2, 19, 22, 12, 00));

    if(rtc.lostPower()) {
        // this will adjust to the date and time at compilation
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }

    
    rtc.writeSqwPinMode(DS3231_SquareWave1Hz);
    Serial.print(F("Sqw pin mode: "));
    Serial.println(rtc.readSqwPinMode());
    // Keep the time in sync using the one-pulse-per-second output of the
    // RTC as an interrupt source and calling system_tick() from the
    // interrupt service routine.
    pinMode(tiemerInterruptPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(tiemerInterruptPin), system_tick, FALLING);
    // attachInterrupt(digitalPinToInterrupt(interrPin), rtc_interrupt, FALLING);

    rtc.clearAlarm(1);
    rtc.clearAlarm(2);


    // DateTime now = rtc.now(); 

    // DateTime alarm2 = DateTime (now.year()-2000, now.month(), now.day(), now.dayOfTheWeek(), now.hour(), now.minute());
    // rtc.setAlarm1(const DateTime &dt, Ds3231Alarm1Mode alarm_mode);
    // rtc.setAlarm2(alarm2, DS3231_A2_PerMinute);
   

    // disableAlarm(uint8_t alarm_num);

    // rtc.alarmFired(1);
    
    

    set_system_time(rtc.now().secondstime());
    set_position(51.8965858, 19.8226161);
   
}

bool sensorTimer(unsigned long interval) { 
    if((millis() - timer1helper) > (interval * 1000UL)) {

        timer1helper = millis();
        return true;

    }
    return false;
}

bool serialCommunicationTimer(unsigned long interval) { 
    if((millis() - timer2helper) > (interval * 1000UL)) {

        timer2helper = millis();
        return true;

    }
    return false;
}

bool ledTimer(unsigned long interval) { 
    if((millis() - timer3helper) > (interval * 1000UL)) {

        timer3helper = millis();
        return true;

    }
    return false;
}

bool heartBeatTimer(unsigned long interval) { 
    if((millis() - timer4helper) > (interval * 1000UL)) {

        timer4helper = millis();
        return true;

    }
    return false;
}


// bool timer1(unsigned long interval) {

//     if((nowRTC.unixtime() - timer1helper) > interval) {

//         timer1helper = nowRTC.unixtime();
//         return true;

//     }
//     return false;
// }

// bool timer2(unsigned long interval) {

//     if((nowRTC.unixtime() - timer2helper) > interval) {

//         timer2helper = nowRTC.unixtime();
//         return true;

//     }
//     return false;
// }

// bool timer3(unsigned long interval) {

//     if((nowRTC.unixtime() - timer3helper) > interval) {

//         timer3helper = nowRTC.unixtime();
//         return true;

//     }
//     return false;
// }
