#ifndef Drive_h
#define Drive_h

#include "Arduino.h"
#include "Navigation.h"
#include "Servo.h"

class Drive
{
  public:
    Drive();
    void on();
    void off();
    void attemptClear();
    boolean isOn();
    boolean isOff();
    void direction(int leftRightCenter);
    void currentExceeded();
  private:
    void turnRudder(int leftRightCenter);
    void turnSecondaryDrive(int leftRightCenter);
    boolean _on;
    boolean _useMainDrive;
    boolean _useRudder;
    boolean _useSecondaryDrive;
    Servo _directionServo;
};

#endif