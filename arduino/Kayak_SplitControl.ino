#include <NewPing.h>
#include <EnableInterrupt.h>
#include <Servo.h>

Servo left;
Servo right;

#define ch1Pin 2
#define ch2Pin 3

#define LMPin 12
#define RMPin 13

#define ch1Min 1000
#define ch2Min 1000

#define ch1Mid 1500
#define ch2Mid 1500

#define ch1Max 2000
#define ch2Max 2000

#define rcOverridePin 6 //Next available interrupt pin

int leftSpeed = 90;
int rightSpeed = 90;
bool Lforward;
bool Rforward;

bool rcEnabled;

int ch1; // Here's where we'll keep our channel values
int ch2;
int ch6;

long int timestamp = 0;

//Channel up time
uint32_t rc_start[2];
uint16_t rc_values[2];
uint16_t rc_store[2];

int const1;
int const2;

unsigned long start;
unsigned long endT;
unsigned long delta;


void setup()
{
  // put your setup code here, to run once:
  rcEnabled = false;
  
  left.attach(12);  // attaches the servo on pin 12 to the servo object
  right.attach(13);  // attaches the servo on pin 13 to the servo object
  
  left.write(90);
  right.write(90);
  
  
  pinMode(ch1Pin, INPUT);
  pinMode(ch2Pin, INPUT);
  pinMode(rcOverridePin, INPUT);
  pinMode(LMPin, OUTPUT);
  pinMode(RMPin, OUTPUT);
  enableInterrupt(ch1Pin, calc_ch1, CHANGE);
  enableInterrupt(ch2Pin, calc_ch2, CHANGE);
  Serial.begin(115200);
  delay(5000);
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  rcOverride();
  if (rcEnabled)
  {
    ReadRCValues();
    ConfigureRCValues();
    CalculateSpeed(); //Calculate motor speeds from ch1 & 2
    WriteMotors(); //Write speeds to motors and determine forward/reverse

//    Serial.print('K');
//    Serial.print(rcEnabled);
//    Serial.print(',');
//    Serial.print(leftSpeed);
//    Serial.print(',');
//    Serial.print(rightSpeed);
//    Serial.print('\n');
  }
  
  
  else
  {
    
    while (Serial.available() > 3)
    {
      if (Serial.read() == 'K')
      {
        int8_t rudder = Serial.read();
        int8_t thrust = Serial.read();
        int8_t delim = Serial.read();
        
        if (delim == '\n')
        {
          map(thrust, 0, 100, -255, 255);
          map(rudder, -180, 180, -255, 255);
          
          const1 = rudder;
          const2 = thrust;
          CalculateSpeed();
          
          WriteMotors();

//            Serial.print("KWrote data\n");
          
          timestamp = millis();
        }
      }
    }
    
    delay(250);
    
    if (millis() - timestamp > 2000)
    {
      left.write(90);
      right.write(90);
      Serial.print("KNot recieving commands. Stopping.\n");
    }
  }
  Serial.print('K');
  Serial.print(rcEnabled);
  Serial.print(',');
  Serial.print(leftSpeed);
  Serial.print(',');
  Serial.print(rightSpeed);
  Serial.print('\n');
}

void ConfigureRCValues()
{
  
  if (rc_values[0] == 0 || rc_values[1] == 0)
  { //If transmitter turned off when powered on
    ch1 = ch1Mid;
    ch2 = ch2Mid;
  }
  else
  {
    ch1 = constrain(rc_values[0], ch1Min, ch1Max);
    ch2 = constrain(rc_values[1], ch2Min, ch2Max);
  }
  const1 = map(ch1, ch1Min, ch1Max, -255, 255);
  const2 = map(ch2, ch2Min, ch2Max, -255, 255);
  
  if (abs(const1) <= 20)
  { const1 = 0; }
  if (abs(const2) <= 20)
  { const2 = 0; }
}

void ReadRCValues()
{ //moves rc_store to rc_values while interrupts disabled
  noInterrupts();
  memcpy(rc_values, (const void *) rc_store, sizeof(rc_store));
  interrupts();
}

void calc_input(uint8_t channel, uint8_t input_pin)
{
  if (digitalRead(input_pin) == HIGH)
  {
    rc_start[channel] = micros();
  }
  else
  {
    rc_store[channel] = micros() - rc_start[channel];
  }
}

void calc_ch1()
{ calc_input(0, 2); }

void calc_ch2()
{ calc_input(1, 3); }

void rcOverride()
{
  ch6 = pulseIn(rcOverridePin, HIGH);
  if (ch6 > 1600)
  {
    rcEnabled = true;
    leftSpeed = 90;
    rightSpeed = 90;
    WriteMotors();
  }
  else
  {
    if (rcEnabled)
    {
      leftSpeed = 90;
      rightSpeed = 90;
      WriteMotors();
    }
    rcEnabled = false;
  }
}

void CalculateSpeed()
{
  rightSpeed = constrain((const2 - const1), -255, 255);
  leftSpeed = constrain((const2 + const1), -255, 255);
  
  rightSpeed = map(rightSpeed, -255, 255, 0, 180);
  leftSpeed = map(leftSpeed, -255, 255, 0, 180);
}

void WriteMotors()
{
  left.write(leftSpeed);
  right.write(rightSpeed);
  
}
