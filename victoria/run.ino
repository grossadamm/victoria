Pilot* pilot;

boolean DRIVING = true;
boolean SLEEPING = false;
boolean STATE = false;

// EEPROM USAGE: 
// BYTE 0: DAY LAST COMMUNICATED
// BYTE 1: NUMBER OF WAYPOINTS STORED
// ONWARDS: WAYPOINTS

// RUDDER REVOLUTION ENCODER COUNTS 126000

// PIN USAGE:
// A0: VOLTAGE FROM RUDDER - SHIELD
// A1: SHIELD RESERVED
// 2:  ONE WIRE
// 3:  RUDDER PWM - SHIELD
// 8:  SHIELD RESERVED
// 9:  BRAKE RUDDER - SHIELD
// 11: SHIELD RESERVED
// 12: FORWARD RUDDER - SHIELD
// 13: SHIELD RESERVED
// 14: GPS
// 15: GPS
// 18: RUDDER ENCODER
// 19: RUDDER ENCODER
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
  pilot = new Pilot();
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


