#include "BitBangSPI.h"
#include "MFRC522_BitBang.h"
#include "config.h"
#include "PiezoSensors.h"
#include "GlobalState.h"
#include "UidMap.h"

#define RST_PIN 48

unsigned long lastPrintTime = 0;

void setup() {
  setupPiezoSensors();
  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, HIGH);
  Serial.begin(9600);
  while (!Serial);
  Serial.println("--- SYSTEM BOOTING ---");

  spiBegin();
  for (uint8_t i = 0; i < NUM_READERS; i++) {
    pinMode(SS_PINS[i], OUTPUT);
    digitalWrite(SS_PINS[i], HIGH);
    initializeReader(SS_PINS[i]);
  }
}

void loop() {
  for (uint8_t i = 0; i < NUM_READERS; i++) {
    setActiveSSPin(SS_PINS[i]);
    byte uid[10];
    byte uidLength;

    if (isNewCardPresent()) {
      if (readCardUID(uid, uidLength)) {
        int mappedIndex = mapUidToIndex(uid);
        mappedIndexPerReader[i] = mappedIndex;
        uidValid[i] = (mappedIndex != -1);
      } else {
        mappedIndexPerReader[i] = -1;
        uidValid[i] = false;
      }

      haltCard();
    }
  }

  // Print mapping every 5 seconds
  if (millis() - lastPrintTime > 5000) {
    Serial.println("=== RFID Card Mapping ===");
    for (uint8_t i = 0; i < NUM_READERS; i++) {
      Serial.print("Reader ");
      Serial.print(i);
      Serial.print(": ");
      if (uidValid[i] && mappedIndexPerReader[i] != -1) {
        Serial.print("Card ");
        Serial.println(mappedIndexPerReader[i]);
      } else {
        Serial.println("No card");
      }
    }
    Serial.println("=========================");
    lastPrintTime = millis();
  }

  delay(200);
}
