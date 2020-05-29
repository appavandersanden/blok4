#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


//defines register addres van magnetometer
#include <Wire.h> //I2C Arduino Library
#define Magnetometer_mX0 0x03
#define Magnetometer_mX1 0x04
#define Magnetometer_mZ0 0x05
#define Magnetometer_mZ1 0x06
#define Magnetometer_mY0 0x07
#define Magnetometer_mY1 0x08
int mX0, mX1, mX_out;
int mY0, mY1, mY_out;
int mZ0, mZ1, mZ_out;
float Xm, Ym, Zm;
#define Magnetometer 0x1E //I2C 7bit address of HMC5883

// initialize waarde voor de functie switch
#define volgstand 9
#define autonoomstand 10

// initialize led die laat zien dat de agv aan staat
#define Ledactief 11

// initialize colorsensor voor line tracking
#define CollorSensor 12



int compas(void) {
  //---- X-Axis
  Wire.beginTransmission(Magnetometer); // transmit to device
  Wire.write(Magnetometer_mX1);
  Wire.endTransmission();
  Wire.requestFrom(Magnetometer, 1);
  if (Wire.available() <= 1)
  {
    mX0 = Wire.read();
  }
  Wire.beginTransmission(Magnetometer); // transmit to device
  Wire.write(Magnetometer_mX0);
  Wire.endTransmission();
  Wire.requestFrom(Magnetometer, 1);
  if (Wire.available() <= 1)
  {
    mX1 = Wire.read();
  }
  //---- Y-Axis
  Wire.beginTransmission(Magnetometer); // transmit to device
  Wire.write(Magnetometer_mY1);
  Wire.endTransmission();
  Wire.requestFrom(Magnetometer, 1);
  if (Wire.available() <= 1)
  {
    mY0 = Wire.read();
  }
  Wire.beginTransmission(Magnetometer); // transmit to device
  Wire.write(Magnetometer_mY0);
  Wire.endTransmission();
  Wire.requestFrom(Magnetometer, 1);
  if (Wire.available() <= 1)
  {
    mY1 = Wire.read();
  }

  //---- Z-Axis
  Wire.beginTransmission(Magnetometer); // transmit to device
  Wire.write(Magnetometer_mZ1);
  Wire.endTransmission();
  Wire.requestFrom(Magnetometer, 1);
  if (Wire.available() <= 1)
  {
    mZ0 = Wire.read();
  }
  Wire.beginTransmission(Magnetometer); // transmit to device
  Wire.write(Magnetometer_mZ0);
  Wire.endTransmission();
  Wire.requestFrom(Magnetometer, 1);
  if (Wire.available() <= 1)
  {
    mZ1 = Wire.read();
  }

  //---- X-Axis
  mX1 = mX1 << 8;
  mX_out = mX0 + mX1; // Raw data
  // From the datasheet: 0.92 mG/digit
  Xm = mX_out * 0.00092; // Gauss unit
  //* Earth magnetic field ranges from 0.25 to 0.65 Gauss, so these are the values that we need to get approximately.
  //---- Y-Axis
  mY1 = mY1 << 8;
  mY_out = mY0 + mY1;
  Ym = mY_out * 0.00092;
  //---- Z-Axis
  mZ1 = mZ1 << 8;
  mZ_out = mZ0 + mZ1;
  Zm = mZ_out * 0.00092;

  //Print out values of each axis
  Serial.print("x: ");
  Serial.print(Xm);
  Serial.print("  y: ");
  Serial.print(Ym);
  Serial.print("  z: ");
  Serial.println(Zm);
  return (mX0);

  delay(50);
}


void FunctieVolgen(int ParralelWaarde) {

  FunctieVolgenPad();
  return;
}


void FunctieVolgenPad(void) {
  int GewensteVolgafstand = 50;  // Dit is de afstand waar de AGV achter de persoon moet blijven zitten.
  int TOF1; //time of flight sensor 1
  int TOF2; //time of flight sensor 2
  int StandAGV = compas(); //stand van de agv wordt gemeten

  while(ParralelWaarde != StandAGV) {
    

  }

  while ((TOF1 > GewensteVolgafstand) && (CollorSensor == 1)) {

    //drive motors;
  }

  while ((TOF2 > GewensteVolgafstand) && (CollorSensor == 1)) {

    //drive motors;
  }



  return;
}

}
void FunctieAutonoom(void) {

  return;
}


// Startwaarden van de compas als parralelwaarden zetten
const int ParralelWaarde = compas();

void setup() {
  //Initialize Serial and I2C communications for magnetometer
  Serial.begin(9600);
  Wire.begin();
  delay(100);

  Wire.beginTransmission(Magnetometer);
  Wire.write(0x02); // Select mode register
  Wire.write(0x00); // Continuous measurement mode
  Wire.endTransmission();

  //Alle pins op input of output zetten
  pinMode(volgstand, INPUT);
  pinMode(autonoomstand, INPUT);
  pinMode(Ledactief, OUTPUT);


  // Led actief aanzetten omdat de arduiono opstart
  digitalWrite(Ledactief, HIGH);

  // Startwaarden van de compas als parralelwaarden zetten


}




void loop() {


  if (volgstand == 1) {

    FunctieVolgen(ParralelWaarde);
  }

  if (autonoomstand == 1) {

    FunctieAutonoom();
  }


}
