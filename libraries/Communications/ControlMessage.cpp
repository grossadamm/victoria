#include "ControlMessage.h"

const PROGMEM char COMMAND_END_CHAR = '$';

ControlMessage::ControlMessage(char message[32]) {
  for(int i = 0; i < 32; i++) {
    _message[i] = message[i];
  }
  _startIndex = 0;
  _endIndex = 0;
  setNextEndIndex();
}

ControlMessage::~ControlMessage() {
  delete[] _message;
}

void ControlMessage::setNextEndIndex() {
  int startedAt = _endIndex;
  for(int i = _startIndex+1; i<32; i++){
    if(_message[i] == COMMAND_END_CHAR) {
      _endIndex = i;
      break;
    }
  }
  if(startedAt == _endIndex) {
    _startIndex = 33;
    _endIndex = _startIndex;
  }
}

bool ControlMessage::commandsAvailable() {
  if(_endIndex != _startIndex) 
    return true;
  return false;
}

Command ControlMessage::getCommand() {
  char* data = new char[_endIndex - _startIndex];
  char command = _message[_startIndex];

  int dataCounter = 0;
  for(int i = _startIndex + 1; i<_endIndex; i++) {
    data[dataCounter] = _message[i];
    dataCounter++;
  }

  _currentCommand.command = command;
  _currentCommand.data = data;
  if(data)
    delete[] data;

  _startIndex = _endIndex + 1;
  setNextEndIndex();
  return _currentCommand;
}

// void ControlMessage::getCurrentCommandData() {}