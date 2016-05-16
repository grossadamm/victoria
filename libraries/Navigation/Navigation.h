#ifndef Navigation_h
#define Navigation_h

#include "Arduino.h"
#include "TinyGPS++.h"
#include "Sensors.h"
#include <Adafruit_HMC5883_U.h>
#include "Storage.h"

class Navigation
{
  public:
    Navigation(Sensors* sensors, Storage* storage, Power* power);
    boolean ready();
    void resetWaypoints();
    void printWaypoint(Waypoint waypoint);
    Waypoint setWaypointChecksum(Waypoint waypoint);
    int courseChangeNeeded();
    float currentHeading();
    double lat();
    double lng();
    byte CRC8(const byte *data, byte len);
    void pushNewWaypoint(Waypoint waypoint);
    void setWaypoints(Waypoint *waypoints);
  private:
    boolean validateWaypoint(Waypoint waypoint);
    boolean compareWaypoints(Waypoint wp1, Waypoint wp2);
    void shiftWaypointsForward();
    void shiftWaypointsBackward();
    TinyGPSPlus _gps;
    Sensors* _sensors;
    Storage* _storage;
    Power* _power;
    Waypoint _currentWaypoint;
    Adafruit_HMC5883_Unified _mag;
    float _currentDeclination;
};


#endif

