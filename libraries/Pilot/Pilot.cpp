#include "Pilot.h"
#include "Drive.h"
#include "Navigation.h"
#include "Sensors.h"
#include "Communications.h"
#include "LowPower.h"

#include "Time.h"

Pilot::Pilot()
{
  _storage = new Storage();
  _power = new Power();
  _sensors = new Sensors(_power);
  _nav = new Navigation(_sensors, _storage, _power);
  _comms = new Communications(_nav, _sensors);
  _drive = new Drive(_power);
  _manualControl = false;
}

void Pilot::run()
{
  if (driveInterrupts()) {
    _drive->off();
  }

  manageLights();

  manageComms();

  if(_manualControl) {
    manual();
    return;
  }

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

void Pilot::manual() {
  ManualControlData input = _comms->readControlData();
  _drive->direction(input.leftRightCenter);
  _drive->speed(input.forwardReverse);
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
    _power->rtc(false);
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
    _power->rtc(true);
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

  while(!_nav->ready()){ // TODO high risk point
    Serial.println("Waiting for nav to be ready...");
    delay(100);
    futureTime = 0;
    while(!_sensors->timeout(futureTime, 45) && !_nav->ready()){ // attempt for 45 seconds
      delay(100);
    }
    if(!_nav->ready()) { // if still not good, wait 10 minutes
      Serial.println("Nav failed after 45 seconds, sleeping");
      delay(100);
      smartSleep(10);
      attempts++;
    }
    if(_nav->ready())
      Serial.println("Nav now ready!");
  }

  return true;
}

void Pilot::setCourse() {
  waitForNav();
  _drive->direction(_nav->courseChangeNeeded()); // TODO is it possible to set change while drive off?
}