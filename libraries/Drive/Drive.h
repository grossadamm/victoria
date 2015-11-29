#ifndef Drive_h
#define Drive_h

#include "Arduino.h"
#include "Navigation.h"
#include "Rudder.h"

class Drive
{
  public:
    Drive(Power* power);
    void on();
    void off();
    void attemptClear();
    void direction(int leftRightCenter);
    void currentExceeded();
    boolean isOn();
    boolean isOff();
    void speed(int percent);
  private:
    void turnRudder(int leftRightCenter);
    void turnSecondaryDrive(int leftRightCenter);
    boolean _useMainDrive;
    boolean _useRudder;
    boolean _useSecondaryDrive;
    int _mainDrive;
    int _leftDrive;
    int _rightDrive;
    Rudder* _rudder;
    Power* _power;
    boolean _on;
};

#endif