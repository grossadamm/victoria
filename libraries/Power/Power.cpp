// #include "Arduino.h"
#include "Power.h"
#include "Arduino.h"

const PROGMEM int RUDDER_BRAKE_ENABLE_PIN = 9;
const PROGMEM int RUDDER_DIRECTION_PIN = 12;
const PROGMEM int TEMP_MOSFET = 36;
const PROGMEM int RTC_MOSFET = 23;
const PROGMEM int GPS_MOSFET = 24;
const PROGMEM int GPS_COMMS_MOSFET = 28; // NOT SURE
const PROGMEM int RF_COMMS_MOSFET = 41; 
const PROGMEM int RUDDER_MOSFET = 30;
const PROGMEM int MAIN_DRIVE_MOSFET = 31;
const PROGMEM int SECONDARY_DRIVE_MOSFET = 32;

Power::Power()
{
  pinMode(RUDDER_BRAKE_ENABLE_PIN, OUTPUT);
  pinMode(RUDDER_DIRECTION_PIN, OUTPUT);
  pinMode(TEMP_MOSFET, OUTPUT);
  pinMode(RTC_MOSFET, OUTPUT);
  pinMode(GPS_MOSFET, OUTPUT);
  pinMode(GPS_COMMS_MOSFET, OUTPUT);
  pinMode(RUDDER_MOSFET, OUTPUT);
  pinMode(MAIN_DRIVE_MOSFET, OUTPUT);
  pinMode(SECONDARY_DRIVE_MOSFET, OUTPUT);
  
}

void Power::gps(boolean onOff) {
  digitalWrite(GPS_MOSFET, onOff ? HIGH : LOW);
}

void Power::gpsComms(boolean onOff) {
  digitalWrite(GPS_COMMS_MOSFET, onOff ? HIGH : LOW);
}

void Power::mainDrive(boolean onOff) {
  digitalWrite(MAIN_DRIVE_MOSFET, onOff ? HIGH : LOW);
}

void Power::secondaryDrive(boolean onOff) {
  digitalWrite(SECONDARY_DRIVE_MOSFET, onOff ? HIGH : LOW);
}

void Power::rfComms(boolean onOff) {
  digitalWrite(RF_COMMS_MOSFET, onOff ? HIGH : LOW);
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

void Power::temps(boolean onOff) {
  digitalWrite(TEMP_MOSFET, onOff ? HIGH : LOW);
}