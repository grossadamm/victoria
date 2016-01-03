#include "Pilot.h"
#include "Drive.h"
#include "Navigation.h"
#include "Sensors.h"
#include "Communications.h"
#include "LowPower.h"
#include "MemoryFree.h"
#include "Time.h"

Pilot::Pilot()
{
  _storage = new Storage();
  _power = new Power();
  _sensors = new Sensors(_power);
  _nav = new Navigation(_sensors, _storage, _power);
  _comms = new Communications(_nav, _sensors, _power, _storage);
  _drive = new Drive(_power);
  _lastManualControlData = {0, 0}; // zero speed and zero rudder turn
  _manualControl = false;
  _insideISBD = false;
}

// This is the "loop" that continues to making things go
// first make sure we aren't burning up motors
// then communicate as needed
// then drive or sleep
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
    Serial.println("Would sleep for 10");
    smartSleep(10);
  }
}

void Pilot::insideISBD() {
  _insideISBD = true;
}

void Pilot::outsideISBD(){
  _insideISBD = false;
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
  if(_insideISBD) {
    return;
  }
  if(_comms->needToCommunicate()) {
    byte message[50];
    _comms->buildMessage(message);

    boolean wasOn = _drive->isOn();
    if(_comms->useGps()){
      _drive->off();
    }
    _comms->sendMessage(message);
    if(wasOn) {
      _drive->on();
    }
  }
  if(_comms->controlDataAvailable()) {
    processCommsData();
  }
}

void Pilot::processCommsData() {
  Command cmd = _comms->readControlData();
  Serial.print("New command: ");
  Serial.println(cmd.command);
  Serial.print("New data: ");
  Serial.println(cmd.data);
  if(cmd.command == '\'')
    bool foo = false;
    // _lastManualControlData = {cmd.data[0], cmd.data[1]}; // ‘FL$ Forward/Reverse %, Left Right Center -90<->90
  else if(cmd.command == 'Z')
    _manualControl = true; // Z$ Manual control 
  else if(cmd.command == 'A')
    _manualControl = false; // A$ Automated control
  else if(cmd.command == '?')
    _comms->sendStatusNextCheck();
  else if(cmd.command == 'E')
    _comms->enableRF();// E$ Enable receiver
  else if(cmd.command == 'D')
    _comms->disableRF();// D$ Disable receiver
  else if(cmd.command == 'N')
    bool foo = false;  // N**$ Override next waypoint
  else if(cmd.command == 'O')
    bool foo = false;  // O**$ Override all waypoints
  else if(cmd.command == 'S')
    bool foo = false;
    // smartSleep(cmd.data[0]*60);  // S**$ Sleep for x hours
  else if(cmd.command == 'U')
    bool foo = false;  // U**$ Update every x hours
  else if(cmd.command == 'C')
    bool foo = false;  // C**$ Check for messages every x hours
  else if(cmd.command == 'T')
    bool foo = false;  // T**$ Run test every x motor runs
  else if(cmd.command == 'P') {
    // P*1$ Enable motor x (1: main, 2: secondary, 3: rudder)
    // P*0$ Disable motor x (1: main, 2: secondary, 3: rudder)
  } else {
    Serial.println("Invalid command received");
  }
}

void Pilot::manual() {
  _drive->direction(_lastManualControlData.leftRightCenter);
  _drive->speed(_lastManualControlData.forwardReverse);
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
    if(!_nav->ready()) { // if still not good, wait 1 minutes
      Serial.println("Nav failed after 45 seconds, sleeping");
      delay(100);
      smartSleep(1);
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