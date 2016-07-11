#ifndef Power_h
#define Power_h

#include "Arduino.h"

class Power
{
  public:
    Power();
    void gps(boolean onOff);
    void gpsComms(boolean onOff);
    void lights(boolean onOff);
    void mainDrive(boolean onOff);
    void rfComms(boolean onOff);
    void rtc(boolean onOff);
    void rudder(boolean onOff);
    void rudderDirectionForward(boolean onOff);
    void rudderBrake(boolean onOff);
    void secondaryDriveLeft(boolean onOff);
    void secondaryDriveRight(boolean onOff);
    void temps(boolean onOff);
    void photocell(boolean onOff);

    void killAllButLights();
};

#endif