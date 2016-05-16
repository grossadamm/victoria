#include "Rudder.h"
#include "Sensors.h"

const PROGMEM int MAX_RUDDER_TURN_DEGREES = 30;
const PROGMEM int SPEED_SET_PIN = 3;
const PROGMEM int ENCODER_PIN_A = 18;
const PROGMEM int ENCODER_PIN_B = 19;
const PROGMEM int VOLTAGE_PIN = 0;

const PROGMEM int POSITION_SET_ACCURACY = 10;

Rudder::Rudder(Power* power, Sensors* sensors)
{
  _on = false;
  _startCounts = 0;
  _power = power;
  _sensors = sensors;
  _rudderSets = new RunningAverage(30);
  _rudderSets->clear();
  _encoder = new Encoder(ENCODER_PIN_A, ENCODER_PIN_B);
  _encoder->write(0);
  setStartPosition();
}

void Rudder::left() {
  on();
  _power->rudderBrake(false);
  _power->rudderDirectionForward(true);
  analogWrite(SPEED_SET_PIN, 255);
}

void Rudder::right() {
  on();
  _power->rudderBrake(false);
  _power->rudderDirectionForward(false);
  analogWrite(SPEED_SET_PIN, 255);
}

void Rudder::set(int leftRightCenter) { 
  int averagedPosition = calculateRequiredPosition(leftRightCenter);

  // set only if the running average is not within 1 degree of the current position
  _currentRudderPosition = position();
  if(averagedPosition > _currentRudderPosition + 1) {
    right();
  } else if (averagedPosition < _currentRudderPosition - 1) {
    left();
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
  time_t futureTime = 0;
  int startPosition =  _encoder->read();
  int leftPosition = 0;
  int rightPosition = 0;
  int middlePosition = 0;
  // _power->killAllButLights();
  // _power->rudderBrake(false);
  // on();
  // run left until against left block or 5 seconds whichever comes first
  on();
  int position = 0;

  left();
  do {
    position = _encoder->read();
    delay(10);
  } while(abs(position - _encoder->read()) > POSITION_SET_ACCURACY && !_sensors->timeout(futureTime, 5));
  leftPosition = position;

  // run right until against right block or 10 seconds whichever comes first
  futureTime = 0;
  right();
  do {
    position = _encoder->read();
    delay(10);
  } while(abs(position - _encoder->read()) > POSITION_SET_ACCURACY && !_sensors->timeout(futureTime, 10));
  rightPosition = position;

  // calculate middle encoder position
  int positionDifference = rightPosition - leftPosition;
  if(positionDifference > 0) {
    middlePosition = leftPosition + (positionDifference/2);
  } else {
    middlePosition = rightPosition + (positionDifference/2);
  }
  
  // move back left until within 10 points of the middle position or have moved past the middle position
  futureTime = 0;
  left();
  do {
    delay(1);
    position = _encoder->read();
    if(positionDifference > 0) {
      if(abs(position - middlePosition) < POSITION_SET_ACCURACY || position < middlePosition) {
        break;
      }
    } else {
      if(abs(position - middlePosition) < POSITION_SET_ACCURACY || position > middlePosition) {
        break;
      }
    }
  } while(!_sensors->timeout(futureTime, 15)); // attempt to get back to middle for at most 15 seconds
}