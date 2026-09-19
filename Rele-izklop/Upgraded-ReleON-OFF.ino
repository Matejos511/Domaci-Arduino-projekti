/*
  ESP8266 - PC Sense -> Radio Button

  GPIO5 = rele
  GPIO4 = PC sense

  Rele je ACTIVE LOW:
    LOW  = rele vklopljen
    HIGH = rele izklopljen

  Serial Monitor:
    ?  = pomoč
    S  = stanje PC sense
    R1 = ročni vklop releja
    R0 = ročni izklop releja
*/

const int relayPin   = 5;   // GPIO5
const int pcSensePin = 4;   // GPIO4

const unsigned long pressTime = 500;
const unsigned long debounceMs = 1000;

bool pcWasOn = false;
bool lastRawState = false;
bool stableState = false;

unsigned long stateChangedAt = 0;


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  delay(500);

  // Rele
  pinMode(relayPin, OUTPUT);

  // Rele mora biti ob zagonu IZKLOPLJEN
  digitalWrite(relayPin, HIGH);

  // PC sense
  pinMode(pcSensePin, INPUT);

  delay(500);

  // Preberi začetno stanje
  bool initialState = digitalRead(pcSensePin);

  lastRawState = initialState;
  stableState = initialState;
  pcWasOn = initialState;

  Serial.println();
  Serial.println("================================");
  Serial.println("ESP8266 PC -> RADIO CONTROLLER");
  Serial.println("================================");

  if (pcWasOn) {
    Serial.println("PC JE VKLOPLJEN");
  } else {
    Serial.println("PC JE IZKLOPLJEN");
  }

  Serial.println();
  Serial.println("Vtipkaj ? za pomoc.");
}


// =====================================================
// LOOP
// =====================================================

void loop() {

  handleSerial();
  handlePcSense();

  delay(10);
}


// =====================================================
// SERIAL
// =====================================================

void handleSerial() {

  if (!Serial.available()) {
    return;
  }

  String cmd = Serial.readStringUntil('\n');

  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "?") {

    printHelp();

  }
  else if (cmd == "S") {

    bool state = digitalRead(pcSensePin);

    Serial.print("Sense GPIO4 = ");
    Serial.print(state ? "HIGH" : "LOW");

    if (state) {
      Serial.println(" -> PC VKLOPLJEN");
    } else {
      Serial.println(" -> PC IZKLOPLJEN");
    }
  }
  else if (cmd == "R1") {

    Serial.println("ROCNI TEST: RELE ON");

    digitalWrite(relayPin, LOW);

  }
  else if (cmd == "R0") {

    Serial.println("ROCNI TEST: RELE OFF");

    digitalWrite(relayPin, HIGH);

  }
  else if (cmd == "P") {

    Serial.println("ROCNI TEST: KLIK");

    pressButton();

  }
  else if (cmd.length() > 0) {

    Serial.print("Neznan ukaz: ");
    Serial.println(cmd);

    Serial.println("Uporabi ?, S, R1, R0 ali P");
  }
}


// =====================================================
// HELP
// =====================================================

void printHelp() {

  Serial.println();
  Serial.println("--------------------------------");
  Serial.println("Ukazi:");
  Serial.println("  ?  - pomoc");
  Serial.println("  S  - stanje PC sense");
  Serial.println("  R1 - rele ON");
  Serial.println("  R0 - rele OFF");
  Serial.println("  P  - en klik releja");
  Serial.println("--------------------------------");
  Serial.println();
}


// =====================================================
// PC SENSE
// =====================================================

void handlePcSense() {

  bool rawState = digitalRead(pcSensePin);

  // Če se je vhod spremenil,
  // začni meriti čas stabilizacije
  if (rawState != lastRawState) {

    lastRawState = rawState;
    stateChangedAt = millis();

    return;
  }


  // Sprememba mora biti stabilna vsaj debounceMs
  if (rawState != stableState) {

    if (millis() - stateChangedAt >= debounceMs) {

      stableState = rawState;

      // ==========================================
      // PC ON
      // ==========================================

      if (stableState && !pcWasOn) {

        Serial.println();
        Serial.println("[AUTO] PC ON");
        Serial.println("[AUTO] Vklapljam radio...");

        pressButton();

        pcWasOn = true;
      }


      // ==========================================
      // PC OFF
      // ==========================================

      else if (!stableState && pcWasOn) {

        Serial.println();
        Serial.println("[AUTO] PC OFF");
        Serial.println("[AUTO] Izklapljam radio...");

        pressButton();

        pcWasOn = false;
      }
    }
  }
}


// =====================================================
// KLIK RELEJA
// =====================================================

void pressButton() {

  Serial.println("RELE ON");

  digitalWrite(relayPin, LOW);

  delay(pressTime);

  digitalWrite(relayPin, HIGH);

  Serial.println("RELE OFF");
}
