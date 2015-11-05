#ifndef ControlMessage_h
#define ControlMessage_h

#include "Arduino.h"
#include "Communications.h"

struct Command; /* Forward declaration */

typedef struct Command
{
  char command;
  char* data;
} Command;

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