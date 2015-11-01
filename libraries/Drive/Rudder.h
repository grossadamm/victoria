#ifndef Rudder_h
#define Rudder_h

#include "Arduino.h"
#include "Navigation.h"
#include "Servo.h"
#include "BaseMotorControl.h"
#include "RunningAverage.h"

class Rudder: public BaseMotorControl
{
  public:
    Rudder();
    void attemptClear();
    void set(int leftRightCenter);
  private:
    RunningAverage* _rudderSets;
    int _currentRudderPosition;
    Servo _directionServo;
};

#endif