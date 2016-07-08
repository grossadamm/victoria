#include "Sensors.h"
#include "OneWire.h"
#include "DallasTemperature.h"

#include "Time.h"
#include "Wire.h"
#include "DS1307RTC.h"

#include "pins.h"

OneWire oneWire(ONE_WIRE);
DallasTemperature temperatureSensors(&oneWire);
DeviceAddress _waterThermometer = {0x28, 0xCD, 0x90, 0x29, 0x07, 0x00, 0x00, 0xE5};

Sensors::Sensors(Power* power)
{
  _power = power;
  _when_times_out = 0;
}

Temperatures Sensors::retrieveTemperatures()
{
  float fahrenheit = -180;
  time_t futureTime = 0;
  _power->temps(true);

  while((fahrenheit < -100 || fahrenheit > 170) && !timeout(futureTime, 3)) { // attempt for a second to retrieve temps
    temperatureSensors.requestTemperatures();
    fahrenheit = DallasTemperature::toFahrenheit(temperatureSensors.getTempC(_waterThermometer));
  }
  Temperatures temps = {fahrenheit, 0, 0, 0};

  _power->temps(false);
  return temps;
}

bool Sensors::night()
{
  !day();
}

bool Sensors::day()
{
  _power->photocell(true);
  delay(3);
  map(analogRead(PHOTOCELL), 0, 1023, 0, 10) > 6;
  _power->photocell(false);
}

bool Sensors::batteryAbove(int percent)
{
  return true;
}

bool Sensors::batteryBelow(int percent)
{
  return false;
}

bool Sensors::currentAbove15Amps()
{
  return false;
}

bool Sensors::storming() {
  return false;
}

bool Sensors::timeout(time_t& futureTime, int seconds) {
  if(futureTime == 0) {
    futureTime = now() + seconds;
  } 
  if (now() > futureTime) {
    return true;
  } else {
    return false;
  }
}

BatteryState Sensors::batteryState() {
  return floating;
}

int Sensors::currentDrawLeft() {
  return map(analogRead(LEFT_CURRENT_DRAW), 0, 1024, -30, 30);
}

int Sensors::currentDrawRight() {
  return map(analogRead(RIGHT_CURRENT_DRAW), 0, 1024, -30, 30);
}

int Sensors::currentDrawCenter() {
  return map(analogRead(CENTER_CURRENT_DRAW), 0, 1024, -30, 30);
}