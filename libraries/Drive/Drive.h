#ifndef Drive_h
#define Drive_h

#include "Arduino.h"
#include "Navigation.h"
#include "Rudder.h"
#include "MainDrive.h"
#include "SecondaryDriveLeft.h"
#include "SecondaryDriveRight.h"
#include "Storage.h"
#include "Sensors.h"

class Drive
{
  public:
    Drive(Power* power, Storage* storage, Sensors* sensors);
    void on();
    void off();
    void attemptClear();
    void direction(int leftRightCenter);
    void currentExceeded();
    boolean isOn();
    boolean isOff();
    void speed(int percent);
    void mainDriveEnable(bool onOff);
    void secondaryDriveLeftEnable(bool onOff);
    void secondaryDriveRightEnable(bool onOff);
    void rudderEnable(bool onOff);
  private:
    int _runCount;
    Rudder* _rudder;
    MainDrive* _mainDrive;
    SecondaryDriveRight* _secondaryDriveRight;
    SecondaryDriveLeft* _secondaryDriveLeft;
    Power* _power;
    Storage* _storage;
    boolean _on;
};

#endif