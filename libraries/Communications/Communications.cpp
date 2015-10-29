#include "Communications.h"
#include "Time.h"
#include "Wire.h"
#include "DS1307RTC.h"
#include "EEPROM.h"

const PROGMEM int DAY_LAST_COMMUNICATED_POSITION = 1;
const PROGMEM int GPS_COMMS_MOSFET = 24;

Communications::Communications(Navigation *nav, Sensors *sensors)
{
  _nav = nav;
  _sensors = sensors;
  setSyncProvider(RTC.get); 
}

void Communications::buildMessage(byte message[50])
{
  Serial.println("Building the message");
  
  applyTemperatures(message);
  message[4] = '|';

  int lighteningCount = 5;
  message[5] = (byte) lighteningCount;
  message[6] = '|';

  int attemptsBeforeComm = 0;
  message[7] = (byte) attemptsBeforeComm;
  message[8] = '|';

  applyCoordinates(message);
  message[17] = '|';

  Serial.println("Message follows:");
  for(int i=0; i<17; i++){
    Serial.print(message[i]);
  }
  Serial.print('\n');
  Serial.println("------");
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

void Communications::receiveMessage() {
  Serial.println("receiving!"); // mailbox check costs a credit
}

void Communications::applyTemperatures(byte message[50]) {
  Temperatures temps = _sensors->retrieveTemperatures();
  int waterTemperature = ((int)temps.water) - 75;
  signed char waterTemperature_sc = (signed char) waterTemperature;
  byte waterTemperature_b = (byte) waterTemperature_sc;
  
  message[0] = waterTemperature_b;
  message[1] = waterTemperature_b;
  message[2] = waterTemperature_b;
  message[3] = waterTemperature_b;
}

union FloatToByte
{
 float number;
 uint8_t bytes[4];
};

void Communications::applyCoordinates(byte message[50]) {
  float lat = -99.521654;
  union FloatToByte latitude;
  latitude.number = lat;
  
  int startingPoint = 9;
  for(int i = startingPoint; i < startingPoint + 4; i++) {
    int index = i-startingPoint;
    message[i] = latitude.bytes[index];
  }

  float longi = 42.915512;
  union FloatToByte longitude;
  longitude.number = longi;

  startingPoint = startingPoint + 4;
  for(int i = startingPoint; i < startingPoint + 4; i++) {
    int index = i-startingPoint;
    message[i] = longitude.bytes[index];
  }
}

boolean Communications::communicatedToday() {
  return ((int) EEPROM.read(DAY_LAST_COMMUNICATED_POSITION)) == day();
}

void Communications::lastCommunicatedOn(int day) {
  EEPROM.write(DAY_LAST_COMMUNICATED_POSITION, day);
}