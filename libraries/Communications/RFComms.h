#ifndef RFComms_h
#define RFComms_h

#include "Arduino.h"
#include "Navigation.h"
#include <RF24.h>
#include "Power.h"

class RFComms
{
  public:
    RFComms(Power* power);
    boolean needToCommunicate();
    boolean sendMessage(byte message[50]);
    void readMessage(char buffer[]);
    boolean dataAvailable();
    void on();
    void off();
    boolean isOn();
  private:
    RF24* _radio;
    Power* _power;
    boolean _on;
};

#endif