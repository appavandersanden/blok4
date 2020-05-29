#include <Wire.h>
#include <VL53L0X.h>

#define knopje 11
VL53L0X sensor;

int functieknop = 1;

void setup()
{
  pinMode(12,INPUT_PULLUP);
  digitalWrite(12,HIGH);
  Serial.begin(9600);
  Wire.begin();

  sensor.init();
  sensor.setTimeout(900);

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous(100);

  pinMode(knopje, INPUT_PULLUP);
}

void loop()
{
  int distance =sensor.readRangeContinuousMillimeters();
//  int distance =sensor.startContinuous(100);

  Serial.println(distance);

  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

 
 

  if(functieknop == 0){

    Serial.println("autonoome functie");
  }
  delay(200);
} 
