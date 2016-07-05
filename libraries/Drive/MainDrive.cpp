#include "MainDrive.h"
#include "Sensors.h"
#include "pins.h"

const PROGMEM int MIN_POWER = 20;

MainDrive::MainDrive(Power* power, Sensors* sensors)
{
  _on = false;
  _power = power;
  _sensors = sensors;
  _motor = new Servo();
}


void MainDrive::forward() {
  if(isOn() && tooMuchCurrentDraw()) {
    _stalled = true;
  }
  if(!disabled()){
    on();
    _motor->write(map(20, -100, 100, 0, 179));
  }
}

void MainDrive::off()
{
  _motor->detach();
  _power->mainDrive(false);
  _on = false;
}

void MainDrive::on()
{
  _motor->attach(CENTER_PWM);
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