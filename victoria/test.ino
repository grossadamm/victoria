#include <ArduinoUnit.h>

#include "test_control_message.h"
#include "test_message.h"

void setup(){Serial.begin(9600);Serial.println("start tests");}

void loop()
{
  Test::run();
}
