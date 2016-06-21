#include "Drive.h"
#include "Rudder.h"
#include "Sensors.h"

const PROGMEM int MAIN_DRIVE_MIN_POWER = 20;
const PROGMEM int SECONDARY_DRIVE_MIN_POWER = 20;

Drive::Drive(Power* power, Storage* storage, Sensors* sensors)
{
  _on = false;
  _power = power;
  _storage = storage;
  _useMainDrive = true;
  _useRudder = true;
  _useSecondaryDrive = false;
  _runCount = 0;
  _rudder = new Rudder(power, sensors, storage);
}

void Drive::off()
{
  _rudder->off();
  _power->mainDrive(false);
  _power->secondaryDrive(false);
  _on = false;
}

void Drive::on()
{
  if(isOff()) {
    _runCount++;
    if(_runCount > _storage->motorTestRunFrequency()){
      attemptClear();
      _runCount = 0;
    }
    if(_useRudder)
      _rudder->on();
    if(_useMainDrive){
      _power->mainDrive(true);
      _mainDrive = MAIN_DRIVE_MIN_POWER;
    }
    if(_useSecondaryDrive){
      _power->secondaryDrive(true);
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
  } else { // TODO handle separate secondary drives
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

void Drive::speed(int percent) { // positive negative for forward/reverse
  // TODO do nothing for now
}

void Drive::mainDrive(bool onOff) {
  _useMainDrive = onOff;
  _power->mainDrive(onOff);
}

void Drive::secondaryDrive(bool onOff) {
  _useSecondaryDrive = onOff;
  _power->secondaryDrive(onOff);
}

void Drive::rudder(bool onOff) {
  _useRudder = onOff;
  if(onOff){
    _rudder->on();
  } else {
    _rudder->off();
  }
}