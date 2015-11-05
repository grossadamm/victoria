#include "ControlMessage.h"

ControlMessage::ControlMessage(char message[32]) {
  _message = message;
  _message = strtok(_message, "$");
}

bool ControlMessage::commandsAvailable() {
  if(_message != 0) 
    return true;
  return false;
}

Command ControlMessage::getCommand() {
  char* data = new char[sizeof(_message)-1];
  for(int i = 0; i<sizeof(_message); i++) {
    data[i] = _message[i+1];
  }
  Command cmd = { _message[0], data };
  _message = strtok(0, "$");
  return cmd;
}