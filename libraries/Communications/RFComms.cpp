#include "RFComms.h"

RFComms::RFComms(Power* power)
{
  _power = power;
  _on = false;
  _radio = new RF24(8, 9);
  on();
}

void RFComms::on(){
  _power->rfComms(true);
  _on = true;
}

void RFComms::off() {
  _power->rfComms(false);
  _on = false;
}

boolean RFComms::needToCommunicate()
{  
  return isOn();
}

boolean RFComms::sendMessage(byte message[50]){
  Serial.println("not sending! not yet implemented");
  return false;  
}

boolean RFComms::dataAvailable() {
  return _radio->available();
}

void RFComms::readMessage(char buffer[32]) {
  _radio->read(&buffer,32);
}

boolean RFComms::isOn() {
  return _on;
}