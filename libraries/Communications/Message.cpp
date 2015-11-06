#include "Message.h"
#include "Sensors.h"

Message::Message(byte message[50]) {
  _message = message;
}

union FloatToByte
{
 float number;
 uint8_t bytes[4];
};

void Message::applyCoordinates(float lat, float longi) {
  union FloatToByte latitude;
  latitude.number = lat;
  
  int startingPoint = 9;
  for(int i = startingPoint; i < startingPoint + 4; i++) {
    int index = i-startingPoint;
    _message[i] = latitude.bytes[index];
  }

  union FloatToByte longitude;
  longitude.number = longi;

  startingPoint = startingPoint + 4;
  for(int i = startingPoint; i < startingPoint + 4; i++) {
    int index = i-startingPoint;
    _message[i] = longitude.bytes[index];
  }
}

float Message::readLatitude() {
  union FloatToByte latitude;

  int startingPoint = 9;
  for(int i = startingPoint; i < startingPoint + 4; i++) {
    int index = i-startingPoint;
    latitude.bytes[index] = _message[i];
  }

  return latitude.number;
}

float Message::readLongitude() {
  union FloatToByte longitude;

  int startingPoint = 13;
  for(int i = startingPoint; i < startingPoint + 4; i++) {
    int index = i-startingPoint;
    longitude.bytes[index] = _message[i];
  }

  return longitude.number;
}

void Message::applyTemperatures(Temperatures temps) {
  int temperature = ((int)temps.water) - 75;
  signed char temperature_sc = (signed char) temperature;
  byte temperature_b = (byte) temperature_sc; 
  _message[0] = temperature_b;

  temperature = ((int)temps.air) - 75;
  temperature_sc = (signed char) temperature;
  temperature_b = (byte) temperature_sc; 
  _message[1] = temperature_b;

  temperature = ((int)temps.internal) - 75;
  temperature_sc = (signed char) temperature;
  temperature_b = (byte) temperature_sc; 
  _message[2] = temperature_b;

  temperature = ((int)temps.battery) - 75;
  temperature_sc = (signed char) temperature;
  temperature_b = (byte) temperature_sc; 
  _message[3] = temperature_b;
}

Temperatures Message::readTemperatures() {
  Temperatures temps = {
    (int)((signed char) _message[0]) + 75,
    (int)((signed char) _message[1]) + 75,
    (int)((signed char) _message[2]) + 75,
    (int)((signed char) _message[3]) + 75
  };
  return temps;
}

void Message::applyLightening(int lighteningCount) {
  _message[5] = (byte) lighteningCount;
}

int Message::readLightening() {
  return (int)_message[5];
}

void Message::applyAttempts(int attemptsBeforeComm) {
  _message[7] = (byte) attemptsBeforeComm;
}

int Message::readAttempts() {
  return (int) _message[7];
}

void Message::print() {
  Serial.println("Message follows:");
  for(int i=0; i<17; i++){
    Serial.print(_message[i]);
  }
  Serial.print('\n');
  Serial.println("------");
}