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
const uint8_t interrPin = 2;

// char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

unsigned long sensorsTimerHelper;
int secondCounter = 0;


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
        // Serial.flush();
        // abort();
    }


    //we don't need the 32K Pin, so disable it
    rtc.disable32K();

    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2021, 2, 4, 16, 49, 20));

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
    pinMode(interrPin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(interrPin), system_tick, FALLING);
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

void printDateAndTime() {
    DateTime now = rtc.now();

    Serial.print(F("("));
    // Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(F(") "));
    Serial.print(now.year(), DEC);
    Serial.print(F("/"));
    Serial.print(now.month(), DEC);
    Serial.print(F("/"));
    Serial.print(now.day(), DEC);
    Serial.print(F("....."));
    Serial.print(now.hour(), DEC);
    Serial.print(F(":"));
    Serial.print(now.minute(), DEC);
    Serial.print(F(":"));
    Serial.print(now.second(), DEC);
    Serial.println();

    Serial.print(F("EPOCH: "));
    Serial.print(now.unixtime());
    Serial.println(F("s"));

    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now + TimeSpan(7,12,30,6));

    // Serial.print(" now + 7d + 30s: ");
    // Serial.print(future.year(), DEC);
    // Serial.print('/');
    // Serial.print(future.month(), DEC);
    // Serial.print('/');
    // Serial.print(future.day(), DEC);
    // Serial.print(' ');
    // Serial.print(future.hour(), DEC);
    // Serial.print(':');
    // Serial.print(future.minute(), DEC);
    // Serial.print(':');
    // Serial.print(future.second(), DEC);
    // Serial.println();

    Serial.print(F("Temperature: "));
    Serial.print(rtc.getTemperature());
    Serial.println(F(" C"));

    Serial.println(F("----------------"));

    time_t now2 = time(nullptr);
    struct tm *currentTime = gmtime(&now2);
    Serial.println(asctime(currentTime));
    Serial.println(isotime(currentTime));


}


bool timer(unsigned long interval) {
    DateTime now = rtc.now();  
    if((now.unixtime() - sensorsTimerHelper) > interval) {

        sensorsTimerHelper = now.unixtime();
        return true;

    }
    // Serial.print(secondCounter++);
    //     Serial.print(F(" "));
    //     if(secondCounter == interval) {
    //         secondCounter = 0;
    //     }
    return false;
}
