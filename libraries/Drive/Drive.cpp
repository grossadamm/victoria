#include "Drive.h"
#include "Rudder.h"

const PROGMEM int MAIN_DRIVE_MOSFET = 31;
const PROGMEM int SECONDARY_DRIVE_MOSFET = 32;

const PROGMEM int MAIN_DRIVE_MIN_POWER = 20;
const PROGMEM int SECONDARY_DRIVE_MIN_POWER = 20;

Drive::Drive(Power* power)
{
  _on = false;
  _power = power;
  _useMainDrive = true;
  _useRudder = true;
  _useSecondaryDrive = false;
  _rudder = new Rudder(power);
}

void Drive::off()
{
  _rudder->off();
  digitalWrite(MAIN_DRIVE_MOSFET, LOW);
  digitalWrite(SECONDARY_DRIVE_MOSFET, LOW);
  _on = false;
}

void Drive::on()
{
  if(isOff()) {
    attemptClear();
    if(_useRudder)
      _rudder->on();
    if(_useMainDrive){
      digitalWrite(MAIN_DRIVE_MOSFET, HIGH);
      _mainDrive = MAIN_DRIVE_MIN_POWER;
    }
    if(_useSecondaryDrive){
      digitalWrite(SECONDARY_DRIVE_MOSFET, HIGH);
      _leftDrive = SECONDARY_DRIVE_MIN_POWER;
      _rightDrive = SECONDARY_DRIVE_MIN_POWER;
    }
  }
  // turn on
  _on = true;
}

boolean Drive::isOn() {
  return _on;
}

boolean Drive::isOff() {
  return !_on;
}

void Drive::attemptClear()
{
  // run reverse
  // wait half a second
  // run forwards
  // wait half a second
  // adjust rudder
}

void Drive::direction(int leftRightCenter) { 
  if(_useRudder) { // use the rudder to control
    _rudder->set(leftRightCenter);
  } else if(_useSecondaryDrive) { // rudder stalled, so use secondary drive
    turnSecondaryDrive(leftRightCenter);
  } else {
    off(); // TODO high risk point
  }
}

void Drive::currentExceeded() {
  off();
  if(_useMainDrive) {
    _useMainDrive = false;
    _useSecondaryDrive = true;
  } else {
    _useMainDrive = true;
    _useSecondaryDrive = false;
  }
}

void Drive::turnSecondaryDrive(int leftRightCenter) {
  int maxPowerIncrease = 30;
  int minPower = 0;
  if(!_useMainDrive) {
    minPower = SECONDARY_DRIVE_MIN_POWER;
  }
  int scaledTurn = leftRightCenter/maxPowerIncrease;
  if(scaledTurn > 1) {
    _leftDrive = minPower + abs(scaledTurn);
    _rightDrive = minPower;
  } else if(scaledTurn < -1) {
    _rightDrive = minPower + abs(scaledTurn);
    _leftDrive = minPower;
  }
}

void Drive::speed(int percent) {
  // TODO do nothing for now
}