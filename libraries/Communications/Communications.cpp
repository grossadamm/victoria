#include "Communications.h"
#include "Time.h"
#include "Wire.h"
#include "DS1307RTC.h"
#include "EEPROM.h"
#include "Message.h"

const PROGMEM int DAY_LAST_COMMUNICATED_POSITION = 1;

Communications::Communications(Navigation *nav, Sensors *sensors, Power* power, Storage* storage)
{
  _nav = nav;
  _sensors = sensors;
  _power = power;
  _gpsComms = new GPSComms(power, storage);
  _lastControlMessage = new ControlMessage(new char {});
  _rfEnabled = true;
  _radio = new RF24(8, 9);
  setSyncProvider(RTC.get); 
}

void Communications::buildMessage(byte message[50])
{
  Serial.println("Building the message");
  Message* msg = new Message(message);
  msg->applyTemperatures(_sensors->retrieveTemperatures());
  msg->applyLightening(5);
  msg->applyAttempts(0);
  msg->applyCoordinates(_nav->lat(), _nav->lng());
  msg->print();
}

boolean Communications::needToCommunicate() {
  if(_rfEnabled)
    return true;
  return _gpsComms->needToCommunicate();
}

boolean Communications::sendMessage(byte message[50]){
  Serial.println("sending!");
  return true;  
}

boolean Communications::controlDataAvailable() {
  return _lastControlMessage->commandsAvailable(); // TODO can get stuck since we aren't checking
}

Command Communications::readControlData() {
  char buffer[32];
  bool newData = false;

  if(_radio->available() && !_lastControlMessage->commandsAvailable()){
    _radio->read(&buffer,32);
    newData = true;
  }

  if(newData)
    _lastControlMessage = new ControlMessage(buffer);

  return _lastControlMessage->getCommand();
}