#ifndef Navigation_h
#define Navigation_h

#include "Arduino.h"
#include "TinyGPS++.h"
#include "Sensors.h"

struct Waypoint ; /* Forward declaration */

typedef struct Waypoint
{
  float latitude;
  float longitude;
  int radius;
  byte checksum;
} Waypoint;

enum LeftRightCenter
{
  LEFT,
  RIGHT,
  CENTER
};

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
    LeftRightCenter leftRightCenter();
  private:
    boolean validateWaypoint(Waypoint waypoint);
    boolean compareWaypoints(Waypoint wp1, Waypoint wp2);
    int waypointEepromPosition(int waypointIndex);
    int retrieveWaypointCount();
    void shiftWaypointsForward();
    byte CRC8(const byte *data, byte len);
    TinyGPSPlus gps;
    Sensors* _sensors;
    int _numberOfWaypoints;
};


#endif

