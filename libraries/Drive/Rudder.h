#ifndef Rudder_h
#define Rudder_h

#include "Arduino.h"
#include "Power.h"
#include "RunningAverage.h"
#include "Encoder.h"
#include "Sensors.h" 
#include "Storage.h" 

class Rudder
{
  public:
    Rudder(Power* power, Sensors* sensors, Storage* storage);
    void attemptClear();
    void set(int leftRightCenter);
    int calculateRequiredPosition(int leftRightCenter);
    int degreesOffCenter();
    void speed(int percent);
    void on();
    void off();
    void stalled();
    boolean isOn();
    boolean isOff();
  private:
    boolean _on;
    boolean _stalled;
    int _startCounts;
    void setStartPosition();
    int smoothLeftRightCenter(int leftRightCenter);
    bool stillMoving(int previousPosition, int currentPosition);
    void left();
    void right();
    void stop();
    int zeroPosition();
    void zeroPosition(int zeroPosition);
    RunningAverage* _rudderSets;
    int _currentRudderPosition;
    int _zeroPosition;
    Encoder* _encoder;
    Power* _power;
    Sensors* _sensors;
    Storage* _storage;
};

#endif