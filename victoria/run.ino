Pilot* pilot;

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


