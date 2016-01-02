#ifndef Message_h
#define Message_h

#include "Arduino.h"

class Message
{
  public:
    Message(byte message[50]);
    void applyCoordinates(float lat, float longi);
    void applyTemperatures(float water, float air, float internal, float battery);
    float readLatitude();
    float readLongitude();
    void readTemperatures(float temps[4]);
    void applyLightening(int count);
    int readLightening();
    void applyAttempts(int count);
    int readAttempts();
    void print();
  private:
    byte* _message;
};

#endif