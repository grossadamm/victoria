#ifndef MainDrive_h
#define MainDrive_h

#include "Arduino.h"
#include "Power.h"
#include "Sensors.h" 
#include "Servo.h"

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
};

#endif