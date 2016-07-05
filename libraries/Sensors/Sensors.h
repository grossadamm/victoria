#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"
#include "OneWire.h"
#include "Power.h"
#include "DallasTemperature.h"

#include "Time.h"

struct Temperatures; /* Forward declaration */

typedef struct Temperatures
{
  float water;
  float air;
  float internal;
  float battery;
} Temperatures;

enum BatteryState
{
  charging,
  floating,
  draining
};


class Sensors
{
  public:
    Sensors(Power* power);
    Temperatures retrieveTemperatures();
    bool night();
    bool day();
    bool batteryAbove(int percent);
    bool batteryBelow(int percent);
    bool currentAbove15Amps();
    bool storming();
    bool timeout(time_t& futureTime, int seconds);
    BatteryState batteryState();
    int currentDrawLeft();
    int currentDrawRight();
    int currentDrawCenter();
  private:
    Power* _power;
    int _one_wire_bus;
    int _when_times_out;
};

#endif