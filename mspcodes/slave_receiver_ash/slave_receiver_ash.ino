// Wire Slave Receiver for Arm
// by RationalAsh
// Created 4 December 2014

#include <Wire.h>
#include <PID_v1.h>

#define LED RED_LED
#define POSFB A4
#define CONOUT P2_2
#define MA P2_0
#define MB P2_1

#define REV -1
#define FWD 1
#define STOP 0

//Define Variables we'll be connecting to
double setPoint = 30, input, output;
//The PID constants
double Kp=2, Ki=0, Kd=0;
double angle = 30;

//To debug or not to debug
boolean debugFlag = true;
boolean serialFlag = false;

PID armcontroller(&input, &output, &setPoint, Kp,Ki,Kd, DIRECT);

//Function to set motor direction
void setDir(int D)
{
  if(D==FWD)
  {
    digitalWrite(MA, HIGH);
    digitalWrite(MB, LOW);
  }
  else if(D==REV)
  {
    digitalWrite(MA, LOW);
    digitalWrite(MB, HIGH);
  }
  else if(D==STOP)
  {
    digitalWrite(MA, HIGH);
    digitalWrite(MB, HIGH);
  }
}

void setup()
{
  pinMode(LED, OUTPUT);
  Wire.begin(1);                // join i2c bus with a given address
  Wire.onReceive(receiveEvent); // register event
  pinMode(POSFB, INPUT);
  input = analogRead(POSFB);
  //Turn on PID
  armcontroller.SetMode(AUTOMATIC);
  armcontroller.SetOutputLimits(-255, 255);
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  //Read the analog feedback value
  input = map(analogRead(POSFB), 0, 1023, 0,270);
  armcontroller.Compute();
  
  if(output <= 0)
  {
    setDir(FWD);
  }
  else if(output >0)
  {
    setDir(REV);
  }
  analogWrite(CONOUT, abs(output));
  
  //Debug outputs
  if(debugFlag)
  {
    Serial.print("Target: ");
    Serial.print(setPoint, 4);
    Serial.print(" | PVal: ");
    Serial.print(input, 4);
    Serial.print(" | PWM: ");
    Serial.print(output, 4);
    Serial.print(" | Err: ");
    Serial.print(setPoint - input, 4);
    Serial.print(" | Dir: ");
    if(output <= 0) Serial.println("REV");
    else if(output > 0) Serial.println("FWD");
  }
  
  if(serialFlag)
  {
    Serial.print("Got angle: ");
    Serial.println(angle, 4);
    setPoint = angle;
    if(setPoint > 180) setPoint = 180;
    serialFlag = false;
  }
  
  delay(50);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  int x = Wire.read();    // receive byte as an integer
  //Serial.println(x);         // print the integer
  int ldstat = x%2;
  digitalWrite(LED, ldstat);
}

//Function that executes whenever Serial Data is received
void serialEvent() {
  while (Serial.available()>0) 
  {
    angle = Serial.parseInt();
  }
  int ldstat = int(angle)%2;
  digitalWrite(LED, ldstat);
  serialFlag = true;
}
