#include <Arduino.h>
#include <Servo.h>

const uint8_t OxServoPin = 1;
const uint8_t FuServoPin = 2;
const uint8_t IgniterPin = 3;
const uint8_t ServoArmingButtonPin = 4;//
const uint8_t ServoArmingLEDPin = 5;//
const uint8_t IgniterArmingButtonPin = 6;//
const uint8_t IgniterArmingLEDPin = 7;//
const uint8_t TestFiringArmingButtonPin = 8;//
const uint8_t TestFiringArmingLEDPPin = 9;//
const uint8_t OxServoButtonPin = 10;//
const uint8_t FuServoButtonPin = 11;//
const uint8_t IgniterButtonPin = 12;//
const uint8_t TestFireButtonPin = 13;//

bool ServoArmed = false;
bool IgniterArmed = false;
bool TestFiringArmed = false;

Servo FuServo;
Servo OxServo;

void setup()
{
  FuServo.attach(FuServoPin);
  OxServo.attach(OxServoPin);

  pinMode(IgniterPin, OUTPUT);
  pinMode(ServoArmingButtonPin, INPUT);
  pinMode(IgniterArmingButtonPin, INPUT);
  pinMode(TestFiringArmingButtonPin, INPUT);
  pinMode(OxServoButtonPin, INPUT);
  pinMode(FuServoButtonPin, INPUT);
  pinMode(IgniterButtonPin, INPUT);
  pinMode(TestFireButtonPin, INPUT);

  pinMode(ServoArmingLEDPin, OUTPUT);
  pinMode(IgniterArmingLEDPin, OUTPUT);
  pinMode(TestFiringArmingLEDPPin, OUTPUT);
  
}

void loop() 
{
  //read servo arming button
  uint8_t ServoArmingButtonState = digitalRead(ServoArmingButtonPin);
  if (ServoArmingButtonState == 1)
  {
    ServoArmed = true;
    digitalWrite(ServoArmingLEDPin, HIGH);
  }
  else
  {
    ServoArmed = false;
    digitalWrite(ServoArmingLEDPin, LOW);
  }
  //read igniter arming button
  uint8_t IgniterArmingButtonState = digitalRead(IgniterArmingButtonPin);
  if (IgniterArmingButtonState == 1)
  {
    IgniterArmed = true;
    digitalWrite(IgniterArmingLEDPin, HIGH);
  }
  else
  {
    IgniterArmed = false;
    digitalWrite(IgniterArmingLEDPin, LOW);
  }
  //read test firing arming button
  uint8_t TestfiringButtonState = digitalRead(TestFiringArmingButtonPin);
  if (TestfiringButtonState == 1)
  {
    TestFiringArmed = true;
    digitalWrite(TestFiringArmingLEDPPin, HIGH);
  }
  else
  {
    TestFiringArmed = false;
    digitalWrite(TestFiringArmingLEDPPin, LOW);
  }

  //Servo safty logic
  if ((ServoArmed && !(IgniterArmed || TestFiringArmed)) == true)
  {
    uint8_t FuServoState = digitalRead(FuServoButtonPin);
    uint8_t OxServoState = digitalRead(OxServoButtonPin);

    if (FuServoState == 1)
    {
      FuServo.write(90);
    }
    else
    {
      FuServo.write(0);
    }
    if (OxServoState == 1)
    {
      OxServo.write(90);
    }
    else
    {
      OxServo.write(0);
    }
  }

  //Igniter safty logic
  if ((ServoArmed && !(IgniterArmed || TestFiringArmed)) == true)
  {
    uint8_t IgniterState = digitalRead(IgniterButtonPin);

    if (IgniterState == 1)
    {
      digitalWrite(IgniterPin, HIGH);
    }
    else
    {
      digitalWrite(IgniterPin, LOW);
    }
  }

  //Testfiring safty logic
  if (TestFiringArmed == true)
  {
    uint8_t TestFireState = digitalRead(TestFireButtonPin);
    
    if (TestFireState == 1)
    {
      FuServo.write(90);
      OxServo.write(90);
      delay(250);
      digitalWrite(IgniterPin, HIGH);
      delay(1000);
      digitalWrite(IgniterPin, LOW);
      delay(5000);
    }
    else
    {
      FuServo.write(0);
      OxServo.write(0);
      digitalWrite(IgniterPin, LOW);
    }
  }
}