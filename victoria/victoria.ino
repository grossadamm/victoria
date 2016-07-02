#include <pins.h>

#include <MemoryFree.h> //TODO

// Pilot
#include <Power.h>
#include <Drive.h>
#include <Pilot.h>
#include <RunningAverage.h>

// Sensors
#include <Sensors.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Comms
#include <Message.h>
#include <Communications.h>
#include <SPI.h>
#include "RF24.h"
#include <IridiumSBD.h> 

// Time
#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>

// Navigation
#include <Navigation.h>
#include <Storage.h>
#include <EEPROM.h>
#include <EEPROMAnything.h>
#include <TinyGPS++.h>
#include <AP_Declination.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Encoder.h>

// Sleep
#include <LowPower.h>
