#include "Sensors.h"
#include "OneWire.h"
#include "DallasTemperature.h"

#include "Time.h"
#include "Wire.h"
#include "DS1307RTC.h"

#define ONE_WIRE_BUS 2
#define TEMP_MOSFET 22

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature temperatureSensors(&oneWire);
DeviceAddress _thermometer = {0x28, 0xCD, 0x90, 0x29, 0x07, 0x00, 0x00, 0xE5};

Sensors::Sensors()
{
  _when_times_out = 0;
}

Temperatures Sensors::retrieveTemperatures()
{
  float fahrenheit = -180;
  time_t futureTime = 0;
  digitalWrite(TEMP_MOSFET, HIGH); // power on the temp sensors

  while((fahrenheit < -100 || fahrenheit > 170) && !timeout(futureTime, 3)) { // attempt for a second to retrieve temps
    temperatureSensors.requestTemperatures();
    fahrenheit = DallasTemperature::toFahrenheit(temperatureSensors.getTempC(_thermometer));
  }
  Temperatures temps = {fahrenheit, 0, 0, 0};

  digitalWrite(TEMP_MOSFET, LOW); // turn them back off
  return temps;
}

boolean Sensors::night()
{
  return false;
}

boolean Sensors::day()
{
  return true;
}

boolean Sensors::batteryAbove50()
{
  return true;
}

boolean Sensors::batteryBelow50()
{
  return false;
}

boolean Sensors::currentAbove15Amps()
{
  return false;
}

boolean Sensors::storming() {
  return false;
}

boolean Sensors::timeout(time_t& futureTime, int seconds) {
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
  return draining;
}