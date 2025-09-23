/// PREDELANO iz 4DOF v 2DOF !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define RV HIGH  //beware it's depending on your hardware wiring
#define FW  LOW //beware it's depending on your hardware wiring

////////////////////////////////////////////////////////////////////////////////
#define pwmMax 255 // or less, if you want to lower the maximum motor's speed

// ratio to amplify pot range (Right or Left) if the pot range is too short (-20� +20� for example) 
// too high ratio may amplify jittering (use high quality cermet pot ! and if ou can use mechanical gears between motor and pot) 
#define gearMotorRorLPotRatio 1 

////////////////////////////////////////////////////////////////////////////////
// defining the motors id

#define motLeft 0
#define motRight 1
// defining a shorter authorized range (than 0-1024) for motors along the considered axe
// it's depending on your pot mounting ratio and the max acceptable amplitude of mouvement your sim can handle
const int potMiniRorL=50; 
const int potMaxiRorL=950;   //  definira max pozicije potenciometra (VARNOSTNA) omejitev

////////////////////////////////////////////////////////////////////////////////
//  DECLARATIONS
////////////////////////////////////////////////////////////////////////////////
/*  pin definitions*/
//  PWM pin : 3, 5, 6, 9, 10, and 11. Provide 8-bit PWM output with the analogWrite() function.

#define potL A0
#define potR A1

int DIRpin[2] = {
  7, 5};  // if HIGH/LOW then motor runs Forward/Reverse (depending on wiring)
int PWMpin[2] = {
  9, 8}; // PWM vary between 1 (slow speed) and 255 (fast speed)

/* init position value*/
int DataValueL=550; //middle position   zadn lev   ce gledas od uzad
int DataValueR=550; //middle position   zadn desn

////////////////////////////////////////////////////////////////////////////////
// INITIALIZATION
////////////////////////////////////////////////////////////////////////////////
void setup()
{
  // serial initialization
  Serial.begin(115200);

  // initialization of Arduino's pins
  pinMode(potL, INPUT);
  pinMode(potR, INPUT);
  for (int i=0; i<2; i++)
  {
    pinMode(DIRpin[i], OUTPUT);
    pinMode(PWMpin[i], OUTPUT);
  }
  // Initialize braked for motor
  for (int i=0; i<2; i++)
  {
    digitalWrite(PWMpin[i], 0);     
    digitalWrite(DIRpin[i], LOW);
  }

  //  testMotor2(0);

}
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Main Loop ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void loop()
{
  // if the loop is quit, the motors have to be stopped by calling the stop function (if not they would continue to turn independantly of the pot feedback)
  int sensorL,sensorR; // sensorW

  // receiving the game's data
  readSerialData();   // DataValues are stored in a circular buffer
  // If no newer orders are in the serial buffer, the last one is kept
  // And this previous order will still be used by the PID regulation MotorMotion Function

  // reading the pots values

  /* Arduino is reading the pot value through its analogic pin
   pot       O ohm <------> 10 000 ohm
   tension   0 V   <------> 5 V
   pin value 0     <------> 1024
   
   if the mechanical pot's range is too short, increase gearMotorPotRatio (normally =1)*/
  sensorR = constrain(gearMotorRorLPotRatio*(analogRead(potR)-512)+512,0,1024);  // range 0-1024
  sensorL = constrain(gearMotorRorLPotRatio*(analogRead(potL)-512)+512,0,1024);  // range 0-1024

  motorMotion(motRight,sensorR,DataValueR, potMiniRorL,potMaxiRorL);
  motorMotion(motLeft,sensorL,DataValueL,potMiniRorL,potMaxiRorL);
}
////////////////////////////////////////////////////////////////////////////////
// Procedure: wait for complete trame-------------------------------------------------------------bralnik   
////////////////////////////////////////////////////////////////////////////////
void readSerialData()
{
  byte Data[2]={
    '0','0'};
  // keep this function short, because the loop has to be short to keep the control over the motors

  // if (Serial.available()>=(2*SignalLength-1)) {
  //we are sure to have a whole signal to decode inside the buffer
  // and no more than one, in order to have the newest (latest) as possible


  if (Serial.available()>2){ //wait a packet of 6
    //parse the buffer : test if the byte is the first of the order R
    Data[0]=Serial.read();
    if (Data[0]=='R'){
      Data[1]=Serial.read();
      Data[2]=Serial.read();
      //  call the function that converts the hexa in decimal and maps the range
      // map the range from Xsim (0-255) to the mechanically authorized range (potMini-potMaxi)
      DataValueR=map(NormalizeData(Data),0,255,potMiniRorL,potMaxiRorL);
    }
    if (Data[0]=='L'){
      Data[1]=Serial.read();
      Data[2]=Serial.read();
      //  call the function that converts the hexa in decimal and maps the range
      DataValueL=map(NormalizeData(Data),0,255,potMiniRorL,potMaxiRorL);
    }
  }
  if (Serial.available()>16) Serial.flush();
  // no need to flush because reading the bytes has reduced the serial buffer and it will be filled in by the next incoming data
}
////////////////////////////////////////////////////////
void motorMotion(int numMot,int actualPos,int targetPos,int potMini, int potMaxi )  /// pwm določitev pa smer?
////////////////////////////////////////////////////////
{
  int Tol=20;
  //could be a parameter read from a pot on an analogic pin
  // the highest, the calmest the simulator would be (less moves)
  // % (PotMaxi-potMini)
  int gap;
  int pwm;
  int brakingDistance=30;

  // security concern : targetPos has to be within the mechanically authorized range
  targetPos=constrain(targetPos,potMini+brakingDistance,potMaxi-brakingDistance);

  gap=abs(targetPos-actualPos);

  if (gap<= Tol) {
    motorOff(numMot); //too near to move     
  }
  else {
    // PID : calculate speed proportionaly to distance to go
    pwm=195;
    if (gap>50)   pwm=215;
    if (gap>75)   pwm=235;   
    if (gap>100)  pwm=255;

    pwm=map(pwm, 0, 255, 0, pwmMax);  //adjust the value according to pwmMax for mechanical debugging purpose !

    // pwm=155;  // debug

    // if motor is outside from the range, send motor back to the limit !
    // go forward (up)
    if ((actualPos<potMini) || (actualPos<targetPos)) motorGo(numMot, FW, pwm);
    // go reverse (down)   
    if ((actualPos>potMaxi) || (actualPos>targetPos)) motorGo(numMot, RV, pwm);

  }
}

////////////////////////////////////////////////////////////////////////////////
void motorOff(int motor){ //Brake //Ground : free wheel actually
  ////////////////////////////////////////////////////////////////////////////////
  analogWrite(PWMpin[motor], 0);
}

////////////////////////////////////////////////////////////////////////////////
void motorGo(int motor, int directionOfRotation, int pwm)
////////////////////////////////////////////////////////////////////////////////
{
  digitalWrite(DIRpin[motor], directionOfRotation);
  analogWrite(PWMpin[motor], pwm);
}


////////////////////////////////////////////////////////////////////////////////
// Function: convert Hex to Dec
////////////////////////////////////////////////////////////////////////////////
int NormalizeData(byte x[3])
////////////////////////////////////////////////////////////////////////////////
{
  int result;

  if ((x[2]==13) || (x[2]=='R') || (x[2]=='L'))  //only a LSB and Carrier Return or 'L' or 'R' in case of value below 16 (ie one CHAR and not 2)
  {
    x[2]=x[1];  //move MSB to LSB
    x[1]='0';     //clear MSB
  }
  for (int i=1; i<3; i++)
  {
    if (x[i]>47 && x[i]<58 ){//for xA to xF
      x[i]=x[i]-48;
    }                       
    if (  x[i]>64 && x[i]<71 ){//for x0 to x9
      x[i]=x[i]-55;               
    }
  }
  result=(x[1]*16+x[2]);
  return result;
}
////////////////////////////////////////////////////////////////////////////////
// Function: convert Hex to Dec
////////////////////////////////////////////////////////////////////////////////

int HexaToDecimal(byte x[2])
{
  byte ret = 0, t = 0;
  for (int i=0; i<2; i++)
  {
    if((x[i] >= '0') && (x[i] <= '9')) t = (x[i] - '0');
    else if((x[i] >= 'A') && (x[i] <= 'F')) t = (x[i] - 'A' + 10);
    else if((x[i] >= 'a') && (x[i] <= 'f')) t = (x[i] - 'a' + 10);
    else break;
    ret *= 16;
    ret += t;
  }
  return ret;
}
