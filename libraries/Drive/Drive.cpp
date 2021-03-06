#include "Drive.h"
#include "Rudder.h"
#include "MainDrive.h"
#include "SecondaryDriveLeft.h"
#include "SecondaryDriveRight.h"
#include "Sensors.h"

const PROGMEM int SECONDARY_DRIVE_MIN_POWER = 20;

Drive::Drive(Power* power, Storage* storage, Sensors* sensors)
{
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
}

boolean Drive::isOn() {
  return _mainDrive->isOn() || _rudder->isOn() || _secondaryDriveLeft->isOn() || _secondaryDriveRight->isOn();
}

boolean Drive::isOff() {
  return !isOn();
}

void Drive::attemptClear()
{
  _mainDrive->attemptClear();
  _rudder->attemptClear();
  _secondaryDriveLeft->attemptClear();
  _secondaryDriveRight->attemptClear();
}

void Drive::direction(int leftRightCenter) { 
  on();
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