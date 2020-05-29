

#include <Wire.h>
#include <ZumoShield.h>
#include <VL53L0X.h>

#define LED_PIN 13

VL53L0X sensor;

ZumoMotors motors;
int functieknop = 1;

void setup()
{
  
  digitalWrite(12,HIGH);
  Serial.begin(9600);
  Wire.begin();

  sensor.init();
  sensor.setTimeout(900);
  sensor.startContinuous();

  pinMode(11, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(12,INPUT_PULLUP);
}

void loop()
{

 int distance =sensor.readRangeContinuousMillimeters();
  //int distance =sensor.startContinuous(100);

  //functieknop = digitalRead(11);


  if ((functieknop == 1) & (distance > 200)){
   digitalWrite(LED_PIN, HIGH);
    motors.setLeftSpeed(50);
    motors.setRightSpeed(50);
    delay(2);
     Serial.println(distance);

  }

  if ((functieknop == 1) & (distance < 200) & (distance > 50)){
   digitalWrite(LED_PIN, LOW);
  motors.setLeftSpeed(0);
    motors.setRightSpeed(0);
    delay(2);
     Serial.println(distance);
  
  }

   if ((functieknop == 1) & (distance < 50)){
   digitalWrite(LED_PIN, LOW);
    motors.setLeftSpeed(-50);
    motors.setRightSpeed(-50);
    delay(2);
     Serial.println(distance);
     
  }


}
