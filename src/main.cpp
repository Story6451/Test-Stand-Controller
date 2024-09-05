#include <Arduino.h>
#include <Servo.h>

//servo pins
const uint8_t OxServoPin = 9;
const uint8_t FuServoPin = 10;

//led pins
const uint8_t ServoArmingLEDPin = 2;
const uint8_t IgniterArmingLEDPin = 3;
const uint8_t TestFiringArmingLEDPPin = 4;

//switch pins
const uint8_t ServoArmingSwitchPin = 5;
const uint8_t IgniterArmingSwitchPin = 6;
const uint8_t ServoSwitchPin = 7;
const uint8_t IgniterSwitchPin = 8;
const uint8_t TestFireSwitchPin = 12;

//other pins
const uint8_t IgniterPin = 13;

bool ServoArmed = false;
bool IgniterArmed = false;
bool TestFiringArmed = false;

Servo FuServo;
Servo OxServo;

void setup()
{
  Serial.begin(9600);
  FuServo.attach(FuServoPin);
  OxServo.attach(OxServoPin);

  pinMode(IgniterPin, OUTPUT);
  pinMode(ServoArmingSwitchPin, INPUT);
  pinMode(IgniterArmingSwitchPin, INPUT);
  //pinMode(TestFiringArmingSwitchPin, INPUT);
  pinMode(ServoSwitchPin, INPUT);
  pinMode(IgniterSwitchPin, INPUT);
  pinMode(TestFireSwitchPin, INPUT);

  pinMode(ServoArmingLEDPin, OUTPUT);
  pinMode(IgniterArmingLEDPin, OUTPUT);
  pinMode(TestFiringArmingLEDPPin, OUTPUT);

  delay(500);
  digitalWrite(ServoArmingLEDPin, HIGH);
  digitalWrite(IgniterArmingLEDPin, HIGH);
  digitalWrite(TestFiringArmingLEDPPin, HIGH);
  delay(200);
  digitalWrite(ServoArmingLEDPin, LOW);
  digitalWrite(IgniterArmingLEDPin, LOW);
  digitalWrite(TestFiringArmingLEDPPin, LOW);
  delay(200);
  digitalWrite(ServoArmingLEDPin, HIGH);
  digitalWrite(IgniterArmingLEDPin, HIGH);
  digitalWrite(TestFiringArmingLEDPPin, HIGH);
  delay(200);
  digitalWrite(ServoArmingLEDPin, LOW);
  digitalWrite(IgniterArmingLEDPin, LOW);
  digitalWrite(TestFiringArmingLEDPPin, LOW);
  delay(200);
  digitalWrite(ServoArmingLEDPin, HIGH);
  digitalWrite(IgniterArmingLEDPin, HIGH);
  digitalWrite(TestFiringArmingLEDPPin, HIGH);
  delay(200);
  digitalWrite(ServoArmingLEDPin, LOW);
  digitalWrite(IgniterArmingLEDPin, LOW);
  digitalWrite(TestFiringArmingLEDPPin, LOW);
 
}

void loop() 
{
  //read servo arming button
  uint8_t ServoArmingSwitchState = digitalRead(ServoArmingSwitchPin);
  if (ServoArmingSwitchState == 1)
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
  uint8_t IgniterArmingSwitchState = digitalRead(IgniterArmingSwitchPin);
  if (IgniterArmingSwitchState == 1)
  {
    IgniterArmed = true;
    digitalWrite(IgniterArmingLEDPin, HIGH);
  }
  else
  {
    IgniterArmed = false;
    digitalWrite(IgniterArmingLEDPin, LOW);
  }
  
  
  if ((ServoArmed == true) && (IgniterArmed == true))
  {
    TestFiringArmed = true;
    digitalWrite(TestFiringArmingLEDPPin, HIGH);
  }
  else
  {
    TestFiringArmed = false;
    digitalWrite(TestFiringArmingLEDPPin, LOW);
  }
  
  Serial.print(ServoArmingSwitchState);Serial.println(IgniterArmingSwitchState);
  //////////////////////////////////////////////////////////////////////////////////////
  //Servo safty logic
  
  if ((ServoArmed && !(IgniterArmed || TestFiringArmed)) == true)
  {
    uint8_t ServoState = digitalRead(ServoSwitchPin);
    if (ServoState == 1)
    {
      Serial.print("Servo");
      FuServo.write(90);
      OxServo.write(90);
    }
    else
    {
      FuServo.write(0);
      OxServo.write(0);
    }
  }

  //Igniter safty logic
  if ((IgniterArmed && !(ServoArmed || TestFiringArmed)) == true)
  {
    uint8_t IgniterState = digitalRead(IgniterSwitchPin);
    if (IgniterState == 1)
    {
      Serial.print("Igniter");
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
    uint8_t TestFireState = digitalRead(TestFireSwitchPin);
    Serial.print("Test firing armed"); Serial.print(TestFireState);
    if (TestFireState == 1)
    {
      Serial.println("Started test fire!!!");
      FuServo.write(90);
      OxServo.write(90);
      delay(250);
      digitalWrite(IgniterPin, HIGH);
      delay(1000);
      digitalWrite(IgniterPin, LOW);

      //allows for controller to abort test fire by closing the valves if the armed switch is released
      uint64_t StartTime = millis();
      while((TestFiringArmed == true) && ((millis() - StartTime) < 5000))
      {
        Serial.println("Firing!!!!");
        FuServo.write(90);
        OxServo.write(90);

        TestFireState = digitalRead(TestFireSwitchPin);
        if (TestFireState == 1)
        {
          TestFiringArmed = true;
        }
        else
        {
          TestFiringArmed = false;
        }
      }
      FuServo.write(0);
      OxServo.write(0);
      Serial.println("Finished test fire!!!");
      delay(10000);
      while(1);
    }
    else
    {
      FuServo.write(0);
      OxServo.write(0);
      digitalWrite(IgniterPin, LOW);
    }
  }
  
  
}