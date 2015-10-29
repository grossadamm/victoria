#include "Drive.h"

const PROGMEM int MAX_RUDDER_TURN_DEGREES = 30;
const PROGMEM int DIRECTION_SERVO_PIN = 40;
const PROGMEM int DIRECTION_SERVO_MOSFET = 30;
const PROGMEM int MAIN_DRIVE_MOSFET = 31;
const PROGMEM int SECONDARY_DRIVE_MOSFET = 32;

const PROGMEM int MAIN_DRIVE_MIN_POWER = 20;
const PROGMEM int SECONDARY_DRIVE_MIN_POWER = 20;

Drive::Drive()
{
  _on = false;
  _directionServo.attach(DIRECTION_SERVO_PIN);
  _useMainDrive = true;
  _useRudder = true;
  _useSecondaryDrive = false;
}

void Drive::off()
{
  digitalWrite(DIRECTION_SERVO_MOSFET, LOW);
  digitalWrite(MAIN_DRIVE_MOSFET, LOW);
  digitalWrite(SECONDARY_DRIVE_MOSFET, LOW);
  _on = false;
}

void Drive::on()
{
  if(isOff()) {
    attemptClear();
    if(_useRudder)
      digitalWrite(DIRECTION_SERVO_MOSFET, HIGH);
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

void Drive::attemptClear()
{
  // run reverse
  // wait half a second
  // run forwards
  // wait half a second
  // adjust rudder
}

boolean Drive::isOn() {
  return _on;
}

boolean Drive::isOff() {
  return !_on;
}

void Drive::direction(int leftRightCenter) { 
  if(_useRudder) { // use the rudder to control
    turnRudder(leftRightCenter);
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

void Drive::turnRudder(int leftRightCenter) {
  int rudderTurn = 0;
  // left is negative
  // right is positive

  // limit rudder turn
  if(leftRightCenter > 0) {
    rudderTurn = min(MAX_RUDDER_TURN_DEGREES, leftRightCenter);
  } else {
    rudderTurn = max(0-MAX_RUDDER_TURN_DEGREES, leftRightCenter);
  }

  //scale rudder by 90 so that 0-180 is the servo degrees
  rudderTurn += 90;
  // set rudder
  _directionServo.write(rudderTurn);

  // TODO watch servo for stall
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