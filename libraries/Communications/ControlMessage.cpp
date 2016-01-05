#include "ControlMessage.h"

const PROGMEM char COMMAND_END_CHAR = '$';

ControlMessage::ControlMessage(char message[32]) {
  for(int i = 0; i < 32; i++) {
    _message[i] = message[i];
  }
  _first = true;
}

ControlMessage::~ControlMessage() {
  delete[] _message;
}

void ControlMessage::setNextEndIndex() {
  
}

bool ControlMessage::commandsAvailable() {
  if(_first){
    _messageContinues = strtok(_message, "$");
    _first = false;
  } else {
    _messageContinues = strtok(NULL, "$");  
  }
  return _messageContinues != NULL;
}

Command ControlMessage::getCommand() {
  char *data = new char[sizeof(_messageContinues) - 1];
  Command command;
  command.command = _messageContinues[0];
  command.data = data;
  for(int i = 0; i<sizeof(_messageContinues); i++){
    command.data[i] = _messageContinues[i+1];
  }
  return command;
}

// void ControlMessage::getCurrentCommandData() {}