#ifndef Message_h
#define Message_h

#include "Arduino.h"
#include "Sensors.h"

class Message
{
  public:
    Message(byte message[50]);
    void applyCoordinates(float lat, float longi);
    void applyTemperatures(Temperatures temps);
    Temperatures readTemperatures();
    void applyLightening(int count);
    void applyAttempts(int count);
    void print();
  private:
    byte* _message;
};

#endif