#include "BaseMotorControl.h"
#include "Arduino.h"

BaseMotorControl::BaseMotorControl(int mosfet, int control)
{
  _on = false;
  _mosfetPin = mosfet;
  _controlPin = control;
}

void BaseMotorControl::off()
{
  digitalWrite(_mosfetPin, LOW);
  _on = false;
}

void BaseMotorControl::on()
{
  if(isOff()) {
    digitalWrite(_mosfetPin, HIGH);
    attemptClear();
  }
  // turn on
  _on = true;
}

void BaseMotorControl::attemptClear()
{
  // run reverse
  // wait half a second
  // run forwards
  // wait half a second
  // adjust rudder
}

boolean BaseMotorControl::isOn() {
  return _on;
}

boolean BaseMotorControl::isOff() {
  return !_on;
}