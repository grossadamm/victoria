#include "SecondaryDriveRight.h"
#include "Sensors.h"
#include "pins.h"

const PROGMEM int MAX_CURRENT_DRAW = 10; // TODO what should this be if stalled, also if lost prop?

SecondaryDriveRight::SecondaryDriveRight(Power* power, Sensors* sensors, int minPower)
{
  _on = false;
  _power = power;
  _sensors = sensors;
  _minPower = minPower;
  _motor = new Servo();
}


void SecondaryDriveRight::set(bool mainDrive, bool secondaryDriveLeft, int leftRightCenter) { 
  if(isOn() && tooMuchCurrentDraw()) {
    _stalled = true;
  }
  if(disabled()){
    return;
  }
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

  out = map(out, -180, 180, 0, 179);
  on();
  _motor->write(out);
}

void SecondaryDriveRight::off()
{
  _motor->detach();
  _power->secondaryDriveRight(false);
  _on = false;
}

void SecondaryDriveRight::on()
{
  _motor->attach(RIGHT_PWM);
  _power->secondaryDriveRight(true);
  _on = true;  
}

bool SecondaryDriveRight::isOn() {
  return _on;
}

bool SecondaryDriveRight::isOff() {
  return !_on;
}

void SecondaryDriveRight::attemptClear() {
  on();
  _motor->write(map(-20, -100, 100, 0, 179));
  delay(2000);
  _motor->write(map(0, -100, 100, 0, 179));
  delay(200);
  _motor->write(map(20, -100, 100, 0, 179));
  delay(2000);
  _motor->write(map(0, -100, 100, 0, 179));
  delay(200);
  _motor->write(map(-100, -100, 100, 0, 179));
  delay(2000);
  _motor->write(map(0, -100, 100, 0, 179));
  delay(200);
  _motor->write(map(100, -100, 100, 0, 179));
  delay(2000);
  _stalled = false;
  off();
}

void SecondaryDriveRight::enable(bool onOff) {
  _enabled = onOff;
}

bool SecondaryDriveRight::disabled() {
  return _stalled || !_enabled;
}

bool SecondaryDriveRight::tooMuchCurrentDraw() {
  _sensors->currentDrawRight() > MAX_CURRENT_DRAW;
}