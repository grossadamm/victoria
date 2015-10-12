#ifndef Communications_h
#define Communications_h

#include "Arduino.h"
#include "Navigation.h"
#include "Sensors.h"

class Communications
{
  public:
    Communications(Navigation* nav, Sensors* sensors);
    void buildMessage(byte message[50]);
    boolean needToCommunicate();
    boolean sendMessage(byte message[50]);
  private:
    void applyTemperatures(byte message[50]);
    void applyCoordinates(byte message[50]);
    boolean communicatedToday();
    void lastCommunicatedOn(int day);
    Navigation* _nav;
    Sensors* _sensors;
};

#endif