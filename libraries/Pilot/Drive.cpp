#include "Drive.h"

Drive::Drive()
{
}

void Drive::off()
{
  // turn off
}

void Drive::on()
{
  if(isOff()) {
    attemptClear();
  }
  // turn on
  
}

void Drive::attemptClear()
{
  
}

boolean Drive::isOn() {
  return true;
}

boolean Drive::isOff() {
  return false;
}

void Drive::direction(LeftRightCenter leftRightCenter) {
  
}