/**
 *
 * @file MappingMega.hpp
 * @author Przemyslaw Sztandera
 * based on https://github.com/Kirizaki/mysensors
 * Mappings.
 * @license GPL V2
 *
 */

#pragma once

#include <OneButton.h>

namespace Toggle {
  const uint8_t OFF  = 0;
  const uint8_t ON   = 1;
  const uint8_t FLIP = 2;
}
const bool ActiveLow = true;

// Pushbuttons declaration
// Remember that names should be consistent with main loop in gateway.ino
//Constructor
//OneButton(int pin, int active = LOW, bool pullupActive = true);
OneButton saloonMain(2, true);
OneButton saloonSide(3, true);
OneButton diningRoomMain(4, true);
//OneButton diningRoomSide(5, true);
//OneButton bedroomMain(6, true);
//OneButton bedroomSide(0, true);
//OneButton kid1Main(0, true);
//OneButton kid2Main(0, true);
//OneButton bathroom1(0, true);
//OneButton bathroom2(0, true);
//OneButton bathroom1Fan(0, true);
//OneButton bathroom2Fan(0, true);
//OneButton kitchen1(0, true);
//OneButton kitchen2(0, true);
//OneButton entrance(0, true);
//OneButton landing(0, true);
//OneButton corridor(0, true);
//OneButton hall(0, true);
//OneButton terrace1(0, true);
//OneButton terrace2(0, true);
//OneButton outside(0, true);
//OneButton garden(0, true);
//OneButton gate(0, true);
//OneButton attic1(0, true);
//OneButton attic2(0, true);

// Child ID declaration of relays
const uint8_t SALOON_1_ID       = 11;
const uint8_t SALOON_2_ID       = 12;
const uint8_t DINING_ROOM_1_ID  = 13;
const uint8_t DINING_ROOM_2_ID  = 14;
const uint8_t BEDROOM_1_ID      = 21;
const uint8_t BEDROOM_2_ID      = 22;
const uint8_t KID1_BEDROOM_ID   = 23;
const uint8_t KID2_BEDROOM_ID   = 24;
const uint8_t BATHROOM_1_ID     = 31;
const uint8_t BATHROOM_2_ID     = 32;
const uint8_t FAN_BATHROOM_1_ID = 41;
const uint8_t FAN_BATHROOM_2_ID = 42;
const uint8_t KITCHEN_1_ID      = 51;
const uint8_t KITCHEN_2_ID      = 52;
const uint8_t ENTRANCE_ID       = 61;
const uint8_t LANDING_ID        = 62;
const uint8_t CORRIDOR_ID       = 63;
const uint8_t HALL_ID           = 64;
const uint8_t TERRACE_MAIN_ID   = 71;
const uint8_t TERRACE_BACK_ID   = 72;
const uint8_t HOUSE_OUTSIDE_ID  = 73;
const uint8_t GARDEN_ID         = 74;
const uint8_t GATE_ID           = 75;
const uint8_t ATTIC_1_ID        = 81;
const uint8_t ATTIC_2_ID        = 82;

typedef struct {
  const uint8_t id;
  const char* description;
  const uint8_t pin;
  bool activeLow;
  const uint8_t buttonPin;
} SensorsStruct;

SensorsStruct Sensors [] = {
//  Child ID           description              pin activeLow isOn relatedButtons
  { SALOON_1_ID,       "Salon Glowne",          9, true, 2}, // 23
  { SALOON_2_ID,       "Salon Kinkiety",        10, true, 3}, // 25
  { DINING_ROOM_1_ID,  "Jadalnia Glowne",       11, true, 4}, // 27
//  { DINING_ROOM_2_ID,  "Jadalnia Kinkiety",     12, false }, // 29
//  { BEDROOM_1_ID,      "Sypialnia Glowne",      13, false }, // 31
//  { BEDROOM_2_ID,      "Sypialnia Kinkiety",    33, false },
//  { KID1_BEDROOM_ID,   "Pokoj Wojtek",          35, false },
//  { KID2_BEDROOM_ID,   "Pokoj Marcel",          37, false },
//  { BATHROOM_1_ID,     "Lazienka",              39, false },
//  { BATHROOM_2_ID,     "Toaleta",               41, false },
//  { FAN_BATHROOM_1_ID, "Lazienka Wentylator",   43, false },
//  { FAN_BATHROOM_2_ID, "Toaleta Wentylator",    45, false },
//  { KITCHEN_1_ID,      "Kuchnia Glowne",        47, false },
//  { KITCHEN_2_ID,      "Kuchnia Blat",          49, false },
//  { ENTRANCE_ID,       "Wejscie",               0, false },
//  { LANDING_ID,        "Wiatrolap",             0, false },
//  { CORRIDOR_ID,       "Przedpokoj",            0, false },
//  { HALL_ID,           "Hall",                  0, false },
//  { TERRACE_MAIN_ID,   "Taras Glowny",          0, false },
//  { TERRACE_BACK_ID,   "Taras Ogrod",           0, false },
//  { HOUSE_OUTSIDE_ID,  "Swiatla zewnetrzne",    0, false },
//  { GARDEN_ID,         "Ogrod",                 0, false },
//  { GATE_ID,           "Brama",                 0, false },
//  { ATTIC_1_ID,        "Strych 1",              0, false },
//  { ATTIC_2_ID,        "Strych 2",              0, false },
};
const uint8_t maxSensors = sizeof(Sensors) / sizeof(SensorsStruct);
MyMessage msgs[maxSensors];

byte getIndex(uint8_t sensorId) {
  for (uint8_t i = 0; i < maxSensors; i++) {
    if (Sensors[i].id == sensorId) return(i);
  }
  return(-1);
}

SensorsStruct getSensor(void* pSensorId){
  const uint8_t sensorId = static_cast<uint8_t>(reinterpret_cast<intptr_t>(pSensorId));
  uint8_t index = getIndex(sensorId);
  return Sensors[index];
}

