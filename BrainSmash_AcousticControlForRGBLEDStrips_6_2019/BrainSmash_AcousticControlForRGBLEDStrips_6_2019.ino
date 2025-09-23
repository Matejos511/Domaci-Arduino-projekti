/**********************************
* ******** BRAIN SMASH ******** 
* ****** AUTHOR: DAEDALUS   7/2019 ******
* **** Visit us at: Youtube.com/BrainSmash ****
* ** Github.com/BrainSmash **
* You may use or modify this code for any and all of your projects whitout any restrictions
* If you use any fragment of this code we will be happy if you could drop us a message ;)
* Brain Smash nor the Author are hold responsible for any misuse partially or complete of this code 
*/
#define Rpin 11
#define Gpin 10
#define Bpin 9
#define delayLEDS 3
#define sensorPin A1

float sensorValue = 0, filteredSignal = 0,
    filteredSignalValues[] = {5.0, 3.7, 3.5, 2.2, 1.8, 1.5, 1.0, 0.5, 0.18, 0.15};   //tle nastavš nivoje

void setup () {

  //Serial.begin (9600); //izklop pisanja da boljš dela
}

void loop () {

  MainFunction();
}

void MainFunction() {

  sensorValue = (float) analogRead(sensorPin) * (5.0 / 1024.0);

  FilterSignal(sensorValue);
  
  //Serial.print(sensorValue);
  //Serial.print(" ");
 // Serial.println(filteredSignal);

  CompareSignalFiltered(filteredSignal);
}

void FilterSignal(float sensorSignal) {

  filteredSignal = (0.945 * filteredSignal) + (0.0549 * sensorSignal);
}

void CompareSignalFiltered(float filteredSignal) {

  if (filteredSignal > filteredSignalValues[0]) {
    RGBColor(0, 0, 255);
   // Serial.println("Blue");
  } else if (filteredSignal <= filteredSignalValues[0] && filteredSignal > filteredSignalValues[1]) {
 //   Serial.println("Azure"); 
    RGBColor(0, 255, 255);
  } else if (filteredSignal <= filteredSignalValues[1] && filteredSignal > filteredSignalValues[2]) {
    RGBColor(0, 127, 255);
 //   Serial.println("Cyan");
  } else if (filteredSignal <= filteredSignalValues[2] && filteredSignal > filteredSignalValues[3]) {
    RGBColor(0, 255, 127);
  //  Serial.println("Aqua marine");
  } else if (filteredSignal <= filteredSignalValues[3] && filteredSignal > filteredSignalValues[4]) {
    RGBColor(0, 255, 0);
  //  Serial.println("Green");
  } else if (filteredSignal <= filteredSignalValues[4] && filteredSignal > filteredSignalValues[5]) {
    RGBColor(255, 255, 0);
   // Serial.println("Yellow");
  } else if (filteredSignal <= filteredSignalValues[5] && filteredSignal > filteredSignalValues[6]) {
    RGBColor(255, 0, 255);
  //  Serial.println("Magenta");
  } else if (filteredSignal <= filteredSignalValues[6] && filteredSignal > filteredSignalValues[7]) {
    RGBColor(255, 0, 127);
  //  Serial.println("Rose");
  } else if (filteredSignal <= filteredSignalValues[7] && filteredSignal > filteredSignalValues[8]) {
    RGBColor(255, 127, 0);
   // Serial.println("Orange");
  } else if (filteredSignal <= filteredSignalValues[8] && filteredSignal > filteredSignalValues[9]) {
    RGBColor(255, 0, 0);
   // Serial.println("Red");
  } else {
    RGBColor(0, 0, 255);
   // Serial.println("Default: Blue");
  }
}

void RGBColor(int Rcolor, int Gcolor, int Bcolor) {

  analogWrite(Rpin, Rcolor);
  analogWrite(Gpin, Gcolor);
  analogWrite(Bpin, Bcolor);

  delay(delayLEDS);
} 
