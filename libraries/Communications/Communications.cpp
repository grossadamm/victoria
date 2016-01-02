#include "Communications.h"
#include "Time.h"
#include "Wire.h"
#include "DS1307RTC.h"
#include "Message.h"

const PROGMEM int DAY_LAST_COMMUNICATED_POSITION = 1;

Communications::Communications(Navigation *nav, Sensors *sensors, Power* power, Storage* storage)
{
  _nav = nav;
  _sensors = sensors;
  _power = power;
  _gpsComms = new GPSComms(power, storage);
  _rfComms = new RFComms(power);
  _lastControlMessage = new ControlMessage(new char {});
  _needToCommunicateOverride = false;
  setSyncProvider(RTC.get); 
}

void Communications::buildMessage(byte message[50])
{
  Serial.println("Building the message");
  for(int i = 0; i < 50; i++) {
    message[i] = 0;
  }
  Message* msg = new Message(message);
  Temperatures temps = _sensors->retrieveTemperatures();
  msg->applyTemperatures(temps.water, temps.air, temps.internal, temps.battery);
  msg->applyLightening(5);
  msg->applyAttempts(0);
  msg->applyCoordinates(_nav->lat(), _nav->lng());
  msg->print();
}

boolean Communications::useGps() {
  return !_rfComms->isOn();
}

void Communications::sendStatusNextCheck() {
  _needToCommunicateOverride = true;
}

boolean Communications::needToCommunicate() {
  return _needToCommunicateOverride || _rfComms->needToCommunicate() || _gpsComms->needToCommunicate();
}

boolean Communications::sendMessage(byte message[50]){
  _needToCommunicateOverride = false;
  if(_rfComms->isOn()) {
    return _rfComms->sendMessage(message);
  } else {
    return _gpsComms->sendMessage(message);
  }
}

boolean Communications::controlDataAvailable() {
  return _lastControlMessage->commandsAvailable() || _rfComms->dataAvailable(); // TODO can get stuck since we aren't checking
}

Command Communications::readControlData() {
  if(_lastControlMessage->commandsAvailable()) {
    return _lastControlMessage->getCommand();
  }

  if(useGps()) {
    // TODO read from gps
  } else {
    char buffer[32] = {0};
    _rfComms->readMessage(buffer);
    Serial.print("Buffer: ");
    Serial.println(buffer);
    _lastControlMessage = new ControlMessage(buffer);
  }

  if(_lastControlMessage->commandsAvailable()) {
    return _lastControlMessage->getCommand();
  }
}

void Communications::disableRF() {
  _rfComms->off();
}

void Communications::enableRF() {
  _rfComms->on();
}