#include "BitBangSPI.h"
#include "MFRC522_BitBang.h"
#include "config.h"
#include "PiezoSensors.h"
#include "GlobalState.h"
#include "UidMap.h"
#include "MP3Notes.h"

#define RST_PIN 48

// unsigned long lastPrintTime = 0;

unsigned long lastRFIDScanTime = 0;
const unsigned long RFID_SCAN_INTERVAL = 2000;  // 2 seconds

bool allTagsCorrect = false;
bool puzzleSolved = false;

char stepToNote[10] = {
  'D',  // A0
  'D',  // A1
  'D',  // A2
  'E',  // A3
  'E',  // A4
  'E',  // A5
  'D',  // A6
  'C',  // A7
  'D',  // A8
  'E'   // A9
};

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("--- SYSTEM BOOTING ---");

  // Initialize Piezo system (starts Timer1 interrupt)
  setupPiezoSensors();

  // Initialize MP3 system
  setupMP3();

  // Initialize RFID readers
  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, HIGH);

  spiBegin();
  for (uint8_t i = 0; i < NUM_RFID_READERS; i++) {
    pinMode(SS_PINS[i], OUTPUT);
    digitalWrite(SS_PINS[i], HIGH);  // Deselect
    initializeReader(SS_PINS[i]);
  }
}

void loop() {
  // --------- RFID Scanning ---------
  if (millis() - lastRFIDScanTime >= RFID_SCAN_INTERVAL) {
    allTagsCorrect = true;  // Assume true unless proven otherwise

    for (uint8_t i = 0; i < NUM_RFID_READERS; i++) {
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

      if (!(mappedIndexPerReader[i]-1 == i)) {
        Serial.print("MISMATCH: ");
        Serial.print(mappedIndexPerReader[i]);
        Serial.print(",");
        Serial.println(i);
        allTagsCorrect = false;  // Tag mismatch
      }
    }

    lastRFIDScanTime = millis();
  }

  // --------- Piezo Processing ---------
  for (int i = 0; i < NUM_PIEZO_SENSORS; i++) {
    if (!justPressed[i]) continue;

    playNote(stepToNote[i]);
    if (i == 0) {
      if (allTagsCorrect && !puzzleSolved) {
        Serial.println("Trigger solenoid!");
        puzzleSolved = true;

        // TODO: trigger solenoid, sound, light, etc.
      } else {
        Serial.println("A0 pressed — but tags not correct or already solved.");
      }
    }
    else if (i >= 1 && i <= 6) {
      int readerIndex = i - 1;
      if (uidValid[readerIndex] && mappedIndexPerReader[readerIndex] != -1) {
        Serial.print("A");
        Serial.println(mappedIndexPerReader[readerIndex]);
      } else {
        Serial.print("A");
        Serial.println(99);  // Fallback for no tag
      }
    }
    else {
      Serial.print("A");
      Serial.println(i);
    }

    justPressed[i] = false;
  }

  // --------- Periodic RFID Status Output ---------
  // if (millis() - lastPrintTime > 5000) {
  //   Serial.println("=== RFID Card Mapping ===");
  //   for (uint8_t i = 0; i < NUM_RFID_READERS; i++) {
  //     Serial.print("Reader ");
  //     Serial.print(i);
  //     Serial.print(": ");
  //     if (uidValid[i] && mappedIndexPerReader[i] != -1) {
  //       Serial.print("Card ");
  //       Serial.println(mappedIndexPerReader[i]);
  //     } else {
  //       Serial.println("No card");
  //     }
  //   }
  //   Serial.println("=========================");
  //   lastPrintTime = millis();
  // }

}
