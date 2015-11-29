#include <Encoder.h>

/*************************************************************
Motor Shield 1-Channel DC Motor Demo
by Randy Sarafan

For more information see:
http://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

*************************************************************/
#define EncoderPinA 18   // Encoder Pin A pin 2 and pin 3 are inturrpt pins
#define EncoderPinB 19  // Encoder Pin B


Encoder knobLeft(19,18);
void setup() {
  
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
//  pinMode(EncoderPinA, INPUT);
//  pinMode(EncoderPinB, INPUT);
//  digitalWrite(EncoderPinA, LOW);
//  digitalWrite(EncoderPinB, LOW);
//  attachInterrupt(digitalPinToInterrupt(EncoderPinA), readEncoder, CHANGE);
  Serial.begin(9600);
  
}

long counts = 0;

void loop(){
  
  if(counts > 126000) {
     digitalWrite(12, LOW);
    digitalWrite(9, HIGH);
    analogWrite(3, 0);
    Serial.println("360");
  } else {
    digitalWrite(12, HIGH); //Establishes forward direction of Channel A
    digitalWrite(9, LOW);   //Disengage the Brake for Channel A
    analogWrite(3, 255);   //Spins the motor on Channel A at full speed
//    Serial.println("full speed");
    Serial.println(knobLeft.read());
//    Serial.println(analogRead(0));
  }
  
}
//
//void readEncoder() //this function is triggered by the encoder CHANGE, and increments the encoder counter
//{ 
//
//  if(digitalRead(EncoderPinB) == digitalRead(EncoderPinA) )
//  {
//    counts = counts-1; //you may need to redefine positive and negative directions
//  }
//  else
//  {
//    counts = counts+1;
//  }
//}
