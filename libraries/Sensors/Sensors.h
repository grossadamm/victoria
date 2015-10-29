#ifndef Sensors_h
#define Sensors_h

#include "Arduino.h"
#include "OneWire.h"
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
  draining,
  reloading
};


class Sensors
{
  public:
    Sensors();
    Temperatures retrieveTemperatures();
    boolean night();
    boolean day();
    boolean batteryAbove(int percent);
    boolean batteryBelow(int percent);
    boolean currentAbove15Amps();
    boolean storming();
    boolean timeout(time_t& futureTime, int seconds);
    BatteryState batteryState();
  private:
    int _one_wire_bus;
    int _when_times_out;
};

#endif