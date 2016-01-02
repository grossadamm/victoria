#include "RFComms.h"

RFComms::RFComms(Power* power)
{
  _power = power;
  _on = false;
  _radio = new RF24(48,49);
  on();
}

void RFComms::on(){
  byte addresses[][6] = {"1Node","2Node"};

  _radio->begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  _radio->setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  _radio->openWritingPipe(addresses[0]);
  _radio->openReadingPipe(1,addresses[1]);
  
  // Start the radio listening for data
  _radio->startListening();
  _on = true;
}

void RFComms::off() {
  // _radio->stopListening();
  _power->rfComms(false);
  _on = false;
}

boolean RFComms::needToCommunicate()
{  
  return false;
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
  int result = _radio->write( &buffer, sizeof(buffer) );

  _radio->startListening();
  return result == 0;  
}

boolean RFComms::dataAvailable() {
  return _radio->available();
}

void RFComms::readMessage(char returnBuf[]) {
  if( _radio->available()){
    while (_radio->available()) { 
      char buffer[32];
      _radio->read( buffer, sizeof(buffer) );
      for(int i = 0; i<32; i++) {
        returnBuf[i] = buffer[i];
      }
    }
  }
}

boolean RFComms::isOn() {
  return _on;
}