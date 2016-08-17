// #include "Arduino.h"
#include "Storage.h"
#include "Arduino.h"
#include "EEPROM.h"
#include "EEPROMAnything.h"
#include "avr/pgmspace.h"

Storage::Storage()
{
  _numberOfWaypoints = ReadWaypointCount();
}

const PROGMEM int WAYPOINT_COUNT_STORAGE_POSITION = 1;
const PROGMEM int DAY_LAST_COMMUNICATED_STORAGE_POSITION = 2;
const PROGMEM int COMMUNICATION_INTERVAL_DAYS_POSITION = 3;
const PROGMEM int MOTOR_TEST_RUN_FREQUENCY_POSITION = 4;
const PROGMEM int RUDDER_ZERO_POSITION_POSITION = 5;
const PROGMEM int NAV_FAILURES_POSITION = 6;
const PROGMEM int WAYPOINT_STORAGE_POSITION = 7;

void Storage::lastCommunicatedOn(int day) {
  EEPROM.write(DAY_LAST_COMMUNICATED_STORAGE_POSITION, (char) day);
}

int Storage::lastCommunicatedOn() {
  (int) EEPROM.read(DAY_LAST_COMMUNICATED_STORAGE_POSITION);
}

void Storage::communicationIntervalDays(int days) {
  EEPROM.write(COMMUNICATION_INTERVAL_DAYS_POSITION, (char) days);
}

int Storage::communicationIntervalDays() {
  (int) EEPROM.read(COMMUNICATION_INTERVAL_DAYS_POSITION);
}

void Storage::motorTestRunFrequency(int runsBetweenTest) {
  EEPROM.write(MOTOR_TEST_RUN_FREQUENCY_POSITION, (char) runsBetweenTest);
}

int Storage::motorTestRunFrequency() {
  (int) EEPROM.read(MOTOR_TEST_RUN_FREQUENCY_POSITION);
}

void Storage::rudderZeroPosition(int rudderZeroPosition) {
  EEPROM.write(RUDDER_ZERO_POSITION_POSITION, (char) rudderZeroPosition);
}

int Storage::rudderZeroPosition() {
  (int) EEPROM.read(RUDDER_ZERO_POSITION_POSITION);
}

void Storage::navFailures(int navFailures) {
  EEPROM.write(NAV_FAILURES_POSITION, (char) navFailures);
}

int Storage::navFailures() {
  (int) EEPROM.read(NAV_FAILURES_POSITION);
}

// Waypoints
const PROGMEM Waypoint CHECKSUM_FAILED_WAYPOINT  = {999.99, 999.99, 255};
const PROGMEM int WAYPOINT_SIZE = 11;
const PROGMEM int MAX_WAYPOINTS = 362; // TODO recalculate

void Storage::StoreWaypoint(Waypoint waypoint, int position)
{
  EEPROM_writeAnything(waypointEepromPosition(position), waypoint);
}

void Storage::AppendNewWaypoint(Waypoint waypoint)
{
  EEPROM_writeAnything(waypointEepromPosition(_numberOfWaypoints), waypoint);
  _numberOfWaypoints++;
  SetWaypointCount(_numberOfWaypoints);
}

Waypoint Storage::RetrieveWaypoint(int waypointNumber)
{
  Waypoint waypoint;
  EEPROM_readAnything(waypointEepromPosition(waypointNumber), waypoint);
  if(!validateWaypoint(waypoint)) {
    return CHECKSUM_FAILED_WAYPOINT; // TODO high risk point
  } else {
    return waypoint;
  }
}

void Storage::SetWaypointCount(int waypointCount){
  EEPROM.write(WAYPOINT_COUNT_STORAGE_POSITION, _numberOfWaypoints);
}

int Storage::ReadWaypointCount() 
{
  return (int) EEPROM.read(WAYPOINT_COUNT_STORAGE_POSITION);
}

int Storage::waypointEepromPosition(int waypointIndex)
{
  return waypointIndex * WAYPOINT_SIZE + WAYPOINT_STORAGE_POSITION;
}

boolean Storage::validateWaypoint(Waypoint waypoint)
{
  byte readChecksum = waypoint.checksum;
  waypoint.checksum = 0;

  byte calculatedChecksum = CRC8((byte *) &waypoint, sizeof(waypoint));

  waypoint.checksum = readChecksum;

  return readChecksum == calculatedChecksum;
}


byte Storage::CRC8(const byte *data, byte len) {
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