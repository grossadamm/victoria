#ifndef Rudder_h
#define Rudder_h

#include "Arduino.h"
#include "Power.h"
#include "RunningAverage.h"
#include "Encoder.h"
#include "Sensors.h"

class Rudder
{
  public:
    Rudder(Power* power, Sensors* sensors);
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
    void left();
    void right();
    RunningAverage* _rudderSets;
    int _currentRudderPosition;
    Encoder* _encoder;
    Power* _power;
    Sensors* _sensors;
};

#endif