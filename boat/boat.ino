// Pilot
#include <Servo.h>
#include <Drive.h>
#include <Pilot.h>
#include <RunningAverage.h>

// Sensors
#include <Sensors.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Comms
#include <Communications.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

// Time
#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>

// Navigation
#include <Navigation.h>
#include <EEPROM.h>
#include <EEPROMAnything.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <AP_Declination.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

// Sleep
#include <LowPower.h>

Navigation* nav;
Sensors* sensors;
Communications* comms;
Pilot* pilot;

boolean DRIVING = true;
boolean SLEEPING = false;
boolean STATE = false;

// EEPROM USAGE: 
// BYTE 0: DAY LAST COMMUNICATED
// BYTE 1: NUMBER OF WAYPOINTS STORED
// ONWARDS: WAYPOINTS


// PIN USAGE:
// 2: ONE WIRE
// 15: GPS
// 14: GPS
// 20: RTC
// 21: RTC
// 22: TEMP MOSFET
// 23: RTC MOSFET
// 24: GPS MOSFET
// 25: WIRELESS MOSFET
// 26: COMMS MOSFET
// 27: LIGHTENING MOSFET
// 28: PHOTORESISTOR MOSFET
// 29: ROTATION MOSFET
// 30: DIRECTION_SERVO_PIN

void setup()
{
  Serial.begin(9600);
  sensors = new Sensors();
  Serial.println("Sensors initialized");
  nav = new Navigation(sensors);
  Serial.println("Navigation initialized");
  comms = new Communications(nav, sensors);
  Serial.println("Comms initialized");
  pilot = new Pilot(nav, sensors, comms);
  Serial.println("Pilot initialized");
  delay(100); 
}

unsigned long time;
void loop()
{
  time = millis();
  pilot->run();
  unsigned long diff = millis() - time;
  if(diff>10)
    Serial.println("Took too long to run this loop!");
}

