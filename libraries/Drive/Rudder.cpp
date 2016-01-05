#include "Rudder.h"

const PROGMEM int MAX_RUDDER_TURN_DEGREES = 30;
const PROGMEM int SPEED_SET_PIN = 3;
const PROGMEM int ENCODER_PIN_A = 18;
const PROGMEM int ENCODER_PIN_B = 19;

Rudder::Rudder(Power* power)
{
  _on = false;
  _startCounts = 0;
  _power = power;
  _rudderSets = new RunningAverage(30);
  _rudderSets->clear();
  _encoder = new Encoder(ENCODER_PIN_A, ENCODER_PIN_B);
  _encoder->write(0);
  setStartPosition();
}

void Rudder::set(int leftRightCenter) { 
  int averagedPosition = calculateRequiredPosition(leftRightCenter);

  // set only if the running average is not within 1 degree of the current position
  _currentRudderPosition = position();
  if(averagedPosition > _currentRudderPosition + 1) {
    // Serial.println("Rudder right");
    on();
    _power->rudderBrake(false);
    _power->rudderDirectionForward(false);
    analogWrite(SPEED_SET_PIN, 255);
  } else if (averagedPosition < _currentRudderPosition - 1) {
    // Serial.println("Rudder left");
    on();
    _power->rudderBrake(false);
    _power->rudderDirectionForward(true);
    analogWrite(SPEED_SET_PIN, 255);
  } else {
    if(isOn())
      Serial.println("Rudder ok");
    off();
    _power->rudderBrake(true);
    _power->rudderDirectionForward(false);
    analogWrite(SPEED_SET_PIN, 0);
  }
  // TODO watch servo for stall
}

void Rudder::off()
{
  _power->rudder(false);
  _on = false;
}

void Rudder::on()
{
  if(isOff()) {
    _startCounts++;
    _power->rudder(true);
    if(_startCounts > 10) {
      _startCounts = 0;
      setStartPosition();
    }
  }
  // turn on
  _on = true;
}

boolean Rudder::isOn() {
  return _on;
}

boolean Rudder::isOff() {
  return !_on;
}

int Rudder::position() {
  return _encoder->read()/350;
}

int Rudder::calculateRequiredPosition(int leftRightCenter) {
 int rudderTurn = 0;
  // left is negative
  // right is positive

  // limit rudder turn
  if(leftRightCenter > 0) {
    rudderTurn = min(MAX_RUDDER_TURN_DEGREES, leftRightCenter);
  } else {
    rudderTurn = max(0-MAX_RUDDER_TURN_DEGREES, leftRightCenter);
  }
  
  // smooth the rudder sets
  _rudderSets->addValue(rudderTurn);
  return _rudderSets->getAverage();
}

void Rudder::setStartPosition() {
  // power stuff off
  // move left as far as possible
  // set counts to that spot
  // move right as far as possible
  // verify counts close
  // move to 0 position
}