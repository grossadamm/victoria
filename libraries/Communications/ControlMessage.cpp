#include "ControlMessage.h"

ControlMessage::ControlMessage(char message[32]) {
  for(int i = 0; i < 32; i++) {
    _message[i] = message[i];
  }
  _startIndex = 0;
  _endIndex = 0;
  setNextEndIndex();
}

void ControlMessage::setNextEndIndex() {
  for(int i = _startIndex+1; i<32; i++){
    if(_message[i] == '$') {
      _endIndex = i;
      return;
    }
  }
  _startIndex = 33;
  _endIndex = _startIndex;
}

bool ControlMessage::commandsAvailable() {
  if(_endIndex != _startIndex) 
    return true;
  return false;
}

Command ControlMessage::getCommand() {
  char* data = new char[_endIndex - _startIndex];
  char command = _message[_startIndex];
  if(_endIndex == _startIndex + 1) {
    data = 0;
  } else {
    int dataCounter = 0;
    for(int i = _startIndex + 1; i<_endIndex; i++) {
      data[dataCounter] = _message[i];
      dataCounter++;
    }
  }
  
  Command cmd = { command, data };

  _startIndex = _endIndex + 1;
  setNextEndIndex();
  return cmd;
}