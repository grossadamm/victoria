// #include "Arduino.h"
#include "Power.h"
#include "Arduino.h"

const PROGMEM int RUDDER_BRAKE_ENABLE_PIN = 9;
const PROGMEM int RUDDER_DIRECTION_PIN = 12;
const PROGMEM int RTC_MOSFET = 23;
const PROGMEM int GPS_MOSFET = 24;
const PROGMEM int RUDDER_MOSFET = 30;

Power::Power()
{
  pinMode(GPS_MOSFET, OUTPUT);
  pinMode(RTC_MOSFET, OUTPUT);
  pinMode(RUDDER_DIRECTION_PIN, OUTPUT);
  pinMode(RUDDER_MOSFET, OUTPUT);
  pinMode(RUDDER_BRAKE_ENABLE_PIN, OUTPUT);
}

void Power::gps(boolean onOff) {
  digitalWrite(GPS_MOSFET, onOff ? HIGH : LOW);
}

void Power::rtc(boolean onOff) {
  digitalWrite(RTC_MOSFET, onOff ? HIGH : LOW);
}

void Power::rudder(boolean onOff) {
  digitalWrite(RUDDER_MOSFET, onOff ? HIGH : LOW);
}

void Power::rudderDirectionForward(boolean onOff) {
  digitalWrite(RUDDER_DIRECTION_PIN, onOff ? HIGH : LOW);
}

void Power::rudderBrake(boolean onOff) {
  digitalWrite(RUDDER_BRAKE_ENABLE_PIN, onOff ? HIGH : LOW);
}