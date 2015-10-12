#include "Drive.h"

const PROGMEM int MAX_RUDDER_TURN_DEGREES = 30;

Drive::Drive()
{
  _on = false;
}

void Drive::off()
{
  // turn off
}

void Drive::on()
{
  if(isOff()) {
    attemptClear();
  }
  // turn on
  _on = true;
}

void Drive::attemptClear()
{
  // run reverse
  // wait half a second
  // run forwards
  // wait half a second
  // adjust rudder
}

boolean Drive::isOn() {
  return _on;
}

boolean Drive::isOff() {
  return !_on;
}

void Drive::direction(int leftRightCenter) { 
  int rudderTurn = 0;
  // left is negative
  // right is positive

  // limit rudder turn
  if(leftRightCenter > 0) {
    rudderTurn = min(MAX_RUDDER_TURN_DEGREES, leftRightCenter);
  } else {
    rudderTurn = max(0-MAX_RUDDER_TURN_DEGREES, leftRightCenter);
  }

  // set rudder
}