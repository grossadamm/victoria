#ifndef Communications_h
#define Communications_h

#include "Arduino.h"
#include "Navigation.h"
#include "Sensors.h"
#include <RF24.h>

struct ManualControlData; /* Forward declaration */

typedef struct ManualControlData
{
  uint8_t forwardReverse;
  uint8_t leftRightCenter;
} ManualControlData;

class Communications
{
  public:
    Communications(Navigation* nav, Sensors* sensors);
    void buildMessage(byte message[50]);
    boolean needToCommunicate();
    boolean sendMessage(byte message[50]);
    ManualControlData readControlData();
  private:
    void applyTemperatures(byte message[50]);
    void applyCoordinates(byte message[50]);
    boolean communicatedToday();
    void lastCommunicatedOn(int day);
    Navigation* _nav;
    Sensors* _sensors;
    bool _rfEnabled;
    RF24* _radio;
    ManualControlData _lastControlData;
};

#endif