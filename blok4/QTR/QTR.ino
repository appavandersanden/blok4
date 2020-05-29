#include <QTRSensors.h>

// Dit voorbeeld is ontworpen voor gebruik met acht RC QTR-sensoren. Deze
// reflectiesensoren moeten worden aangesloten op digitale pinnen 3 tot 10. De
// sensoren 'emitter control pin (CTRL of LEDON) kan optioneel worden aangesloten op
// digitale pin 2, of u kunt de verbinding verbroken laten en de oproep naar setEmitterPin () verwijderen.
// setEmitterPin ().
//
// De installatiefase van dit voorbeeld kalibreert de sensoren gedurende tien seconden en
// zet de LED van de Arduino aan (meestal op pin 13) terwijl de kalibratie bezig is
// Aan. Tijdens deze fase moet u elke reflectiesensor blootstellen aan de
// lichtste en donkerste metingen die ze zullen tegenkomen. Als je dat bijvoorbeeld bent
// Als u een lijnvolger maakt, moet u de sensoren tijdens de lijn over de lijn schuiven
// de kalibratiefase zodat elke sensor kan aflezen hoe donker de
// lijn is en hoe licht de grond is. Onjuiste kalibratie zal resulteren in
// slechte metingen.
//
// De hoofdlus van het voorbeeld leest de gekalibreerde sensorwaarden en gebruikt
// hen om de positie van een lijn te schatten. Je kunt dit testen door een stuk af te plakken
// van 3/4 "zwarte elektrische tape op een stuk wit papier en schuif de
// sensor eroverheen. Het print de sensorwaarden naar de seriële monitor als
// getallen van 0 (maximale reflectie) tot 1000 (minimale reflectie) gevolgd
// door de geschatte locatie van de lijn als een getal van 0 tot 5000. 1000 betekent
// de lijn ligt direct onder sensor 1, 2000 betekent direct onder sensor 2,
// etc. 0 betekent dat de lijn zich direct onder sensor 0 bevindt of voor het laatst door sensor is gezien
// 0 voordat ze verloren gaan. 5000 betekent dat de lijn zich direct onder sensor 5 bevindt of was
// laatst gezien door sensor 5 voordat deze verloren is gegaan

QTRSensors qtr;

const uint8_t SensorCount = 8;// aantal sensoren op de qtr
uint16_t sensorValues[SensorCount];

///////////////////////////////////////////////
int callibrateQTR(void) {

  delay(500);

  digitalWrite(LED_BUILTIN, HIGH); // led aanzetten om te laten zien dat de qtr sensor gaat calibreren

  //hier gaat de qtr sensor calibreren. Probeer voor elke sensor een potje en geen potje te zetten. Hier kan hij waarders
  // bepalen van hoe erg wat reflecteerd. Deze calicibratie duurt 10 seconden. Uiteindleijk gaat de led uit wanneer hij klaar is met
  //calibreren.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED

  // print the calibration minimum values measured when emitters were on

  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}



///////////////////////////////////////
void setup()
{

  //voor wanneer dit nog niet staat ingesteld
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // configure the QTR sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]) { 3, 4, 5, 6, 7, 8, 9, 10 }, SensorCount);
  qtr.setEmitterPin(2);

  callibrateQTR();
}

//////////////////////////////////////
void loop()
{ 
  int goedgekeurd = 0; // wanneer het potje in het midden van de qtr sensor staat wordt dit veranderdt naar 1 en gaat hij door;

  while(goedgekeurd = 0){
    
 
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

// hier proberen we de agv voor de qtr sensor te centreren. Misschien kunnen we hier een apparte functie voor maken.
  if (sensorValues[1] <= 800){
   // Drive.motor.agv(speed);
    delay(20);
    //Drive.motor.agv(0);
  }
  
   if (sensorValues[2] <= 800){
   // Drive.motor.agv(speed);
    delay(20);
   // Drive.motor.agv(0);
  }

   if (sensorValues[3] <= 800){

    //Drive.motor.agv(speed);
    delay(20);
   // Drive.motor.agv(0);
  }
  
  if ((sensorValues[4] <= 800) & (sensorValues[5]<= 800)){

  // Maak geluideje met de pieper
  delay(2000);
  goedgekeurd = 1;
  }
  
   if (sensorValues[6] <= 800){

    //Drive.motor.agv(-speed);
    delay(20);
    //Drive.motor.agv(0);
  }
  
   if (sensorValues[7] <= 800){

    //Drive.motor.agv(-speed);
    delay(20);
    //Drive.motor.agv(0);
  }

  if (sensorValues[8] <= 800){

    //Drive.motor.agv(-speed);
    delay(20);
    //Drive.motor.agv(0);
  }
  
  }
 }
