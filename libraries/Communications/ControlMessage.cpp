#include "ControlMessage.h"

ControlMessage::ControlMessage(byte message[32]) {
  _message = message;
}

ManualControlData ControlMessage::getCommand() {
  ManualControlData mcd = {0, 0};
  return mcd;
}