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

#include <uRTCLib.h>

uRTCLib clock;
unsigned long sensorsTimerHelper;


void myDelay(unsigned long interval) {
    unsigned long current = millis();
    while ((millis() - current < interval) && (millis() - current >= 0)) {}
}


void initializeTime() {
    
	URTCLIB_WIRE.begin();

	clock.set_rtc_address(URTCLIB_ADDRESS);
	clock.set_model(URTCLIB_MODEL_DS3231);

	// Only used once, then disabled
	// clock.set(20, 34, 20, 6, 30, 1, 21);
	//  RTCLib::set(byte second, byte minute, byte hour, byte dayOfWeek, byte dayOfMonth, byte month, byte year)

	if (clock.enableBattery()) {
		Serial.println(F("Battery activated correctly."));
	} else {
		Serial.println(F("ERROR activating battery."));
	}

	Serial.print(F("Lost power status: "));
	if (clock.lostPower()) {
		Serial.print(F("POWER FAILED. Clearing flag..."));
		clock.lostPowerClear();
		Serial.println(F(" done."));
	} else {
		Serial.println(F("POWER OK"));
	}

    clock.alarmClearFlag(URTCLIB_ALARM_1);
	clock.alarmClearFlag(URTCLIB_ALARM_2);

}

void printTimeAndTemp() {

    clock.refresh();

	Serial.print(F("DateTime: "));
    Serial.print(clock.day());
    Serial.print(F("/"));
	Serial.print(clock.month());
    Serial.print(F("/"));
	Serial.print(clock.year());
    Serial.print(F(" "));
	Serial.print(clock.hour());
    Serial.print(F(":"));
	Serial.print(clock.minute());
    Serial.print(F(":"));
	Serial.print(clock.second());

	Serial.print(F(" Day of week: "));
	Serial.println(clock.dayOfWeek());

	Serial.print(F("Temp: "));
	Serial.print((float) clock.temp() / 100);
    Serial.println(F(" °C"));

}

unsigned long getEpochInSeconds() {

    clock.refresh();

    unsigned long epoch = 0;
    unsigned long fromEpochStart = 946684799L; // number of seconds from 1/1/1970 to 31/12/1999
    unsigned long dayToSeconds = 86400L;
    int currentYearInCenturies = clock.year() + 2000;
    unsigned long yearsToSeconds = 0;
    unsigned long monthsToSeconds = 0;
    unsigned long daysToSeconds = 0;
    unsigned long timeToSeconds = 0;
    

    for (int i = 2000; i < currentYearInCenturies; i++) {
        if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0) {
            yearsToSeconds += 366UL * dayToSeconds;
        } else {
            yearsToSeconds += 365UL * dayToSeconds;
        }
    }

    for (int i = 1; i < clock.month(); i++) {

         switch (clock.month()) {
            case 1:
            case 3:
            case 5:
            case 7:
            case 8:
            case 10:
            case 12:
                monthsToSeconds += 31UL * dayToSeconds;
                break;
            case 2:
                if ((currentYearInCenturies % 4 == 0 && currentYearInCenturies % 100 != 0) || currentYearInCenturies % 400 == 0) {
                    monthsToSeconds += 29UL * dayToSeconds;
                } else {
                    monthsToSeconds += 28UL * dayToSeconds;
                }
                break;
            case 4:
            case 6:
            case 9:
            case 11:
                monthsToSeconds += 30UL * dayToSeconds;
                break;
            default:
                break;
        }

    }

    daysToSeconds = ((unsigned long)clock.day() - 1) *  dayToSeconds;

    timeToSeconds = ((unsigned long)clock.hour() * 3600UL) + ((unsigned long)clock.minute() * 60UL) + (unsigned long)clock.second() + fromEpochStart;

    epoch = yearsToSeconds + monthsToSeconds + daysToSeconds + timeToSeconds;
    
    return epoch;
}



bool timer(unsigned long interval) {
    
    if((getEpochInSeconds() - sensorsTimerHelper) > interval) {

        sensorsTimerHelper = getEpochInSeconds();
        return true;

    }
    return false;
}

// void myDelay(unsigned long interval) {
//     unsigned long current = millis();
//     while ((millis() - current < interval) && (millis() - current >= 0)) {}
// }
