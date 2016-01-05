#include "Message.h"

test(Message_coordinates) 
{
  byte m[50] = {0};
  Message* msg = new Message(m);
  float lat = -32.33;
  float longi = 100.55;
  msg->applyCoordinates(lat,longi);

  float readLat = msg->readLatitude();
  assertEqual(lat, readLat);

  float readLong = msg->readLongitude();
  assertEqual(longi, readLong);
}


test(Message_temps) 
{
  byte m[50] = {0};
  Message* msg = new Message(m);
  Temperatures temps = {32,33,34,35};
  msg->applyTemperatures(temps.water, temps.air, temps.internal, temps.battery);

  float readTemps[4];
  msg->readTemperatures(readTemps);
  assertEqual(temps.water, readTemps[0]);
  assertEqual(temps.air, readTemps[1]);
  assertEqual(temps.internal, readTemps[2]);
  assertEqual(temps.battery, readTemps[3]);
}

test(Message_ligthtening) 
{
  byte m[50] = {0};
  Message* msg = new Message(m);
  int lightening = 5;
  msg->applyLightening(lightening);

  int readLightening = msg->readLightening();
  assertEqual(lightening, readLightening);
}

test(Message_attempts) 
{
  byte m[50] = {0};
  Message* msg = new Message(m);
  int attempts = 5;
  msg->applyAttempts(attempts);

  int readAttempts = msg->readAttempts();
  assertEqual(attempts, readAttempts);
}