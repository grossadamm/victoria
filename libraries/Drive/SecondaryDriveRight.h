#ifndef SecondaryDriveRight_h
#define SecondaryDriveRight_h

#include "Arduino.h"
#include "Power.h"
#include "Sensors.h" 

class SecondaryDriveRight
{
  public:
    SecondaryDriveRight(Power* power, Sensors* sensors);
    void attemptClear();
    void set(bool mainDrive, bool secondaryDriveLeft, int leftRightCenter);
    void enable(bool onOff);
    bool disabled();
    void on();
    void off();
    void stalled();
    boolean isOn();
    boolean isOff();
  private:
    boolean _on;
    boolean _stalled;
    boolean _enabled;
    Power* _power;
    Sensors* _sensors;
};

#endif