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


void Message::applyTemperatures(Temperatures temps) {
  int waterTemperature = ((int)temps.water) - 75;
  signed char waterTemperature_sc = (signed char) waterTemperature;
  byte waterTemperature_b = (byte) waterTemperature_sc;
  
  _message[0] = waterTemperature_b;
  _message[1] = waterTemperature_b;
  _message[2] = waterTemperature_b;
  _message[3] = waterTemperature_b;
}

void Message::applyLightening(int lighteningCount) {
  _message[5] = (byte) lighteningCount;
}

void Message::applyAttempts(int attemptsBeforeComm) {
  _message[7] = (byte) attemptsBeforeComm;
}

void Message::print() {
  Serial.println("Message follows:");
  for(int i=0; i<17; i++){
    Serial.print(_message[i]);
  }
  Serial.print('\n');
  Serial.println("------");
}