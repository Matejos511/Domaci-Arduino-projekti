#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define echoPin 6 // Pin za sprejem echo impulza
#define echoPin1 8 // Pin za sprejem echo impulza
#define trigPin 7 // Pin za oddajo trigger impulza
#define trigPin1 10 // Pin za oddajo trigger impulza
#define Led_pin 13 //pin za rele


void setup() {
  
  lcd.begin(16,2); // Pozicija (kolumna in vrstica) prikazovanja naslednjega teksta.
  lcd.print("Kolicina vode:"); // izpis teksta
  Serial. begin(9600); // nastavitev komunikacije s SRF04
  pinMode(echoPin , INPUT); // določitev pina za odčitek SRF04
  pinMode(echoPin1 , INPUT); // določitev pina za odčitek1 SRF04
  pinMode(trigPin , OUTPUT); // določitev pina za proženje SRF04
  pinMode(trigPin1 , OUTPUT); // določitev pina za proženje SRF04
  pinMode(Led_pin , OUTPUT);// določitev pina za rele
  

}

void loop() {


  digitalWrite(trigPin, LOW); // postavi Trigger pin na Low za 2 mikrosekundi
 
  digitalWrite(trigPin, HIGH); // postavi Trigger pin na HIGH za 10 mikrosekundi
  
  delayMicroseconds(10);
  
  digitalWrite(trigPin, LOW); // postavi Trigger pin na LOW za začetek merjenja
  int distance = pulseIn(echoPin, HIGH); // odčitaj
  int distance1 = (distance / 58); // Izračunaj odčitek v cm
  int distance8 = (2000-distance1 *20);
  delay(51);
  lcd.setCursor(0,1); // pozicioniraj naslednji izpis
  lcd.print(distance8);
  lcd.print("L..."); // dodaj še napis cm
  delay(50); // zakasnitev do naslednjega merjenja
   

 

  { digitalWrite(trigPin1, LOW); // postavi Trigger pin na Low za 2 mikrosekundi
  digitalWrite(trigPin1, HIGH); // postavi Trigger pin na HIGH za 10 mikrosekundi
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW); // postavi Trigger pin na LOW za začetek merjenja
  int distance2 = pulseIn(echoPin1, HIGH); // odčitaj
  int distance3 = (distance2 / 58); // Izračunaj odčitek v cm
  int distance7 = (4620 - distance3 *34);
  delay(51);

  lcd.print(distance7); // izpiši razdaljo
  lcd.print("L..."); // dodaj še napis cm }
delay(50); // zakasnitev do naslednjega merjenja}

  if (distance1>=40){ 
 digitalWrite(Led_pin, LOW);}

  if (distance1<=13){
 digitalWrite(Led_pin, HIGH);}
 if (distance3<=5&&(Led_pin,HIGH)) 
 digitalWrite(Led_pin, LOW);
}
}



 
  

   
