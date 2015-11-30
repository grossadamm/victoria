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
  private:
    boolean driveInterrupts();
    void drive();
    void manageLights();
    void manageComms();
    void manual();
    void smartSleep(int minutes);
    void setCourse();
    boolean waitForNav();
    Navigation* _nav;
    Sensors* _sensors;
    Communications* _comms;
    Drive* _drive;
    Storage* _storage;
    Power* _power;
    int secondsSlept;
    boolean _manualControl;
    ManualControlData _lastManualControlData;
};

#endif