#ifndef Pilot_h
#define Pilot_h

#include "Arduino.h"
#include "Drive.h"
#include "Navigation.h"
#include "Sensors.h"
#include "Communications.h"

struct ManualControlData; /* Forward declaration */

typedef struct ManualControlData
{
  char forwardReverse;
  char leftRightCenter;
} ManualControlData;

class Pilot
{
  public:
    Pilot();
    void run();
    void insideISBD();
    void outsideISBD();
  private:
    bool driveInterrupts();
    void drive();
    void manageLights();
    void manageComms();
    void manual();
    void smartSleep(int minutes);
    void setCourse();
    void processCommsData();
    bool waitForNav();
    Navigation* _nav;
    Sensors* _sensors;
    Communications* _comms;
    Drive* _drive;
    Storage* _storage;
    Power* _power;
    int secondsSlept;
    bool _manualControl;
    ManualControlData _lastManualControlData;
    bool _insideISBD;
};

#endif