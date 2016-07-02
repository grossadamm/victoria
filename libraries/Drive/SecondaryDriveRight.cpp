#include "SecondaryDriveRight.h"
#include "Sensors.h"
#include "pins.h"

SecondaryDriveRight::SecondaryDriveRight(Power* power, Sensors* sensors)
{
  _on = false;
  _power = power;
  _sensors = sensors;
}


void SecondaryDriveRight::set(bool mainDrive, bool secondaryDriveLeft, int leftRightCenter) { 

}

void SecondaryDriveRight::off()
{
  _power->secondaryDriveRight(false);
  _on = false;
}

void SecondaryDriveRight::on()
{
  _power->secondaryDriveRight(true);
  _on = true;  
}

boolean SecondaryDriveRight::isOn() {
  return _on;
}

boolean SecondaryDriveRight::isOff() {
  return !_on;
}

void SecondaryDriveRight::attemptClear() {
  
}

void SecondaryDriveRight::enable(bool onOff) {
  _enabled = onOff;
}

bool SecondaryDriveRight::disabled() {
  return _stalled || !_enabled;
}