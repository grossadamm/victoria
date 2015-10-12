#include "Pilot.h"
#include "Drive.h"
#include "Navigation.h"
#include "Sensors.h"
#include "Communications.h"
#include "LowPower.h"

#include "Time.h"

#define RTC_MOSFET 23

Pilot::Pilot(Navigation *nav, Sensors *sensors, Communications* comms)
{
  _nav = nav;
  _sensors = sensors;
  _comms = comms;
  _drive = new Drive();
}

void Pilot::run()
{
  manageLights();

  manageComms();

  if (driveInterrupts()) {
    _drive->off();
    smartSleep(10);
    return;
  }

  drive();

  BatteryState batteryState = _sensors->batteryState();

  if(batteryState != floating && batteryState != draining) { // ?
    smartSleep(10);
    return;
  }
}

void Pilot::manageLights() {
  if(_sensors->night() && _sensors->batteryAbove50()) {
   // lights on
  }
  if(_sensors->day() || _sensors->batteryBelow50()) {
    // lights off
  }
}

void Pilot::manageComms() {
  if(_comms->needToCommunicate()) {
    Serial.println("Need to communicate");
    byte message[50];
    _comms->buildMessage(message);
    _comms->sendMessage(message);
  } else {
    Serial.println("No need to communicate, build message anyways");
    byte message[50];
    _comms->buildMessage(message);
  }
}

boolean Pilot::driveInterrupts() {
  if(_sensors->currentAbove15Amps()) {
    return true;
  }

  if(_sensors->storming()) {
    return true;
  }

  if(_sensors->batteryBelow50()) {
    return true;
  }

  return false;
}

void Pilot::smartSleep(int minutes) {
  while(secondsSlept < minutes * 60) {
    digitalWrite(RTC_MOSFET, LOW);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
    digitalWrite(RTC_MOSFET, HIGH);
    secondsSlept = secondsSlept + 8;
  }
}

void Pilot::drive() {
  waitForNav();
  if(_drive->isOn() && _sensors->batteryAbove50()) {
    setCourse();
    return;
  }

  BatteryState batteryState = _sensors->batteryState();

  if(_drive->isOff() && batteryState == floating) {
    setCourse();
    _drive->on();
    return;
  }
}

boolean Pilot::waitForNav() {
  int attempts = 0;
  time_t futureTime = 0;
  while(!_nav->ready() && attempts < 3){
    Serial.println("Waiting for nav to be ready...");
    while(_sensors->timeout(futureTime, 45) && !_nav->ready()){ // attempt for 45 seconds
      _nav->ready(); // to make nav ready
    }
    if(!_nav->ready()) { // if still not good, wait 10 minutes
      Serial.println("Nav failed after 45 seconds, sleeping");
      smartSleep(10);
      attempts++;
    }
  } // try that three times

  if(!_nav->ready()) {
    return false;
  }
  return true;
}

void Pilot::setCourse() {
  waitForNav();

  LeftRightCenter lrc = _nav->leftRightCenter();
  _drive->direction(lrc);
}