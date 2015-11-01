#include "Rudder.h"
#include "RunningAverage.h"

const PROGMEM int MAX_RUDDER_TURN_DEGREES = 30;


const PROGMEM int DIRECTION_SERVO_PIN = 40;
const PROGMEM int DIRECTION_SERVO_MOSFET = 30;


Rudder::Rudder() : BaseMotorControl(DIRECTION_SERVO_MOSFET, DIRECTION_SERVO_PIN)
{
  _directionServo.attach(_controlPin);
  _rudderSets = new RunningAverage(30);
  _rudderSets->clear();
}

void Rudder::set(int leftRightCenter) { 
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
  
  // smooth the rudder sets
  _rudderSets->addValue(rudderTurn);
  int averagePosition = _rudderSets->getAverage();

  // set if needed
  _currentRudderPosition = _directionServo.read();
  if(averagePosition != _currentRudderPosition) {
    Serial.print("Adjusting rudder to: "); Serial.println(averagePosition);
    on();
    _directionServo.write(rudderTurn);
  } else {
    off();
  }
  // TODO watch servo for stall
}