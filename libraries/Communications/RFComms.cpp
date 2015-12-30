#include "RFComms.h"

RFComms::RFComms(Power* power)
{
  _power = power;
  _on = false;
  _radio = new RF24(8, 9);
  on();
}

void RFComms::on(){
  byte addresses[][6] = {"1Node","2Node"}; // TODO dunno what this is
  _power->rfComms(true);
  _radio->begin();
  _radio->openWritingPipe(addresses[0]);
  _radio->openReadingPipe(1,addresses[1]);
  _radio->enableAckPayload();                     // Allow optional ack payloads
  _radio->enableDynamicPayloads();                // Ack payloads are dynamic payloads
  _radio->startListening();
  _on = true;
}

void RFComms::off() {
  _radio->stopListening();
  _power->rfComms(false);
  _on = false;
}

boolean RFComms::needToCommunicate()
{  
  return isOn();
}

boolean RFComms::sendMessage(byte message[50]){
  _radio->stopListening();  
  byte buffer[32];
  for(int i = 0; i < 32; i++) {
    buffer[i] = message[i];
  }
  _radio->write( &buffer, sizeof(buffer) );
  for(int i = 0; i < 32; i++) {
    buffer[i] = 0;
  }
  for(int i = 32; i < 50; i++) {
    buffer[i-32] = message[i];
  }
  _radio->write( &buffer, sizeof(buffer) );

  _radio->startListening();
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