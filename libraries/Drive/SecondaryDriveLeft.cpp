#include "SecondaryDriveLeft.h"
#include "Sensors.h"
#include "pins.h"

SecondaryDriveLeft::SecondaryDriveLeft(Power* power, Sensors* sensors, int minPower)
{
  _on = false;
  _power = power;
  _sensors = sensors;
  _minPower = minPower;
  _motor = new Servo();
}


void SecondaryDriveLeft::set(bool mainDrive, bool secondaryDriveRight, int leftRightCenter) { 
  int maxPowerIncrease = 30;
  int minPower = 0;
  int out = 0;

  if(!mainDrive) {
    minPower = _minPower;
    out = minPower;
  }
  int scaledTurn = leftRightCenter/maxPowerIncrease;
  if(scaledTurn > 1) {
    // set to 
    out = minPower + abs(scaledTurn);
  } else if(scaledTurn < -1) {
    if(!secondaryDriveRight) {
      out = 0 - (minPower + abs(scaledTurn));
    } else {
      out = minPower;  
    }
  }

  out = map(out, -180, 180, 0, 179);
  _motor->write(out);
}

void SecondaryDriveLeft::off()
{
  _motor->detach();
  _power->secondaryDriveLeft(false);
  _on = false;
}

void SecondaryDriveLeft::on()
{
  _motor->attach(LEFT_PWM);
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
  // 1/2 power forward/back
  // full power forward/back
}

void SecondaryDriveLeft::enable(bool onOff) {
  _enabled = onOff;
}

bool SecondaryDriveLeft::disabled() {
  return _stalled || !_enabled;
}