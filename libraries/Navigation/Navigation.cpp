// #include "Arduino.h"
#include "Navigation.h"
#include "TinyGPS++.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <AP_Declination.h>

const PROGMEM Waypoint CHECKSUM_FAILED_WAYPOINT  = {999.99, 999.99, 255};
const PROGMEM float RADIANS = 0.0174532;

static const uint32_t GPSBaud = 9600;

const PROGMEM double FAKE_LAT = 51.5034070;
const PROGMEM double FAKE_LONG =  -0.1275920;
const PROGMEM float FAKE_HEADING =  90;

// #define USE_FAKE_LOCATION 1
// The serial connection to the GPS device

Navigation::Navigation(Sensors* sensors, Storage* storage, Power* power)
{
  _sensors = sensors;
  _storage = storage;
  _power = power;
  Serial3.begin(GPSBaud);
  _currentWaypoint = _storage->RetrieveWaypoint(0);
  _mag = Adafruit_HMC5883_Unified(12345);
}

boolean Navigation::ready()
{
  #if defined(USE_FAKE_LOCATION)
    return true;
  #endif

  _power->gps(true);
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
  Waypoint waypoints[6];
  waypoints[0] = {37.3701, -122.4269, 20};
  waypoints[1] = {37.1004, -124.1135, 20};
  waypoints[2] = {30, -125, 70};
  waypoints[3] = {25, -128, 70};
  waypoints[4] = {13, -172, 70};

  _storage->SetWaypointCount(0);
  Waypoint waypoint;
  for(int i = 0; i < sizeof(waypoints); i++){
    waypoint = setWaypointChecksum(waypoints[i]);
    _storage->AppendNewWaypoint(waypoint);
  }
}

Waypoint Navigation::setWaypointChecksum(Waypoint waypoint){
  waypoint.checksum = 0;
  byte waypointChecksum = CRC8((byte *) &waypoint, sizeof(waypoint));
  waypoint.checksum = waypointChecksum;
}

void Navigation::shiftWaypointsForward()
{
  int numberOfWaypoints = _storage->ReadWaypointCount();
  for(int i = 1; i < numberOfWaypoints; i++){
    _storage->StoreWaypoint(_storage->RetrieveWaypoint(i), i-1);
  }
  numberOfWaypoints--;
  _storage->SetWaypointCount(numberOfWaypoints);
}

void Navigation::printWaypoint(Waypoint waypoint){
  Serial.println(waypoint.latitude);
  Serial.println(waypoint.longitude);
  Serial.println(waypoint.radiusHectometers);
  Serial.println(waypoint.checksum);
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
      lat(), lng(),
      (double) _currentWaypoint.latitude,
      (double) _currentWaypoint.longitude) / 100; // for hectometers
  
  if(distanceToDestination <  _currentWaypoint.radiusHectometers) {
    shiftWaypointsForward();
    _currentWaypoint = _storage->RetrieveWaypoint(0);
  }

  // get current course
  // get desired course
  double currentCourse = currentHeading();
  double desiredCourse = TinyGPSPlus::courseTo(
      lat(), lng(),
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

double Navigation::lat() {
  #if defined(USE_FAKE_LOCATION)
    return FAKE_LAT;
  #else
    return _gps.location.lat();
  #endif
}

double Navigation::lng() {
  #if defined(USE_FAKE_LOCATION)
    return FAKE_LONG;
  #else
    return _gps.location.lng();
  #endif
}

float Navigation::currentHeading() {
  float heading = 0;
  #if defined(USE_FAKE_LOCATION)
    heading = FAKE_HEADING;
  #else
    sensors_event_t event; 
    _mag.getEvent(&event);
    // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
    // Calculate heading when the magnetometer is level, then correct for signs of axis.
    heading = atan2(event.magnetic.y, event.magnetic.x);
  #endif
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