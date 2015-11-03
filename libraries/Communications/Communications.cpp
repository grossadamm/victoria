#include "Communications.h"
#include "Time.h"
#include "Wire.h"
#include "DS1307RTC.h"
#include "EEPROM.h"
#include "Message.h"
#include "ControlMessage.h"

const PROGMEM int DAY_LAST_COMMUNICATED_POSITION = 1;
const PROGMEM int GPS_COMMS_MOSFET = 24;

Communications::Communications(Navigation *nav, Sensors *sensors)
{
  _nav = nav;
  _sensors = sensors;
  _rfEnabled = true;
  _radio = new RF24(8, 9);
  _lastControlData = {0, 0}; // zero speed and zero rudder turn
  setSyncProvider(RTC.get); 
}

void Communications::buildMessage(byte message[50])
{
  Serial.println("Building the message");
  Message* msg = new Message(message);
  msg->applyTemperatures(_sensors->retrieveTemperatures());
  msg->applyLightening(5);
  msg->applyAttempts(0);
  msg->applyCoordinates(33.333, 123.123);
  msg->print();
}

boolean Communications::needToCommunicate()
{  
  if(hour()>=18 && !communicatedToday()) {
    return true;
  } else {
    return false;
  }
}

boolean Communications::sendMessage(byte message[50]){
  Serial.println("sending!");
  lastCommunicatedOn(day());
  return true;  
}

// void Communications::receiveMessage() {
//   Serial.println("receiving!"); // mailbox check costs a credit
// }

boolean Communications::communicatedToday() {
  return ((int) EEPROM.read(DAY_LAST_COMMUNICATED_POSITION)) == day();
}

void Communications::lastCommunicatedOn(int day) {
  EEPROM.write(DAY_LAST_COMMUNICATED_POSITION, day);
}

ManualControlData Communications::readControlData() {
  byte buffer[32];
  bool newData = false;

  if(_radio->available()){
    _radio->read(&buffer,32);
    newData = true;
  }

  if(newData) {
    ControlMessage* control = new ControlMessage(buffer);
    _lastControlData = control->getCommand();
  }
  return _lastControlData;
}