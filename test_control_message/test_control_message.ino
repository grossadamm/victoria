#include <ArduinoUnit.h>

#include "all.h"

#include "ControlMessage.h"
#include "test_control_message.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  Test::run();
}
