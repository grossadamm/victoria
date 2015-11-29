#ifndef Rudder_h
#define Rudder_h

#include "Arduino.h"
#include "Power.h"
#include "RunningAverage.h"
#include "Encoder.h"

class Rudder
{
  public:
    Rudder(Power* power);
    void attemptClear();
    void set(int leftRightCenter);
    int calculateRequiredPosition(int leftRightCenter);
    int position();
    void speed(int percent);
    void on();
    void off();
    boolean isOn();
    boolean isOff();
  private:
    boolean _on;
    int _startCounts;
    void setStartPosition();
    RunningAverage* _rudderSets;
    int _currentRudderPosition;
    Encoder* _encoder;
    Power* _power;
};

#endif