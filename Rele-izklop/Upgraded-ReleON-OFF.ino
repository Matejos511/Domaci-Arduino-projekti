/*
  PC Power Sense → Radio Button Press (ESP8266 / Arduino)
  -------------------------------------------------------
  - pcSensePin: zazna 5V iz PC (preko delilnika!) → HIGH = PC vklopljen
  - relayPin: active-LOW rele (simulira pritisk gumba na radiu)

  Konzolni ukazi (Serial 115200):
    ?     → prikaže pomoč
    S     → stanje sense pina (0/1) + interpretacija
    R0    → vklopi rele (pritisk gumba – aktivno LOW)
    R1    → izklopi rele (spusti gumb)
*/

const int relayPin   = 1;          // Active-LOW rele
const int pcSensePin = 7;          // Sense pin (preko delilnika)

bool pcWasOn = false;
const unsigned long pressTime = 500;   // dolžina "klika" v ms
const unsigned long debounceMs = 1000;  // anti-bounce za sense

unsigned long lastSenseChange = 0;
bool lastStableState = false;

void setup() {
  Serial.begin(115200);
  delay(100);                       // počakaj, da se Serial stabilizira

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);     // rele izklopljen (active-LOW)

  pinMode(pcSensePin, INPUT);       // digitalni vhod (po delilniku)

  Serial.println(F("\n=== PC Sense → Radio Controller ==="));
  Serial.println(F("Vtipkaj ? za seznam ukazov"));
  printHelp();
}

void loop() {
  handleSerial();                   // najprej obravnavaj ukaze iz konzole
  handlePcSense();                  // potem avtomatsko zaznavanje PC-ja
  delay(20);                        // malo dihanja (ni potrebno 200 ms)
}

// ------------------------------------------------------------
//  Serijska konzola
// ------------------------------------------------------------
void handleSerial() {
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "?") {
    printHelp();
  }
  else if (cmd == "S") {
    bool state = digitalRead(pcSensePin);
    Serial.print(F("Sense pin: "));
    Serial.print(state);
    Serial.println(state ? F("  → PC VKLOPLJEN") : F("  → PC IZKLOPLJEN"));
  }
  else if (cmd == "R1") {
    digitalWrite(relayPin, LOW);    // aktiviraj rele (pritisk)
    Serial.println(F("Rele ON (R0) – pritisk gumba"));
  }
  else if (cmd == "R0") {
    digitalWrite(relayPin, HIGH);   // spusti rele
    Serial.println(F("Rele OFF (R1) – gumb spuščen"));
  }
  else if (cmd.length() > 0) {
    Serial.print(F("Neznan ukaz: "));
    Serial.println(cmd);
    Serial.println(F("Vtipkaj ? za pomoč"));
  }
}

void printHelp() {
  Serial.println(F("----------------------------------------"));
  Serial.println(F("Ukazi:"));
  Serial.println(F("  ?     - ta pomoč"));
  Serial.println(F("  S     - stanje sense pina (0/1)"));
  Serial.println(F("  R1    - vklopi rele (pritisk gumba)"));
  Serial.println(F("  R0    - izklopi rele (spusti gumb)"));
  Serial.println(F("----------------------------------------"));
}

// ------------------------------------------------------------
//  Avtomatsko zaznavanje vklopa/izklopa PC-ja
// ------------------------------------------------------------
void handlePcSense() {
  bool rawState = digitalRead(pcSensePin);

  // Debounce – sprememba mora trajati dovolj dolgo
  if (rawState != lastStableState) {
    if (millis() - lastSenseChange > debounceMs) {
      lastStableState = rawState;
      lastSenseChange = millis();

      if (rawState && !pcWasOn) {
        // PC se je pravkar vklopil
        Serial.println(F("[AUTO] PC ON → pritisk gumba (vklop radia)"));
        pressButton();
        pcWasOn = true;
      }
      else if (!rawState && pcWasOn) {
        // PC se je pravkar izklopil
        Serial.println(F("[AUTO] PC OFF → pritisk gumba (izklop radia)"));
        pressButton();
        pcWasOn = false;
      }
    }
  } else {
    lastSenseChange = millis();   // resetiraj timer, če je stanje stabilno
  }
}

void pressButton() {
  digitalWrite(relayPin, LOW);    // aktiviraj rele
  delay(pressTime);
  digitalWrite(relayPin, HIGH);   // spusti rele
}
