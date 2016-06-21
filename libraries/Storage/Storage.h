#ifndef Storage_h
#define Storage_h

#include "Arduino.h"

struct Waypoint ; /* Forward declaration */

typedef struct Waypoint
{
  float latitude;
  float longitude;
  int radiusHectometers;
  byte checksum;
} Waypoint;

class Storage
{
  public:
    Storage();
    void lastCommunicatedOn(int day);
    int lastCommunicatedOn();

    void communicationIntervalDays(int days);
    int communicationIntervalDays();

    void motorTestRunFrequency(int runsBetweenTest);
    int motorTestRunFrequency();

    void rudderZeroPosition(int rudderZeroPosition);
    int rudderZeroPosition();

    void AppendNewWaypoint(Waypoint waypoint);
    void StoreWaypoint(Waypoint waypoint, int waypointNumber);
    Waypoint RetrieveWaypoint(int waypointNumber);
    void SetWaypointCount(int count);
    int ReadWaypointCount();
  private:
    int waypointEepromPosition(int waypointIndex);
    boolean validateWaypoint(Waypoint waypoint);
    byte CRC8(const byte *data, byte len);
    int _numberOfWaypoints;
};

#endif