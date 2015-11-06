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
  msg->applyTemperatures(temps);

  Temperatures readTemps = msg->readTemperatures();
  assertEqual(temps.water, readTemps.water);
  assertEqual(temps.air, readTemps.air);
  assertEqual(temps.internal, readTemps.internal);
  assertEqual(temps.battery, readTemps.battery);
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