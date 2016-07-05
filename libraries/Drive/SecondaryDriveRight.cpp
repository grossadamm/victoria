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
  int maxPowerIncrease = 30;
  int minPower = 0;
  int out = 0;

  if(!mainDrive) {
    minPower = _minPower;
    out = minPower;
  }
  int scaledTurn = leftRightCenter/maxPowerIncrease;
  if(scaledTurn > 1) {
    if(!secondaryDriveLeft) {
      out = 0 - (minPower + abs(scaledTurn));
    } else {
      out = minPower;  
    }
  } else if(scaledTurn < -1) {
    out = minPower + abs(scaledTurn);
  }

  // TODO set out
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