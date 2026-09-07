const int relayPin = 1;        // Pin za rele (active LOW)
const int pcSensePin = 7;     // Pin, ki zazna 5V iz PC USB (preko delilnika!)
bool pcWasOn = false;
const unsigned long pressTime = 500; // trajanje klika v ms

void setup() {
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // rele izklopljen (ker je active-low)

  pinMode(pcSensePin, INPUT);
}

void loop() {
  bool pcIsOn = digitalRead(pcSensePin);

  if (!pcWasOn && pcIsOn) {
    pressButton();   // vklop radia
    pcWasOn = true;
  }

  if (pcWasOn && !pcIsOn) {
    pressButton();   // izklop radia
    pcWasOn = false;
  }

  delay(200);
}

void pressButton() {
  digitalWrite(relayPin, LOW);   // aktiviraj rele (klik)
  delay(pressTime);
  digitalWrite(relayPin, HIGH);  // spusti rele
}
