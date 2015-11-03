#ifndef ControlMessage_h
#define ControlMessage_h

#include "Arduino.h"
#include "Communications.h"

class ControlMessage
{
  public:
    ControlMessage(byte message[32]);
    ManualControlData getCommand();
  private:
    byte* _message;
};

#endif