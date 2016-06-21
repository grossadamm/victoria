Pilot* pilot;

// RUDDER REVOLUTION ENCODER COUNTS 126000

// PIN USAGE:
// A0: VOLTAGE FROM RUDDER - SHIELD
// A1: SHIELD RESERVED
// A2: ACCEL
// A3: ACCEL
// A4: ACCEL
// A5: PHOTOCELL
// A8: RUDDER CURRENT DRAW ?
// A11:CENTER CURRENT DRAW
// A11:RIGHT CURRENT DRAW
// A11:LEFT CURRENT DRAW
// A11:SOL CURRENT DRAW
// A11:BATT CURRENT DRAW 
// 2:  ONE WIRE
// 3:  RUDDER PWM - SHIELD
// 4:  LEFT PWM
// 5:  RIGHT PWM
// 6:  TEMP MOSFET 
// 7:  CENTER PWM
// 8:  SHIELD RESERVED
// 9:  BRAKE RUDDER - SHIELD
// 11: SHIELD RESERVED
// 12: FORWARD RUDDER - SHIELD
// 13: SHIELD RESERVED
// 14: GPS - Serial3
// 15: GPS - Serial3
// 16: GPSComms - Serial2
// 17: GPSComms - Serial2
// 18: RUDDER ENCODER - Serial1
// 19: RUDDER ENCODER - Serial1
// 20: RTC, COMPASS(w) - SDA
// 21: RTC, COMPASS(o) - SCL
// 25: WIRELESS MOSFET
// 22: GPSComms sleep pin
// 27: GPS MOSFET
// 32: ACCELL MOSFET
// 33: LEFT MOSFET
// 35: RIGHT MOSFET
// 37: CENTER MOSFET
// 38: RTC MOSFET
// 42: PHOTORESISTOR MOSFET
// 44: MOTOR DRAW MOSFET
// 46: CURRENT MONITORS MOSFET
// 47: RF MOSFET
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
    Serial.print("freeMemory()=");
    Serial.println(freeMemory());
  }
}

bool ISBDCallback()
{
  pilot->insideISBD();
  loop();
  pilot->outsideISBD();
  return true;
}


