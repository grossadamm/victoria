#ifndef Communications_h
#define Communications_h

#include "Arduino.h"
#include "Navigation.h"
#include "Sensors.h"
#include "Power.h"
#include "Storage.h"
#include <RF24.h>
#include "ControlMessage.h"
#include "Command.h"
#include "GPSComms.h"

class Communications
{
  public:
    Communications(Navigation* nav, Sensors* sensors, Power* power, Storage* storage);
    void buildMessage(byte message[50]);
    boolean needToCommunicate();
    boolean sendMessage(byte message[50]);
    boolean controlDataAvailable();
    Command readControlData();
  private:
    void applyTemperatures(byte message[50]);
    void applyCoordinates(byte message[50]);
    Navigation* _nav;
    Sensors* _sensors;
    Power* _power;
    Storage* _storage;
    GPSComms* _gpsComms;
    bool _rfEnabled;
    RF24* _radio;
    ControlMessage* _lastControlMessage;
};

#endif