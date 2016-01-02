Pilot* pilot;

// RUDDER REVOLUTION ENCODER COUNTS 126000

// PIN USAGE:
// A0: VOLTAGE FROM RUDDER - SHIELD
// A1: SHIELD RESERVED
// 2:  ONE WIRE
// 3:  RUDDER PWM - SHIELD
// 8:  SHIELD RESERVED
// 9:  BRAKE RUDDER - SHIELD
// 10: GPSComms sleep pin
// 11: SHIELD RESERVED
// 12: FORWARD RUDDER - SHIELD
// 13: SHIELD RESERVED
// 14: GPS - Serial3
// 15: GPS - Serial3
// 16: GPSComms - Serial2
// 17: GPSComms - Serial2
// 18: RUDDER ENCODER - Serial1
// 19: RUDDER ENCODER - Serial1
// 20: RTC - SDA
// 21: RTC - SCL
// 22: TEMP MOSFET
// 23: RTC MOSFET
// 24: GPS MOSFET
// 25: WIRELESS MOSFET
// 26: COMMS MOSFET
// 27: LIGHTENING MOSFET
// 28: PHOTORESISTOR MOSFET
// 29: ROTATION MOSFET
// 30: DIRECTION_SERVO_PIN
// 48: RF - CE
// 49: RF - CSN
// 50: RF - MISO
// 51: RF - MOSI
// 52: RF - SCK

void setup()
{
  Serial.begin(115200);
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
  if(diff>10) {
    Serial.print("Took too long to run this loop! ");
    Serial.print(diff);
    Serial.println("ms");
  }
}

bool ISBDCallback()
{
  pilot->insideISBD();
  loop();
  pilot->outsideISBD();
  return true;
}


