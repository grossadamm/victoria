#include "GPSComms.h"
#include "Time.h"
#include "Wire.h"
#include "DS1307RTC.h"
#include "IridiumSBD.h"

const PROGMEM int IRIDIUM_SLEEP_PIN = 10;
const PROGMEM int IRIDIUM_LOW_POWER = 1;
const PROGMEM int IRIDIUM_HIGH_POWER = 0;

IridiumSBD isbd(Serial2, IRIDIUM_SLEEP_PIN);

GPSComms::GPSComms(Power* power, Storage* storage)
{
  _storage = storage;
  _power = power;
  setSyncProvider(RTC.get); // TODO abstract out these globals
  isbd.setPowerProfile(IRIDIUM_LOW_POWER);
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
  _power->gpsComms(true);

  isbd.begin();
  byte rxBuffer[50];
  size_t size = 50;
  int result = isbd.sendReceiveSBDBinary(message, size, rxBuffer, size);
  for(int i = 0; i < 50; i++) {
     _lastMessageReceived[i] = rxBuffer[i];
   }
  isbd.sleep();

  _storage->lastCommunicatedOn(day());
  _power->gpsComms(false);
  return true;  
}

void GPSComms::readMessage(byte message[50]) {
   for(int i = 0; i < 50; i++) {
     message[i] = _lastMessageReceived[i];
   }
   for(int i = 0; i < 50; i++) {
     _lastMessageReceived[i] = 0;
   }
}

boolean GPSComms::communicatedToday() {
  return _storage->lastCommunicatedOn() == day();
}