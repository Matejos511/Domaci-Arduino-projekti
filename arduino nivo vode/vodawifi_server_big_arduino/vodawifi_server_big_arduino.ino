//OLD 

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);  //vrži vn staro lcd povezavzrt4o

/*    //NEW
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  //PRIVEZ NA SCL/SDA
*/
int distance;
int distance1;
int distance2;
int distance3;
int distance4;

//ura
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 2000;  //the value is a number of milliseconds

#define echoPin 6 // Pin za sprejem echo impulza
#define echoPin1 8 // Pin za sprejem echo impulza
#define trigPin 7 // Pin za oddajo trigger impulza
#define trigPin1 10 // Pin za oddajo trigger impulza
#define Relejama1 13 //pin za rele


void setup() {
  
  lcd.begin(20,4); // Pozicija (kolumna in vrstica) prikazovanja naslednjega teksta.
  lcd.print("Kolicina vode:"); // izpis teksta
  
  Serial. begin(9600); // nastavitev komunikacije
  
  pinMode(echoPin , INPUT); // določitev pina za odčitek SRF1      zj nwm al cisterne al jama
  pinMode(echoPin1 , INPUT); // določitev pina za odčitek1 SRF2
  pinMode(trigPin , OUTPUT); // določitev pina za proženje SRF1
  pinMode(trigPin1 , OUTPUT); // določitev pina za proženje SRF2
  pinMode(Relejama1 , OUTPUT);// določitev pina za rele
  
    startMillis = millis();
}
void loop() {
  currentMillis = millis();
  if (currentMillis - startMillis >= period){ //PREVERI VSAKO SEKUNDO
  digitalWrite(trigPin, LOW); // postavi Trigger pin na Low za 2 mikrosekundi
  digitalWrite(trigPin, HIGH); // postavi Trigger pin na HIGH za 10 mikrosekundi
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); // postavi Trigger pin na LOW za začetek merjenja
  int distance = pulseIn(echoPin, HIGH); // odčitaj
  int distance1 = (distance / 58); // Izračunaj odčitek v cm
  int distance2 = (2000-distance1 *20);
  
  digitalWrite(trigPin1, LOW); // postavi Trigger pin na Low za 2 mikrosekundi
  digitalWrite(trigPin1, HIGH); // postavi Trigger pin na HIGH za 10 mikrosekundi
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW); // postavi Trigger pin na LOW za začetek merjenja
  int distance3 = pulseIn(echoPin1, HIGH); // odčitaj
  int distance4 = (distance3 / 58); // Izračunaj odčitek v cm
  int distance5 = (4620 - distance4 *34);
  
  lcd.setCursor(0,1); // pozicioniraj naslednji izpis
  lcd.print(distance2);
  lcd.print("L..."); // dodaj še napis L
  lcd.print(distance5); // izpiši razdaljo
  lcd.print("L..."); // dodaj še napis L
  lcd.setCursor(0,2);
  lcd.print(distance1);
  lcd.print("cm...");
  lcd.print(distance4);
  lcd.print("cm...");
        startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
        
  //serijska raw data  kolk cm v jami?
  if (Serial.available() > 0) { // reply only when you receive data:
  Serial.print(distance);
  Serial.print(" D1: ");
  Serial.print(distance1);
  Serial.println(" cm");
  Serial.print(distance3);
  Serial.print(" D2: ");
  Serial.print(distance4);
  Serial.println(" cm");
  }
 }

  
  if (distance1 >= 40){ 
 digitalWrite(Relejama1, LOW);}

  if (distance1 <= 13){
 digitalWrite(Relejama1, HIGH);}
 
 if (distance3 <=5 &&(Relejama1,HIGH)){ 
 digitalWrite(Relejama1, LOW);}

}
