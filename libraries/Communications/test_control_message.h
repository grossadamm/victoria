#include "ControlMessage.h"

test(ControlMessage_fetch) 
{
  char m[5] = { 'P', '1', '1', '$', 0 };
  ControlMessage* cm = new ControlMessage(m);
  assertEqual(1, cm->commandsAvailable());
  Command foo = cm->getCommand();
  assertEqual(m[0],foo.command);
  assertEqual(m[1],foo.data[0]);
  assertEqual(m[2],foo.data[1]);
  assertEqual(2, sizeof(foo.data)); 


  foo = cm->getCommand();
  assertEqual(0, cm->commandsAvailable());
}

test(ControlMessage_fetchfetch) 
{
  char m[9] = { 'P', '1', '1', '$', 'P', '1', '1', '$', 0 };
  ControlMessage* cm = new ControlMessage(m);
  assertEqual(1, cm->commandsAvailable());
  Command foo = cm->getCommand();
  assertEqual(1, cm->commandsAvailable());
  foo = cm->getCommand();
  assertEqual(0, cm->commandsAvailable());
}

test(ControlMessage_fetchfetchfetch) 
{
  char m[9] = "Z$A$?$";
  ControlMessage* cm = new ControlMessage(m);
  assertEqual(1, cm->commandsAvailable());
  Command foo = cm->getCommand();
  assertEqual('Z', foo.command);

  assertEqual(1, cm->commandsAvailable());
  foo = cm->getCommand();
  assertEqual('A', foo.command);

  assertEqual(1, cm->commandsAvailable());
  foo = cm->getCommand();
  assertEqual('?', foo.command);

  assertEqual(0, cm->commandsAvailable());
}