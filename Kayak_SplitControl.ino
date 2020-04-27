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

int leftSpeed = 90; //Speed value to send to servo. Speed of 90 turns motors off.
int rightSpeed = 90; 
bool Lforward; //Flags direction of travel of left motor
bool Rforward; //---- of right motor

bool rcEnabled; //Flags when under RC via switch on controller - takes precedence over autonomy mode

//Channel values from receiver read into these variables
int ch1; //Vertical stick
int ch2; //Horizontal stick
int ch6; //Override switch

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

  //Left and right servo motor setup
  left.attach(LMPin);  // attaches the servo on pin 12 to the servo object
  right.attach(RMPin);  // attaches the servo on pin 13 to the servo object

  //Stop motors
  left.write(90);
  right.write(90);
  
  //Configure I/O pins and pin-change interrupts for reading from the transmitter
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
  //Check for override switch  
  rcOverride(); 
  if (rcEnabled) //If in user override mode
  {
    ReadRCValues(); //Update channel values in rc_store array
    ConfigureRCValues(); //Map RC values and save in global vars
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
    
    while (Serial.available() > 3) //WHile a valid command sequence available
    {
      if (Serial.read() == 'K') //Check for prefix byte
      {
        int8_t rudder = Serial.read(); //Read nav data from serial
        int8_t thrust = Serial.read();
        int8_t delim = Serial.read();
        
        if (delim == '\n') //If valid nav data sent
        {
          map(thrust, 0, 100, -255, 255); //map thrust and rudder to -255 to 255 range
          map(rudder, -180, 180, -255, 255);
          
          //Rudder and thrust correspond to Ch1 and Ch2 on standard RC remote, use differential thrust calculations from CalculateSpeed() to get motor speeds.
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
    
    if (millis() - timestamp > 2000) //If 2 seconds since last valid Serial sequence
    {
      left.write(90); //Kill motors
      right.write(90); 
      Serial.print("KNot recieving commands. Stopping.\n"); //Transmit error message
    }
  }
  //Log data
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
  
  if (rc_values[0] == 0 || rc_values[1] == 0) //Values initialized to 0 by receiver until connection established with transmitter
  { //If transmitter turned off when powered on, set to mid value constants representing speed of 90 (0 mph)
    ch1 = ch1Mid;
    ch2 = ch2Mid;
  }
  else
  {
    ch1 = constrain(rc_values[0], ch1Min, ch1Max); //Constrain the received pwm from the remote control to specified channel bounds
    ch2 = constrain(rc_values[1], ch2Min, ch2Max);
  }
  const1 = map(ch1, ch1Min, ch1Max, -255, 255); //Map these pwm signals to range -255 to 255
  const2 = map(ch2, ch2Min, ch2Max, -255, 255);
  
  if (abs(const1) <= 20) //Cutoff width of 20 to prevent small input drift from turning on motors
  { const1 = 0; } //All receiver values between -20 and 20 mapped to 0
  if (abs(const2) <= 20)
  { const2 = 0; }
}

void ReadRCValues()
{
  noInterrupts(); //disable interrupts while memory operations occuring
  memcpy(rc_values, (const void *) rc_store, sizeof(rc_store)); //copy channel pwm values from rc_values to rc_store
  interrupts(); //Enable interrupts
}

//Called when pin-change interrupt occurs on "input_pin", stores start and stop times to calculate pwm width of signal
void calc_input(uint8_t channel, uint8_t input_pin)
{
  if (digitalRead(input_pin) == HIGH)
  {
    rc_start[channel] = micros(); //Set Up time
  }
  else
  {
    rc_store[channel] = micros() - rc_start[channel]; //total PWM width is current (down) time - up time
  }
}

//Function called on Channel 1 pin change interrupt
void calc_ch1()
{ calc_input(0, 2); }

//Function called on Channel 2 pin change interrupt
void calc_ch2()
{ calc_input(1, 3); }

//Get state of override switch and update flag
void rcOverride()
{
  ch6 = pulseIn(rcOverridePin, HIGH);
  if (ch6 > 1600) //If RCOverride switch is on
  {
    if(!rcEnabled){ //If rcOverride was previously disabled
      //Enable RC Override and kill motors
      rcEnabled = true;
      leftSpeed = 90;
      rightSpeed = 90;
      WriteMotors();
    }
  }
  else //If RCOverride switch is off
  {
    if (rcEnabled) //If rcOverride was previously enabled
    {
      //Kill motors and turn off override flag
      leftSpeed = 90;
      rightSpeed = 90;
      WriteMotors();
      rcEnabled = false;
    }
  }
}

//Read PWM values from channels 1 and 2 and calculate left and right speed
void CalculateSpeed()
{
  //Calculates differential thrusts and constrains from -255 to 255
  rightSpeed = constrain((const2 - const1), -255, 255);
  leftSpeed = constrain((const2 + const1), -255, 255);

  //Maps speeds to range 0-180
  rightSpeed = map(rightSpeed, -255, 255, 0, 180);
  leftSpeed = map(leftSpeed, -255, 255, 0, 180);
}

//Send current speed values to speed constrollers
void WriteMotors()
{
  left.write(leftSpeed);
  right.write(rightSpeed);
  
}
