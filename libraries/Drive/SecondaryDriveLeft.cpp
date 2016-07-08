#include "SecondaryDriveLeft.h"
#include "Sensors.h"
#include "pins.h"

const PROGMEM int MAX_CURRENT_DRAW = 10; // TODO what should this be if stalled, also if lost prop?

SecondaryDriveLeft::SecondaryDriveLeft(Power* power, Sensors* sensors, int minPower)
{
  _on = false;
  _power = power;
  _sensors = sensors;
  _minPower = minPower;
  _motor = new Servo();
}


void SecondaryDriveLeft::set(bool mainDrive, bool secondaryDriveRight, int leftRightCenter) { 
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
    out = minPower + abs(scaledTurn);
  } else if(scaledTurn < -1) {
    if(!secondaryDriveRight) {
      out = 0 - (minPower + abs(scaledTurn));
    } else {
      out = minPower;  
    }
  }

  out = map(out, -180, 180, 0, 179);
  on();
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

bool SecondaryDriveLeft::isOn() {
  return _on;
}

bool SecondaryDriveLeft::isOff() {
  return !_on;
}

void SecondaryDriveLeft::attemptClear() {
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

void SecondaryDriveLeft::enable(bool onOff) {
  _enabled = onOff;
}

bool SecondaryDriveLeft::disabled() {
  return _stalled || !_enabled;
}

bool SecondaryDriveLeft::tooMuchCurrentDraw() {
  _sensors->currentDrawLeft() > MAX_CURRENT_DRAW;
}