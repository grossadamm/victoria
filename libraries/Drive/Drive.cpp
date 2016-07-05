#include "Drive.h"
#include "Rudder.h"
#include "MainDrive.h"
#include "SecondaryDriveLeft.h"
#include "SecondaryDriveRight.h"
#include "Sensors.h"

const PROGMEM int SECONDARY_DRIVE_MIN_POWER = 20;

Drive::Drive(Power* power, Storage* storage, Sensors* sensors)
{
  _on = false;
  _power = power;
  _storage = storage;
  _runCount = 0;
  _rudder = new Rudder(power, sensors, storage);
  _mainDrive = new MainDrive(power, sensors);
  _secondaryDriveLeft = new SecondaryDriveLeft(power, sensors, SECONDARY_DRIVE_MIN_POWER);
  _secondaryDriveRight = new SecondaryDriveRight(power, sensors, SECONDARY_DRIVE_MIN_POWER);
  // TODO ESC CALIBRATION??
}

void Drive::off()
{
  _rudder->off();
  _mainDrive->off();
  _secondaryDriveLeft->off();
  _secondaryDriveRight->off();
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
  _mainDrive->attemptClear();
  _rudder->attemptClear();
  _secondaryDriveLeft->attemptClear();
  _secondaryDriveRight->attemptClear();
}

void Drive::direction(int leftRightCenter) { 
  if(!_mainDrive->disabled()) {
    _mainDrive->forward();
  }

  if(!_rudder->disabled() && !_mainDrive->disabled()) { // use the rudder to control
    _rudder->set(leftRightCenter);
    _secondaryDriveLeft->off();
    _secondaryDriveRight->off();
  } else {
    _secondaryDriveLeft->set(_mainDrive->disabled(), _secondaryDriveRight->disabled(), leftRightCenter);
    _secondaryDriveRight->set(_mainDrive->disabled(), _secondaryDriveLeft->disabled(), leftRightCenter);
    _rudder->off();
  }
}  

// void Drive::turnSecondaryDrive(int leftRightCenter) {
//   int maxPowerIncrease = 30;
//   int minPower = 0;
//   if(!_useMainDrive) {
//     minPower = SECONDARY_DRIVE_MIN_POWER;
//   }
//   int scaledTurn = leftRightCenter/maxPowerIncrease;
//   if(scaledTurn > 1) {
//     _leftDrive = minPower + abs(scaledTurn);
//     _rightDrive = minPower;
//   } else if(scaledTurn < -1) {
//     _rightDrive = minPower + abs(scaledTurn);
//     _leftDrive = minPower;
//   }
// }


void Drive::mainDriveEnable(bool onOff) {
  _mainDrive->enable(onOff);
}

void Drive::secondaryDriveLeftEnable(bool onOff) {
  _secondaryDriveLeft->enable(onOff);
}

void Drive::secondaryDriveRightEnable(bool onOff) {
  _secondaryDriveRight->enable(onOff);
}

void Drive::rudderEnable(bool onOff) {
  _rudder->enable(onOff);
}