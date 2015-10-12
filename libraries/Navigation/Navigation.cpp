#include "Arduino.h"
#include "Navigation.h"
#include "EEPROM.h"
#include "EEPROMAnything.h"
#include "avr/pgmspace.h"
#include "TinyGPS++.h"
#include "SoftwareSerial.h"

const PROGMEM Waypoint MEMORY_VALIDATING_WAYPOINT = {123.0, 456.1, 20};
const PROGMEM Waypoint CHECKSUM_FAILED_WAYPOINT  = {999.99, 999.99, 255};
const PROGMEM int WAYPOINT_SIZE = 11;
const PROGMEM int MAX_WAYPOINTS = 362;
const PROGMEM int WAYPOINT_STORAGE_OFFSET = 2;
const PROGMEM int WAYPOINT_COUNT_POSITION = 1;

static const int RXPin = 15, TXPin = 14;
static const uint32_t GPSBaud = 4800;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

Navigation::Navigation(Sensors* sensors)
{
  _sensors = sensors;
  ss.begin(GPSBaud);
}

boolean Navigation::ready()
{
  _numberOfWaypoints = retrieveWaypointCount();

  Waypoint waypoint = retrieveWaypoint(WAYPOINT_STORAGE_OFFSET);

  if(compareWaypoints(waypoint, MEMORY_VALIDATING_WAYPOINT)){
    return true;
  } else {
    return false;
  }
}

void Navigation::resetWaypoints() {
  storeWaypoint(MEMORY_VALIDATING_WAYPOINT, 0);
  Waypoint waypoints[6];
  waypoints[0] = {37.3701, -122.4269, 20};
  waypoints[1] = {37.1004, -124.1135, 20};
  waypoints[2] = {30, -125, 70};
  waypoints[3] = {25, -128, 70};
  waypoints[4] = {13, -172, 70};

  _numberOfWaypoints = 1;
  for(int i = 0; i < sizeof(waypoints); i++){
    appendNewWaypoint(waypoints[i]);
  }
}

void Navigation::storeWaypoint(Waypoint& waypoint, int position)
{
  waypoint.checksum = 0;
  byte waypointChecksum = CRC8((byte *) &waypoint, sizeof(waypoint));
  waypoint.checksum = waypointChecksum;
  
  int usedBytes = EEPROM_writeAnything(waypointEepromPosition(position), waypoint);
}

byte Navigation::CRC8(const byte *data, byte len) {
  byte crc = 0x00;
  while (len--) {
    byte extract = *data++;
    for (byte tempI = 8; tempI; tempI--) {
      byte sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum) {
        crc ^= 0x8C;
      }
      extract >>= 1;
    }
  }
  return crc;
}


void Navigation::appendNewWaypoint(Waypoint& waypoint)
{
  EEPROM_writeAnything(waypointEepromPosition(_numberOfWaypoints), waypoint);
  _numberOfWaypoints++;
  EEPROM.write(WAYPOINT_COUNT_POSITION, _numberOfWaypoints);
}

void Navigation::storeWaypoint(const Waypoint& waypoint, int position)
{
  EEPROM_writeAnything(waypointEepromPosition(position), waypoint);
}

Waypoint Navigation::retrieveWaypoint(int waypointNumber)
{
  Waypoint waypoint;
  EEPROM_readAnything(waypointEepromPosition(waypointNumber), waypoint);
  if(!validateWaypoint(waypoint)) {
    Serial.println("checksum failed");
    return CHECKSUM_FAILED_WAYPOINT;
  } else {
    return waypoint;
  }
}

void Navigation::shiftWaypointsForward()
{
  for(int i = 1; i < _numberOfWaypoints; i++){
    Navigation::storeWaypoint(Navigation::retrieveWaypoint(i), i-1);
  }
  _numberOfWaypoints--;
  EEPROM.write(WAYPOINT_COUNT_POSITION, _numberOfWaypoints);
}

void Navigation::printWaypoint(Waypoint waypoint){
  Serial.println(waypoint.latitude);
  Serial.println(waypoint.longitude);
  Serial.println(waypoint.radius);
}

boolean Navigation::validateWaypoint(Waypoint waypoint)
{
  byte readChecksum = waypoint.checksum;
  waypoint.checksum = 0;

  byte calculatedChecksum = CRC8((byte *) &waypoint, sizeof(waypoint));

  waypoint.checksum = readChecksum;

  return readChecksum == calculatedChecksum;
}

boolean Navigation::compareWaypoints(Waypoint wp1, Waypoint wp2)
{
  return wp1.longitude == wp2.longitude &&
         wp1.latitude  == wp2.latitude &&
         wp1.radius    == wp2.radius;
}

LeftRightCenter Navigation::leftRightCenter() 
{
  // compare current waypoint to gps
  // if matched, shift forward
  // 
  // set course towards waypoint
  return CENTER;
}

int Navigation::waypointEepromPosition(int waypointIndex)
{
  return waypointIndex * WAYPOINT_SIZE + WAYPOINT_STORAGE_OFFSET;
}

int Navigation::retrieveWaypointCount() 
{
  return (int) EEPROM.read(WAYPOINT_COUNT_POSITION);
}