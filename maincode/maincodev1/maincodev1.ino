#include <QTRSensors.h>
#include <Wire.h>
#include <VL53L0X.h>

#define volgen A0
#define zelf A1
#define buzzer 3
#define userpushbuton 12
#define LED 13

/* SDA op SDA pin, SCL op SCL pin
 * buzzer pin 3, pushbutton pin 12, gele led pin 13
 * 3 way-switch pin A0, A1
 * PIN 7,8,9,10 voor motoren
 * QTR, PIN 4,5,6,11
 */


QTRSensors qtr;
const uint8_t SensorCount = 8;// aantal sensoren op de qtr
uint16_t sensorValues[SensorCount];

VL53L0X TOF;

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
  while (waarde == HIGH) {
    i++;

    waarde = digitalRead(userpushbuton);

    if (i == 50000) {
      digitalWrite(LED, HIGH);
    }

    if (i == 10000) {
      digitalWrite(LED, LOW);
      i = 0;
    }
  }

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
    if (sensorValues[0] <= 800) {
      // Drive.motor.agv(speed);
      delay(20);
      //Drive.motor.agv(0);
    }

    if (sensorValues[1] <= 800) {
      // Drive.motor.agv(speed);
      delay(20);
      // Drive.motor.agv(0);
    }

    if ((sensorValues[2] <= 800) & (sensorValues[3] <= 400) & (sensorValues[4] <= 400) & (sensorValues[5] <= 800)) {
      delay(20);
      if ((sensorValues[2] <= 800) & (sensorValues[3] <= 400) & (sensorValues[4] <= 400) & (sensorValues[5] <= 800)) {
        // Drive.motor.agv(0);
        // Maak geluideje met de pieper
        digitalWrite(LED_BUILTIN, HIGH);
        delay(1000);
        digitalWrite(LED_BUILTIN, LOW);
        goedgekeurd = 1;
      }
    }
    if (sensorValues[6] <= 800) {

      //Drive.motor.agv(-speed);
      delay(20);
      //Drive.motor.agv(0);
    }

    if (sensorValues[7] <= 800) {

      //Drive.motor.agv(-speed);
      delay(20);
      //Drive.motor.agv(0);
    }

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

  // configure the QTR sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]) {
    3, 4, 5, 6, 7, 8, 9, 10
  }, SensorCount);
  qtr.setEmitterPin(2);

  TOF.init();
  TOF.setTimeout(500);
  TOF.startContinuous();

  Serial.println("QTR calibreren....");
  calibrerenQTR();
  Serial.println("QTR gecalibreerd");
}



void loop() {

  int uitvoerfunctie = uitvoerFunctieChecken(); // volgen = 1, zelf rijden =2
  int distanceTOF = TOF.readRangeContinuousMillimeters();

  if (uitvoerfunctie = 1) {
    Serial.print("Distance: ");
    Serial.print(distanceTOF);
    Serial.print("mm");
    if (TOF.timeoutOccurred()) {
      Serial.print(" TIMEOUT");
    }
  }

 if (uitvoerfunctie = 2) {
  AGVcentreren();
 }
 
}
