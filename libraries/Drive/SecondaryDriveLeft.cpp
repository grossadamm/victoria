#include "SecondaryDriveLeft.h"
#include "Sensors.h"
#include "pins.h"

SecondaryDriveLeft::SecondaryDriveLeft(Power* power, Sensors* sensors)
{
  _on = false;
  _power = power;
  _sensors = sensors;
}


void SecondaryDriveLeft::set(bool mainDrive, bool secondaryDriveRight, int leftRightCenter) { 

}

void SecondaryDriveLeft::off()
{
  _power->secondaryDriveLeft(false);
  _on = false;
}

void SecondaryDriveLeft::on()
{
  _power->secondaryDriveLeft(true);
  _on = true;  
}

boolean SecondaryDriveLeft::isOn() {
  return _on;
}

boolean SecondaryDriveLeft::isOff() {
  return !_on;
}

void SecondaryDriveLeft::attemptClear() {
  
}

void SecondaryDriveLeft::enable(bool onOff) {
  _enabled = onOff;
}

bool SecondaryDriveLeft::disabled() {
  return _stalled || !_enabled;
}