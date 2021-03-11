#include <NewPing.h>
#include <EnableInterrupt.h>
#include <Servo.h>

Servo left;
Servo right;

#define ch1Pin 2
#define ch2Pin 3
#define ch4Pin 4
#define LMPin 12
#define RMPin 13

#define RedPin 9
#define GreenPin 10
#define BluePin 11

#define ch1Min 1000
#define ch2Min 1000
#define ch4Min 1000


#define ch1Mid 1500
#define ch2Mid 1500
#define ch4Mid 1500


#define ch1Max 2000
#define ch2Max 2000
#define ch4Max 2000


#define rcOverridePin 6 //Next available interrupt pin

int leftSpeed = 90;
int rightSpeed = 90;
int lCompens = 0;
bool Lforward;
bool Rforward;

bool rcEnabled;

int ch1; // Here's where we'll keep our channel values
int ch2;
int ch4;
int ch6;

long int timestamp = 0;

//Channel up time
uint32_t rc_start[3];
uint16_t rc_values[3];
uint16_t rc_store[3];

int const1;
int const2;

unsigned long start;
unsigned long endT;
unsigned long delta;

int lightStatus;

void setup()
{
  // put your setup code here, to run once:
  rcEnabled = false;
  
  left.attach(12);  // attaches the servo on pin 12 to the servo object
  right.attach(13);  // attaches the servo on pin 13 to the servo object
  
  left.write(90);
  right.write(90);
  
  lightStatus = 0; //Lights off
  
  pinMode(ch1Pin, INPUT);
  pinMode(ch2Pin, INPUT);
  pinMode(ch4Pin,INPUT);
  pinMode(rcOverridePin, INPUT);
  pinMode(LMPin, OUTPUT);
  pinMode(RMPin, OUTPUT);
  pinMode(RedPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  enableInterrupt(ch1Pin, calc_ch1, CHANGE);
  enableInterrupt(ch2Pin, calc_ch2, CHANGE);
  
  Serial.begin(115200);
  delay(1000);
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  rcOverride();
  if (rcEnabled)
  {
    lightStatus = 1; //Blue = RC state
    ReadRCValues();
    ReadTrim();
    ConfigureRCValues();
    CalculateSpeed(); //Calculate motor speeds from ch1 & 2
    WriteMotors(); //Write speeds to motors and determine forward/reverse

    Serial.print('K');
    Serial.print(rcEnabled);
    Serial.print(',');
    Serial.print(leftSpeed);
    Serial.print(',');
    Serial.print(rightSpeed);
    Serial.print('\n');
    Serial.print('C');
    Serial.print(lCompens);
    Serial.print('\n');
  }
  
  
  else
  { 
    while (Serial.available() > 4)
    {      
      if (Serial.read() == 'K')
      {
        int8_t rudder = Serial.read();
        int8_t thrust = Serial.read();
        int8_t status = Serial.read();
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
          switch (status)
          {
          case 3:
            lightStatus = 2; //Green - autonomy, all sensors active
            break;
          case 1 ... 2:
            lightStatus = 3; //Yellow - autonomy, 1 sensor fault
            break;
          default:
            lightStatus = 4; //Red - Complete sensor failure
            break;   
          }
        }
      }
    }
    
    //delay(250); //Not sure why this is here at the moment
    
    if (millis() - timestamp > 2000)
    {
      left.write(90);
      right.write(90);
      Serial.print("KNot recieving commands. Stopping.\n");
      lightStatus = 4; // Red - Serial communication fault
    }
  }

  UpdateLights();

  Serial.print('K');
  Serial.print(rcEnabled);
  Serial.print(',');
  Serial.print(lightStatus);
  Serial.print(',');
  Serial.print(leftSpeed);
  Serial.print(',');
  Serial.print(rightSpeed);
  Serial.print('\n');
}

void ReadTrim(){
  ch4 = pulseIn(4, HIGH);
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
    ch4 = constrain(ch4,ch4Min,ch4Max);
  }
  const1 = map(ch1, ch1Min, ch1Max, -255, 255);
  const2 = map(ch2, ch2Min, ch2Max, -255, 255);
  lCompens = map(ch4, ch4Min, ch4Max, 0, 110);
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
  if ((ch6 > 1600)&&(!rcEnabled))
  {
    rcEnabled = true;
    leftSpeed = 90;
    rightSpeed = 90;
    //Write Motors will be called in the loop, just set speeds back to 0 and let it be written later
    //WriteMotors();
    
  }
  else if(ch6 < 1600 && rcEnabled)
  {
    leftSpeed = 90;
    rightSpeed = 90;
    //WriteMotors();
    rcEnabled = false;
  }
}

void CalculateSpeed()
{
  int rightMin = -255 + lCompens;
  int rightMax = 255 - lCompens;
  rightSpeed = constrain((const2 - const1), rightMin, rightMax);
  leftSpeed = constrain((const2 + const1), -255, 255);
  if(abs(leftSpeed) > 5){
    if(leftSpeed > 0)
      leftSpeed = min(255,leftSpeed+lCompens);
    if(leftSpeed < 0)
      leftSpeed = max(-255,leftSpeed-lCompens);
  }
  rightSpeed = map(rightSpeed, -255, 255, 17, 163);
  leftSpeed = map(leftSpeed, -255, 255, 17, 163);
  /*Serial.print("Left: ");
  Serial.println(leftSpeed);
  Serial.print("Right: ");
  Serial.println(rightSpeed);*/
}

void WriteMotors()
{
  left.write(leftSpeed);
  right.write(rightSpeed);
}

void UpdateLights()
{
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;

  switch(lightStatus){
    case 0: //Off
      Red = 0;
      Green = 0;
      Blue = 0;
      break;
    case 1: //Blue
      Red = 0;
      Green = 0;
      Blue = 255;
      break;
    case 2: //Green
      Red = 0;
      Green = 255;
      Blue = 0;
      break;
    case 3: //Yellow
      Red = 255;
      Green = 255;
      Blue = 0;
      break;
    case 4: //Red
      Red = 255;
      Green = 0;
      Blue = 0;
      break;
    default:
      lightStatus = 0;
      Red = 0;
      Green = 0;
      Blue = 0;
      break;
  }
  analogWrite(RedPin,Red);
  analogWrite(GreenPin,Green);
  analogWrite(BluePin,Blue);
}
