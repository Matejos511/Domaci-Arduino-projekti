/***********************************************************
File name: 18_DHT11.ino
Description: you can see the temperature and humidity data     displayed on the LCD1602.
Website: www.adeept.com

***********************************************************/
const unsigned long desets = 10000;
unsigned long prejcajt = 0;

#define Led_pin 13 //pin za rele
#define Led_pin1 8 //pin za rele
#define gumb1 3//pin +
#define gumb2 4//pin -
#define model 5//pin za preklop
#define data 
#include <dht11.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//SPREMENLJIVKE
int temperatura=25;
int vlaga=65;
int modeS;
int p;
int m;
int modeSW=0;

//TABELE            s min h
//        gumbSW    [3,4,5]ura
int v[10]={0 ,0 ,0 ,50 ,0 , 0, 0}; //tabela z več spremenljivkami

dht11 DHT11;
#define DHT11PIN 2

//LiquidCrystal_I2C lcd(0x3F,20,4); // set the LCD address to 0x27 for a 16 chars and 2 line display 
LiquidCrystal_I2C lcd(0x3F,2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

void setup()

{
  pinMode(gumb1,INPUT); 
  pinMode(gumb2,INPUT);
  pinMode(model,INPUT); 

Serial.begin(9600);
lcd.begin(20,4);
lcd.backlight(); //open the backlight 
pinMode(Led_pin , OUTPUT);// določitev pina za rele   
pinMode(Led_pin1 , OUTPUT);// določitev pina za rele 
lcd.setCursor ( 0, 2 );            // pojdi v 4. vrstico
lcd.print("Set temp:");
lcd.print(temperatura);
lcd.setCursor ( 0, 3 ); 
lcd.print("Set vlaga:"); 
lcd.print(vlaga);

digitalWrite(Led_pin, HIGH);
digitalWrite(Led_pin1, HIGH);
v[2] = digitalRead(model);
  
}
 
void loop(){ 
  
  if (modeSW != 5){
 digitalWrite(Led_pin, HIGH);         //// VARNOST
 digitalWrite(Led_pin1, HIGH);
}

    int chk = DHT11.read(DHT11PIN); /bere senzorje
    lcd.setCursor(0, 0); // set the cursor to column 0, line 0
    lcd.print("Vlaga:");// Print a message of "Humidity: "to the LCD.
    lcd.print((float)DHT11.humidity, 2);// Print a message of "Humidity: "to the LCD.
    lcd.print(" % "); // Print the unit of the centigrade temperature to the LCD.
    
    lcd.setCursor(0, 1); // set the cursor to column 0, line 0
    lcd.print("Temp:");// Print a message of "Temp: "to the LCD.
    lcd.print((float)DHT11.temperature, 2);// Print a centigrade temperature to the LCD. 
    lcd.print(" C "); // Print the unit of the centigrade temperature to the LCD.

    m = digitalRead(gumb1);
    p = digitalRead(gumb2);
    
    //MENI NASTAVLJANJA
    v[1] = v[0];
    v[0] = digitalRead(model);
    if(v[1] == HIGH &&  v[0] == LOW) {
    modeSW++;
    lcd.setCursor (19, 3); 
    lcd.print(modeSW);
    }
     if(modeSW>5){  //omejitev menija
     modeSW=0;}

      //notramja ura*********************
unsigned long cajt = millis();
  if(cajt - prejcajt >= desets){
      v[3] +=10;
      prejcajt = cajt;
      Serial.print("Preteklo je: ");
      Serial.print(v[5]);
      Serial.print("h ");
      Serial.print(v[4]);
      Serial.print("min ");
      Serial.print(v[3]);
      Serial.println("s");
}
if (v[3]==60){  //V 3s, v4 min v5 h
    v[3]=0;
  v[4]++;
  lcd.setCursor(15, 0); 
    lcd.print(v[5]);
    lcd.setCursor(17, 0); 
    lcd.print(":");
    lcd.setCursor(18, 0); 
    lcd.print(v[4]);
  if (v[4]==60){
    v[4]=0;
  v[5]++;
  }
}

//MOD 1(vlaga) , 2(temperatura)--> GUMBI
     if (p == HIGH && modeSW == 1){
      vlaga++;
      lcd.setCursor ( 0, 3 ); 
lcd.print("Set vlaga:"); 
lcd.print(vlaga);
}
     if (m == HIGH && modeSW == 1){
      vlaga--;
lcd.setCursor ( 0, 3 ); 
lcd.print("Set vlaga:"); 
lcd.print(vlaga);
 }
 //-----temp------    2          ----temp
if (p == HIGH && modeSW == 2){
      temperatura++;
      lcd.setCursor ( 0, 2 );            
lcd.print("Set temp:");
lcd.print(temperatura); 
}
    if (m == HIGH && modeSW == 2){
      temperatura--;
      lcd.setCursor ( 0, 2 );           
lcd.print("Set temp:");
lcd.print(temperatura); 
  } 
  
// Programski del sušenja sadja -----------------------------------------------------------------------PROGRAM

if (modeSW == 5){       //OBRATOVANJE LE NA MODU 5
   if (DHT11.humidity > vlaga){ 
  digitalWrite(Led_pin, HIGH);}
   if (DHT11.humidity < vlaga){ 
  digitalWrite(Led_pin, LOW);}

  if (DHT11.temperature > temperatura){
  digitalWrite(Led_pin1, HIGH);}
   if (DHT11.temperature < temperatura){
  digitalWrite(Led_pin1, LOW);}
}
      //1. program 50-80° 24-36h
      
}
   
