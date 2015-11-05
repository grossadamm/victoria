#include "Message.h"

test(Message) 
{
  byte m[50] = {0};
  Message* msg = new Message(m);
  Temperatures temps = {1,2,3,4};
  msg->applyTemperatures(temps);
  Temperatures readTemps = msg->readTemperatures();
  assertEqual(temps.water, readTemps.water);
}