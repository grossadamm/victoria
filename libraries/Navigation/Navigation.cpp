// #include "Arduino.h"
#include "Navigation.h"
#include "EEPROM.h"
#include "EEPROMAnything.h"
#include "avr/pgmspace.h"
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <AP_Declination.h>

const PROGMEM Waypoint MEMORY_VALIDATING_WAYPOINT = {123.0, 456.1, 20};
const PROGMEM Waypoint CHECKSUM_FAILED_WAYPOINT  = {999.99, 999.99, 255};
const PROGMEM int WAYPOINT_SIZE = 11;
const PROGMEM int MAX_WAYPOINTS = 362;
const PROGMEM int WAYPOINT_STORAGE_OFFSET = 2;
const PROGMEM int WAYPOINT_COUNT_POSITION = 1;
const PROGMEM float RADIANS = 0.0174532;

const PROGMEM int GPS_MOSFET = 24;

static const uint32_t GPSBaud = 9600;

// The serial connection to the GPS device

Navigation::Navigation(Sensors* sensors)
{
  _sensors = sensors;
  Serial1.begin(GPSBaud);
  validateEeprom(); // TODO what good is this?
  _currentWaypoint = retrieveWaypoint(1);
  _mag = Adafruit_HMC5883_Unified(12345);
}

boolean Navigation::ready()
{
  digitalWrite(GPS_MOSFET, HIGH);
   _mag.begin();
  while (Serial1.available() > 0) // read gps data if available
    _gps.encode(Serial1.read());

  if(_gps.location.isValid()){
    _currentDeclination = AP_Declination::get_declination(_gps.location.lat(),  _gps.location.lng()) * RADIANS;
    return true;
  }

  return false;
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
    return CHECKSUM_FAILED_WAYPOINT; // TODO high risk point
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
  Serial.println(waypoint.radiusHectometers);
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
         wp1.radiusHectometers    == wp2.radiusHectometers;
}

int Navigation::courseChangeNeeded() 
{
  // compare current waypoint to gps
  // if matched, shift forward waypoints
  double distanceToDestination = TinyGPSPlus::distanceBetween(
      _gps.location.lat(), _gps.location.lng(),
      (double) _currentWaypoint.latitude,
      (double) _currentWaypoint.longitude) / 100; // for hectometers
  
  if(distanceToDestination <  _currentWaypoint.radiusHectometers) {
    shiftWaypointsForward();
    _currentWaypoint = retrieveWaypoint(1);
  }

  // get current course
  // get desired course
  double currentCourse = currentHeading();
  double desiredCourse = TinyGPSPlus::courseTo(
      _gps.location.lat(),
      _gps.location.lng(),
      (double) _currentWaypoint.latitude,
      (double) _currentWaypoint.longitude);

  // course difference
  int courseChangeNeeded = (int)(360 + desiredCourse - currentCourse) % 360;

  // make left negative and right positive
  if(courseChangeNeeded > 180) {
    courseChangeNeeded = courseChangeNeeded - 360;
  } 

  // return -180(left) to 180(right) as integer
  return courseChangeNeeded;
}

int Navigation::waypointEepromPosition(int waypointIndex)
{
  return waypointIndex * WAYPOINT_SIZE + WAYPOINT_STORAGE_OFFSET;
}

int Navigation::retrieveWaypointCount() 
{
  return (int) EEPROM.read(WAYPOINT_COUNT_POSITION);
}

boolean Navigation::validateEeprom() {
  _numberOfWaypoints = retrieveWaypointCount();

  Waypoint waypoint = retrieveWaypoint(0);

  if(!compareWaypoints(waypoint, MEMORY_VALIDATING_WAYPOINT)){
    return false;
  }

  return true;
}

float Navigation::currentHeading() {
  sensors_event_t event; 
  _mag.getEvent(&event);

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  heading += _currentDeclination;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  return headingDegrees;
}