// Wire Master Writer for Arm
// by RationalAsh
// Created 4 December 2014

#include <Wire.h>

//String to store the incoming data
String inputString = "";
boolean stringComplete = false;
int angles[5];

byte x;
byte y[3];

byte ctob(char ch)
{
  byte ans = byte(ch) - 48;
  return ans;
}

byte bstoint(byte one=0, byte ten=0, byte hund=0)
{
  byte ans = (one*1 + ten*10 + hund*100);
  if(ans <= 255)  return ans;
  else return 0;
}

byte getAngle(String inputStr)
{
  int i = inputStr.length() - 1;
  
  if(i==1)
    { 
      y[2] = ctob(inputStr[0]);
    }
    else if(i==2)
    {
      y[2] = ctob(inputStr[1]);
      y[1] = ctob(inputStr[0]);
    }
    else if(i==3)
    {
      y[2] = ctob(inputStr[2]);
      y[1] = ctob(inputStr[1]);
      y[0] = ctob(inputStr[0]);
    }
    
    byte ans = bstoint(y[2], y[1], y[0]);
    
    return ans;
}

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  inputString.reserve(30);
}

void loop()
{
  //If string received, then transmit data to the slave ICs
  if(stringComplete)
  {
    y[0] = 0; y[1] = 0; y[2] = 0;
    int i = inputString.length() - 1;
    Serial.println("GotString!");
    if(i==1)
    { 
      y[2] = ctob(inputString[0]);
    }
    else if(i==2)
    {
      y[2] = ctob(inputString[1]);
      y[1] = ctob(inputString[0]);
    }
    else if(i==3)
    {
      y[2] = ctob(inputString[2]);
      y[1] = ctob(inputString[1]);
      y[0] = ctob(inputString[0]);
    }
      
    //x = bstoint(y[2], y[1], y[0]);
    x = getAngle(inputString);
    Serial.print("Sending: ");
    Serial.println(x);
    Serial.print("Length: ");
    Serial.println(i);
    
    //Transmit the angles to the ICs
    Wire.beginTransmission(3);
    Wire.write(x);
    Wire.endTransmission();
    
    Wire.beginTransmission(4);
    Wire.write(x);
    Wire.endTransmission();
    
    //Wire.beginTransmission(3);
    //Wire.write(x);
    //Wire.endTransmission();
    
    //Wire.beginTransmission(3);
    //Wire.write(x);
    //Wire.endTransmission();
    
    //Wire.beginTransmission(3);
    //Wire.write(x);
    //Wire.endTransmission();
    
    inputString = "";
    stringComplete = false;
  }

  //introduce delay
  //x++;
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
    } 
  }
}
