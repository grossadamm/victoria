#ifndef SecondaryDriveLeft_h
#define SecondaryDriveLeft_h

#include "Arduino.h"
#include "Power.h"
#include "Sensors.h" 
#include "Servo.h"

class SecondaryDriveLeft
{
  public:
    SecondaryDriveLeft(Power* power, Sensors* sensors, int minPower);
    void attemptClear();
    void set(bool mainDrive, bool secondaryDriveRight, int leftRightCenter);
    void enable(bool onOff);
    bool disabled();
    void on();
    void off();
    void stalled();
    bool isOn();
    bool isOff();
    bool tooMuchCurrentDraw();
  private:
    bool _on;
    bool _stalled;
    bool _enabled;
    Power* _power;
    Sensors* _sensors;
    Servo* _motor;
    int _minPower;
};

#endif