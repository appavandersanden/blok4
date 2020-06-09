#include <QTRSensors.h>
#include <Wire.h>
#include <ZumoShield.h>
#include "Adafruit_VL53L0X.h"

#define volgen A0
#define zelf A1
#define buzzer 3
#define userpushbuton 12
#define LED 13

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31

// set the pins to shutdown
#define SHT_LOX1 A2 //hier nog de juiste pin
#define SHT_LOX2 A3 //idem dito

// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;

Pushbutton button(ZUMO_BUTTON);
//button.waitForButton()
ZumoMotors motors;
/* SDA op SDA pin, SCL op SCL pin
 * buzzer pin 3, pushbutton pin 12, gele led pin 13
 * 3 way-switch pin A0, A1
 * PIN 7,8,9,10 voor motoren
 * QTR, PIN 4,5,6,11
 */


QTRSensors qtr;
const uint8_t SensorCount = 8;// aantal sensoren op de qtr
uint16_t sensorValues[SensorCount];

int read_dual_sensors(void) {
  int hummer;
  
  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!

  // print sensor one reading
  Serial.print("1: ");
  if(measure1.RangeStatus != 4) {     // if not out of range
    Serial.print(measure1.RangeMilliMeter);
  hummer = measure1.RangeMilliMeter;
  Serial.println("joe");
  Serial.println(hummer);
  } else {
    Serial.print("Out of range");
  }
  
  Serial.print(" ");

  // print sensor two reading
  Serial.print("2: ");
  if(measure2.RangeStatus != 4) {
    Serial.print(measure2.RangeMilliMeter);
  } else {
    Serial.print("Out of range");
  }
  Serial.println();
  return(hummer);
}

void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);    
  digitalWrite(SHT_LOX2, LOW);
  delay(10);
  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  // activating LOX1 and reseting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);

  // initing LOX1
  if(!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while(1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if(!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while(1);
  }
}

int uitvoerFunctieChecken(void) {
  int uitvoerfunctie;

  if (analogRead(volgen) == HIGH) {

    uitvoerfunctie = 1;
  }

  if (analogRead(zelf) == HIGH) {

    uitvoerfunctie = 2;
  }

  return (uitvoerfunctie);
}


void calibrerenQTR(void) {
  int waarde = HIGH;
  int i = 0;

  // led later knipperen zodat iedereen weet dat er gecalibreert meot worden
  // knop 12 indrukken zodat hij doorgaat.

  delay(2000);
  digitalWrite(LED, HIGH);

  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED, LOW); // turn off Arduino's LED

  // print the calibration minimum values measured when emitters were on
  Serial.println("minmum values QTR");
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  Serial.println("maximum values QTR");

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
}

void AGVcentreren(void) {
  int goedgekeurd = 0; // wanneer het potje in het midden van de qtr sensor staat wordt dit veranderdt naar 1 en gaat hij door;

  while (goedgekeurd = 0) {


    // read calibrated sensor values and obtain a measure of the line position
    // from 0 to 5000 (for a white line, use readLineWhite() instead)
    uint16_t position = qtr.readLineBlack(sensorValues);

    // print the sensor values as numbers from 0 to 1000, where 0 means maximum
    // reflectance and 1000 means minimum reflectance, followed by the line
    // position
    for (uint8_t i = 0; i < SensorCount; i++)
    {
      Serial.print(sensorValues[i]);
      Serial.print('\t');
    }
    Serial.println(position);

    // hier proberen we de agv voor de qtr sensor te centreren.
    if (sensorValues[0] <= 800)
      motors.setLeftSpeed(100);
      motors.setRightSpeed(100);
      delay(20);
      motors.setLeftSpeed(0);
      motors.setRightSpeed(0);
    }

    if (sensorValues[1] <= 800) {
      motors.setLeftSpeed(100);
      motors.setRightSpeed(100);
      delay(20);
      motors.setLeftSpeed(0);
      motors.setRightSpeed(0);
    }

    if ((sensorValues[0] <= 800) & (sensorValues[1] <= 400) & (sensorValues[2] <= 400) & (sensorValues[3] <= 800)) {
      motors.setLeftSpeed(0);
      motors.setRightSpeed(0);
      delay(20);
      if ((sensorValues[0] <= 800) & (sensorValues[1] <= 400) & (sensorValues[2] <= 400) & (sensorValues[3] <= 800)) {
        // Maak geluideje met de pieper
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        goedgekeurd = 1;
      }
    }
    if (sensorValues[2] <= 800) {

      motors.setLeftSpeed(-100);
      motors.setRightSpeed(-100);
      delay(20);
      motors.setLeftSpeed(0);
      motors.setRightSpeed(0);
    }

    if (sensorValues[3] <= 800) {

      motors.setLeftSpeed(-100);
      motors.setRightSpeed(-100);
      delay(20);
      motors.setLeftSpeed(0);
      motors.setRightSpeed(0);
    }
  
}


void setup() {

  Serial.begin(9600);
  Wire.begin();

  pinMode(volgen, INPUT);
  pinMode(zelf, INPUT);
  pinMode(userpushbuton, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  setID();

  // configure the QTR sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]) {4,5,6,11}, 4);
  qtr.setEmitterPin(2);

  digitalWrite(LED, HIGH);
  button.waitForButton();
  Serial.println("QTR calibreren....");
  //calibrerenQTR();
  Serial.println("QTR gecalibreerd");
}



void loop() {

  int uitvoerfunctie = uitvoerFunctieChecken(); // volgen = 1, zelf rijden =2
  //if(uitvoerfunctie = 1)
  int uitvoer =read_dual_sensors(); //lees waardes TOF
  Serial.println("dit is in loop");
  Serial.print(uitvoer);
  
 if (uitvoerfunctie = 2) {
  AGVcentreren();
 }
 
}
