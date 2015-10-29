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
  if (driveInterrupts()) {
    _drive->off();
  }

  manageLights();

  manageComms();

  drive();

  if(_drive->isOff()) { // if drive is off, we must not be ready to go, sleep
    smartSleep(10);
  }
}

void Pilot::manageLights() {
  if(_sensors->night() && _sensors->batteryAbove(45)) {
   // lights on
  }
  if(_sensors->day() || _sensors->batteryBelow(45)) {
    // lights off
  }
}

void Pilot::manageComms() {
  if(_comms->needToCommunicate()) {
    byte message[50];
    _comms->buildMessage(message);

    boolean wasOn = _drive->isOn();
    _drive->off();
    _comms->sendMessage(message);
    if(wasOn) {
      _drive->on();
    }
  }
}

boolean Pilot::driveInterrupts() {
  if(_sensors->currentAbove15Amps()) {
    _drive->currentExceeded();
    return true;
  }

  if(_sensors->storming()) {
    return true;
  }

  if(_sensors->batteryBelow(50)) {
    return true;
  }

  return false;
}

void Pilot::smartSleep(int minutes) {
  while(secondsSlept < minutes * 60) {
    // kill all mosfets and power except lights
    // set sleep mode
    // sleep for 8 seconds at a time until minutes reached
    // power up rtc
    digitalWrite(RTC_MOSFET, LOW);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
    digitalWrite(RTC_MOSFET, HIGH);
    secondsSlept = secondsSlept + 8;
  }
}

void Pilot::drive() {
  if(_drive->isOn() && _sensors->batteryAbove(50)) { // continue driving while battery good
    setCourse();
    return;
  }

  BatteryState batteryState = _sensors->batteryState();

  if(_drive->isOff() && batteryState == floating) { // don't start again until battery floats
    setCourse(); // TODO high risk point
    _drive->on();
    return;
  }
}

boolean Pilot::waitForNav() {
  int attempts = 0;
  time_t futureTime = 0;
  if(!_nav->ready()) {
    _drive->off(); // TODO high risk point
  }

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
    return false; // TODO high risk point
  }
  return true;
}

void Pilot::setCourse() {
  waitForNav();
  _drive->direction(_nav->courseChangeNeeded()); // TODO is it possible to set change while drive off?
}