// #include "Arduino.h"
#include "Power.h"
#include "Arduino.h"
#include "pins.h"

Power::Power()
{
  pinMode(BRAKE_RUDDER___SHIELD, OUTPUT);
  pinMode(FORWARD_RUDDER___SHIELD, OUTPUT);
  pinMode(TEMP_MOSFET, OUTPUT);
  pinMode(RTC_MOSFET, OUTPUT);
  pinMode(GPS_MOSFET, OUTPUT);
  pinMode(GPSCOMMS_SLEEP_PIN, OUTPUT);
  pinMode(LED_MOSFET, OUTPUT);
  pinMode(RUDDER_MOSFET, OUTPUT);
  pinMode(CENTER_MOSFET, OUTPUT);
  pinMode(LEFT_MOSFET, OUTPUT);
  pinMode(RIGHT_MOSFET, OUTPUT);
  pinMode(PHOTORESISTOR_MOSFET, OUTPUT);
  
}

void Power::gps(boolean onOff) {
  digitalWrite(GPS_MOSFET, onOff ? HIGH : LOW);
}

void Power::gpsComms(boolean onOff) {
  digitalWrite(GPSCOMMS_SLEEP_PIN, onOff ? LOW : HIGH);
}

void Power::lights(boolean onOff) {
  digitalWrite(LED_MOSFET, onOff ? HIGH : LOW);
}

void Power::mainDrive(boolean onOff) {
  digitalWrite(CENTER_MOSFET, onOff ? HIGH : LOW);
}

void Power::secondaryDriveLeft(boolean onOff) {
  digitalWrite(LEFT_MOSFET, onOff ? HIGH : LOW);
}

void Power::secondaryDriveRight(boolean onOff) {
  digitalWrite(RIGHT_MOSFET, onOff ? HIGH : LOW);
}
void Power::rfComms(boolean onOff) {
  digitalWrite(RF_MOSFET, onOff ? HIGH : LOW);
}

void Power::rtc(boolean onOff) {
  digitalWrite(RTC_MOSFET, onOff ? HIGH : LOW);
}

void Power::rudder(boolean onOff) {
  digitalWrite(RUDDER_MOSFET, onOff ? HIGH : LOW);
}

void Power::rudderDirectionForward(boolean onOff) {
  digitalWrite(FORWARD_RUDDER___SHIELD, onOff ? HIGH : LOW);
}

void Power::rudderBrake(boolean onOff) {
  digitalWrite(BRAKE_RUDDER___SHIELD, onOff ? HIGH : LOW);
}

void Power::temps(boolean onOff) {
  digitalWrite(TEMP_MOSFET, onOff ? HIGH : LOW);
}

void Power::photocell(boolean onOff) {
  digitalWrite(PHOTORESISTOR_MOSFET, onOff ? HIGH : LOW);
}

void Power::killAllButLights() {
  gps(false);
  gpsComms(false);
  mainDrive(false);
  rfComms(false);
  rtc(false);
  rudder(false);
  rudderDirectionForward(false);
  rudderBrake(false);
  secondaryDriveLeft(false);
  secondaryDriveRight(false);
  temps(false);
  photocell(false);
}