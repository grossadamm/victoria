#ifndef MainDrive_h
#define MainDrive_h

#include "Arduino.h"
#include "Power.h"
#include "Sensors.h" 

class MainDrive
{
  public:
    MainDrive(Power* power, Sensors* sensors);
    void attemptClear();
    void forward();
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