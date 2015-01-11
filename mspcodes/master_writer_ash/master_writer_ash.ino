// Wire Master Writer for Arm
// by RationalAsh
// Created 4 December 2014

#include <Wire.h>

//Array to store the incoming data
volatile int angles[5] = {0, 0, 0, 0, 0};
boolean serialFlag = false;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  Serial.setTimeout(200);
}

void loop()
{
  if(serialFlag)
  {  
  //Transmit the angles to the ICs
  Wire.beginTransmission(1);
  Wire.write(byte(angles[0]));
  Wire.endTransmission();
  
  Wire.beginTransmission(2);
  Wire.write(byte(angles[1]));
  Wire.endTransmission();
    
  Wire.beginTransmission(3);
  Wire.write(byte(angles[2]));
  Wire.endTransmission();
  
  Wire.beginTransmission(4);
  Wire.write(byte(angles[3]));
  Wire.endTransmission();
 
  Wire.beginTransmission(5);
  Wire.write(byte(angles[4]));
  Wire.endTransmission();
  
  Serial.println("Got angles!");
  Serial.print("m1: ");
  Serial.print(angles[0]);
  Serial.print(" m2: ");
  Serial.print(angles[1]);
  Serial.print(" m3: ");
  Serial.print(angles[2]);
  Serial.print(" m4: ");
  Serial.print(angles[3]);
  Serial.print(" m5: ");
  Serial.println(angles[4]);

  serialFlag = false;  
  }  

  //introduce delay
  delay(10);
}

void serialEvent() {
  while (Serial.available()>0) 
  {
    angles[0] = Serial.parseInt();
    angles[1] = Serial.parseInt();
    angles[2] = Serial.parseInt();
    angles[3] = Serial.parseInt();
    angles[4] = Serial.parseInt();
  }
  serialFlag = true;
}
