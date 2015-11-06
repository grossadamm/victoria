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
    float readLatitude();
    float readLongitude();
    Temperatures readTemperatures();
    void applyLightening(int count);
    int readLightening();
    void applyAttempts(int count);
    int readAttempts();
    void print();
  private:
    byte* _message;
};

#endif