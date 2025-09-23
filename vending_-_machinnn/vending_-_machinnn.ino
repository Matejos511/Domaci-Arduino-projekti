//branje
int ber0;
int ber1;
int ber2;
int ber3;
int ber4;
//eurodva
int dva = 2;
int dvain;
//gumb za izklop
int pladenj = 3;
int pladenjread;
int tonePin = 1;

// IZDELKI
int cena1 = 2;
int cena2 = 1;
int cena3 = 3;

int vrata1 = 9;
int vrata2 = 8;
int vrata3 = 7;
int vrata1b;
int vrata2b;
int vrata3b;
int vrzi1 = 6;
int vrzi2 = 5;
int vrzi3 = 4;

float znesek=0;

//displej
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//I2C pins declaration
LiquidCrystal_I2C lcd(0x3F,2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
//LiquidCrystal_I2C lcd(0x3F,20,4);
 
void setup() {
  //da se ne vrti 
  digitalWrite(vrzi1,HIGH);
  digitalWrite(vrzi2, HIGH);
  digitalWrite(vrzi3, HIGH);
  Serial.begin(9600);
  pinMode(dva,INPUT);
  pinMode(pladenj,INPUT);
  pinMode(tonePin,OUTPUT);
  // določitev inputov/ autputov za stvari
  pinMode(vrata1,INPUT);
  pinMode(vrata2,INPUT);
  pinMode(vrata3,INPUT);
  pinMode(vrzi1,OUTPUT);
  pinMode(vrzi2,OUTPUT);
  pinMode(vrzi3,OUTPUT);
  
  lcd.begin(20,4);//Defining 20 columns and 4 rows of lcd display
  lcd.backlight();//To Power ON the back light
  //lcd.backlight();// To Power OFF the back light

  lcd.setCursor(0,0);  
  lcd.print("VSTAVI KOVANCE");
  lcd.setCursor(0,1);  
  lcd.print("ZNESEK:");
  lcd.setCursor(0,2);  
  lcd.print("IZBIRA:");
}
  
void loop() {
     odpira();
     bere();
}
void bere(){
ber0=analogRead(A0);
ber1=analogRead(A1);
ber2=analogRead(A2);
ber3=analogRead(A3);
ber4=analogRead(A4);
dvain = digitalRead(dva);
if (ber0 > 5){
   znesek = znesek + 0.1;
    lcd.setCursor(8,1);  
    lcd.print(znesek); 
    delay(500);
    }
if (ber1 > 5){
   znesek = znesek + 0.05;
    lcd.setCursor(8,1);  
    lcd.print(znesek); 
    delay(500);
    }
if (ber2 > 5){
   znesek = znesek + 0.2;
    lcd.setCursor(8,1);  
    lcd.print(znesek); 
    delay(500);
    }
if (ber3 > 5){
  znesek = znesek + 1;
   lcd.setCursor(8,1);  
   lcd.print(znesek); 
   delay(500);
   }
if (ber4 > 5){
  znesek = znesek + 0.5;
   lcd.setCursor(8,1);  
   lcd.print(znesek); 
   delay(500);
   }
if (dvain == LOW){
  znesek = znesek + 2;
   lcd.setCursor(8,1);  
   lcd.print(znesek); 
   delay(500);
  }
}
void odpira(){
  vrata1b = digitalRead(vrata1);
  vrata2b = digitalRead(vrata2);
  vrata3b = digitalRead(vrata3);
  pladenjread = digitalRead(pladenj);
  
  if (vrata1b == HIGH && znesek >= cena1){
    digitalWrite(vrzi1, LOW);
     znesek = znesek - cena1;
    lcd.setCursor(8,2);  
    lcd.print("1");
    lcd.setCursor(8,1); 
    lcd.print(znesek);
    delay(1000);
    lcd.setCursor(8,2);  
    lcd.print("0");
  }
   if (vrata2b == HIGH && znesek >= cena2){
    digitalWrite(vrzi2, LOW);
    znesek = znesek - cena2;
    lcd.setCursor(8,2);  
    lcd.print("2");
    lcd.setCursor(8,1); 
    lcd.print(znesek);
    delay(1000);
    lcd.setCursor(8,2);  
    lcd.print("0");
  }
   if (vrata3b == HIGH && znesek >= cena3){
    digitalWrite(vrzi3, LOW);
    znesek = znesek - cena3;
    lcd.setCursor(8,2);  
    lcd.print("3");
    lcd.setCursor(8,1); 
    lcd.print(znesek);
    delay(1000);
    lcd.setCursor(8,2);  
    lcd.print("0");
  }
if(pladenjread == HIGH){
  digitalWrite(vrzi1,HIGH);
  digitalWrite(vrzi2, HIGH);
  digitalWrite(vrzi3, HIGH);
  tone(tonePin, 329, 225);
  }
}
