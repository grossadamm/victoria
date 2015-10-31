#ifndef Navigation_h
#define Navigation_h

#include "Arduino.h"
#include "TinyGPS++.h"
#include "Sensors.h"
#include <Adafruit_HMC5883_U.h>

struct Waypoint ; /* Forward declaration */

typedef struct Waypoint
{
  float latitude;
  float longitude;
  int radiusHectometers;
  byte checksum;
} Waypoint;

class Navigation
{
  public:
    Navigation(Sensors* sensors);
    boolean ready();
    void resetWaypoints();
    void printWaypoint(Waypoint waypoint);
    void appendNewWaypoint(Waypoint& waypoint);
    void storeWaypoint(Waypoint& waypoint, int waypointIndex);
    void storeWaypoint(const Waypoint& waypoint, int waypointIndex);
    Waypoint retrieveWaypoint(int waypointIndex);
    int courseChangeNeeded();
    float currentHeading();
  private:
    boolean validateWaypoint(Waypoint waypoint);
    boolean compareWaypoints(Waypoint wp1, Waypoint wp2);
    int waypointEepromPosition(int waypointIndex);
    int retrieveWaypointCount();
    void shiftWaypointsForward();
    boolean validateEeprom();
    byte CRC8(const byte *data, byte len);
    TinyGPSPlus _gps;
    Sensors* _sensors;
    int _numberOfWaypoints;
    Waypoint _currentWaypoint;
    Adafruit_HMC5883_Unified _mag;
    float _currentDeclination;
};


#endif

