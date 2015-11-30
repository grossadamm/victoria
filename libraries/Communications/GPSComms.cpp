#include "GPSComms.h"
#include "Time.h"
#include "Wire.h"
#include "DS1307RTC.h"

GPSComms::GPSComms(Power* power, Storage* storage)
{
  _storage = storage;
  _power = power;
  setSyncProvider(RTC.get); // TODO abstract out these globals
}

boolean GPSComms::needToCommunicate()
{  
  if(hour()>=18 && !communicatedToday()) {
    return true;
  } else {
    return false;
  }
}

boolean GPSComms::sendMessage(byte message[50]){
  Serial.println("sending!");
  _power->gpsComms(true);
  _storage->lastCommunicatedOn(day());
  _power->gpsComms(false);
  return true;  
}

void GPSComms::readMessage(byte message[50]) {
   // mailbox check costs a credit
}

boolean GPSComms::communicatedToday() {
  return _storage->lastCommunicatedOn() == day();
}