#include "MainDrive.h"
#include "Sensors.h"
#include "pins.h"

MainDrive::MainDrive(Power* power, Sensors* sensors)
{
  _on = false;
  _power = power;
  _sensors = sensors;
}


void MainDrive::forward() { 

}

void MainDrive::off()
{
  _power->mainDrive(false);
  _on = false;
}

void MainDrive::on()
{
  _power->mainDrive(true);
  _on = true;  
}

boolean MainDrive::isOn() {
  return _on;
}

boolean MainDrive::isOff() {
  return !_on;
}

void MainDrive::attemptClear() {
  
}

void MainDrive::enable(bool onOff) {
  _enabled = onOff;
}

bool MainDrive::disabled() {
  return _stalled || !_enabled;
}