#ifndef ControlMessage_h
#define ControlMessage_h

#include "Arduino.h"
#include "Command.h"

class ControlMessage
{
  public:
    ControlMessage(char message[32]);
    Command getCommand();
    bool commandsAvailable();
  private:
    char* _message;
};

#endif