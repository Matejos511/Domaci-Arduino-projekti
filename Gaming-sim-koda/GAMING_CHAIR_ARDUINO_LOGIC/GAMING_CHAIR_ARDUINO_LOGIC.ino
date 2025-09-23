//********************************************************************************************
// RC Model Servo
// Original code By EAOROBBIE (Robert Lindsay)
// Completely mangled by aarondc
// For free use for Sim Tool Motion Software
//********************************************************************************************
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

#include <Servo.h>                                  
const int kActuatorCount = 2;                       // how many Actuators we are handling

// the letters ("names") sent from Sim Tools to identify each actuator
// NB: the order of the letters here determines the order of the remaining constants kPins and kActuatorScale
const char kActuatorName[kActuatorCount] = { 'R', 'L',};
const int kPins[kActuatorCount] = {4, 5};                       // pins to which the Actuators are attached
const int kActuatorScale[kActuatorCount][4] = { { 0, 179 } ,    // Right Actuator scaling
                                                { 179, 0 } ,    // Left side Actuator scaling
                                               };      
const char kEOL = '~';                              // End of Line - the delimiter for our acutator values 
const int kMaxCharCount = 3;                        // some insurance...
Servo actuatorSet[kActuatorCount];                  // our array of Actuators
int actuatorPosition[kActuatorCount] = {90, 90};    // current Actuator positions, initialised to 90
int currentActuator;                                // keep track of the current Actuator being read in from serial port
int valueCharCount = 0;                             // how many value characters have we read (must be less than kMaxCharCount!!

// set up some states for our state machine
// psReadActuator = next character from serial port tells us the Actuator
// psReadValue = next 3 characters from serial port tells us the value
enum TPortState { psReadActuator, psReadValue };   
TPortState currentState = psReadActuator;

void setup()
{ 
    // attach the Actuators to the pins
    for (int i = 0; i < kActuatorCount; i++) 
        actuatorSet[i].attach(kPins[i]);
    
    // initialise actuator position
    for (int i = 0; i < kActuatorCount; i++) 
        updateActuator(i);
    lcd.begin(20,4);
    Serial.begin(9600); // opens serial port at a baud rate of 9600
} 
 
void loop()
{ 
 
   
  lcd.setCursor ( 0, 0);  
  lcd.print(actuatorPosition[currentActuator]);
  
}

// this code only runs when we have serial data available. ie (Serial.available() > 0). ---------           TLE ZAČNE BRT IZ PCJA
void serialEvent() {
    char tmpChar;    ///  ker  motor    po črkah
    int tmpValue;

    while (Serial.available()) {
        // if we're waiting for a Actuator name, grab it here
        if (currentState == psReadActuator) {
            tmpChar = Serial.read();
            // look for our actuator in the array of actuator names we set up     ------------------------- DOLOČ AKTUATOR

            for (int i = 0; i < kActuatorCount; i++) {
              lcd.setCursor ( 5, 4 );
              lcd.print(tmpChar);   -------------------------------------------------------------  KER MOTOR - ČRKA   
                if (tmpChar == kActuatorName[i]) {

                    currentActuator = i;                        // remember which actuator we found
                    currentState = psReadValue;                 // start looking for the Actuator position 
                    actuatorPosition[currentActuator] = 0;      // initialise the new position
                    valueCharCount = 0;                         // initialise number of value chars read in 
                    break;
                }
            }
        }
        
        // if we're ready to read in the current Actuator's position data    PREBERI POZICIJO AKTUATORJA -------------
        if (currentState == psReadValue) {
            while ((valueCharCount < kMaxCharCount) && Serial.available()) {
                tmpValue = Serial.read();
                 lcd.setCursor ( 0, 1);  
                 lcd.print(tmpValue);-------------------------------------------------------------------------------- POZICIJA MOTORJA
                if (tmpValue != kEOL) {
                    tmpValue = tmpValue - 48;
                    if ((tmpValue < 0) || (tmpValue > 9)) tmpValue = 0;
                    actuatorPosition[currentActuator] = actuatorPosition[currentActuator] * 10 + tmpValue;
                    valueCharCount++;
                }
                else break;
            }
            
            // if we've re
ator and start looking for the next Actuator name
            if (tmpValue == kEOL || valueCharCount == kMaxCharCount) {

                // scale the new position so the value is between 0 and 179
                actuatorPosition[currentActuator] = map(actuatorPosition[currentActuator], 0, 255, kActuatorScale[currentActuator][0], kActuatorScale[currentActuator][1]);

                updateActuator(currentActuator);
                currentState = psReadActuator;
            }
        }
    }
}


// write the current Actuator position to the passed in Actuator 
void updateActuator(int thisActuator) {
    actuatorSet[thisActuator].write(actuatorPosition[thisActuator]);
}
