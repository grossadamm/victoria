#ifndef Drive_h
#define Drive_h

#include "Arduino.h"
#include "Navigation.h"

class Drive
{
  public:
    Drive();
    void on();
    void off();
    void attemptClear();
    boolean isOn();
    boolean isOff();
    void direction(LeftRightCenter leftRightCenter);
  private:
};

#endif