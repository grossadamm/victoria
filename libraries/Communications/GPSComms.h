#ifndef GPSComms_h
#define GPSComms_h

#include "Arduino.h"
#include "Navigation.h"

class GPSComms
{
  public:
    GPSComms(Power* power, Storage* storage);
    boolean needToCommunicate();
    boolean sendMessage(byte message[50]);
    void readMessage(byte message[50]);
  private:
    boolean communicatedToday();
    void lastCommunicatedOn(int day);
    Power* _power;
    Storage* _storage;
    byte* _lastMessageReceived;
};

#endif