// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

String inputString = "";
boolean stringComplete = false;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  inputString.reserve(30);
}

byte x = 0;

void loop()
{
  //x = Serial.parseInt();
  if(stringComplete)
  {
    Serial.println("GotString!");
    x = int(inputString[0]);
    Serial.print("Sending: ");
    Serial.println(x);
    
    Wire.beginTransmission(1);
    Wire.write(x);
    Wire.endTransmission();
    
    Wire.beginTransmission(2);
    Wire.write(x);
    Wire.endTransmission();
    
    Wire.beginTransmission(3);
    Wire.write(x);
    Wire.endTransmission();
    
    inputString = "";
    stringComplete = false;
  }
 
  //Wire.beginTransmission(4); // transmit to device #4
  //Wire.write("x is ");        // sends five bytes
  //Wire.write(x);              // sends one byte  
  //Wire.endTransmission();    // stop transmitting

  x++;
  delay(500);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
    stringComplete = true;
    //Serial.println("GotString!feifeifjiefj");
    } 
  }
}
