#include "BitBangSPI.h"
#include "MFRC522_BitBang.h"
#include "config.h"
#include "PiezoSensors.h"
#include "GlobalState.h"
#include "UidMap.h"
#include "MP3Notes.h"
#include "SolenoidControl.h"

#define RST_PIN 48

unsigned long lastRFIDScanTime = 0;
const unsigned long RFID_SCAN_INTERVAL = 2000;

unsigned long lastVersionCheckTime = 0;
const unsigned long VERSION_POLL_INTERVAL = 10000;  // 10 seconds

bool allTagsCorrect = false;
bool puzzleSolved = false;

uint8_t firmwareVersions[NUM_RFID_READERS] = {0};

char stepToNote[9] = {
  'c',  // A1
  'B',  // A2
  'F',  // A3
  'E',  // A4
  'D',  // A5
  'C',  // A6
  'D',  // A7
  'C',  // A8
  'C'   // A9
};

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("--- SYSTEM BOOTING ---");

  // Initialize subsystems
  setupPiezoSensors();
  setupMP3();
  setupSolenoid();

  pinMode(RST_PIN, OUTPUT);
  digitalWrite(RST_PIN, HIGH);

  spiBegin();
  for (uint8_t i = 0; i < NUM_RFID_READERS; i++) {
    pinMode(SS_PINS[i], OUTPUT);
    digitalWrite(SS_PINS[i], HIGH);
    initializeReader(SS_PINS[i]);

    // Optional: print firmware version
    firmwareVersions[i] = readFirmwareVersion(SS_PINS[i]);
    Serial.print("READER_");
    Serial.print(i);
    Serial.print(",VERSION:0x");
    Serial.print(firmwareVersions[i], HEX);
    Serial.print(",STATUS:");
    Serial.println(firmwareVersions[i] == 0x92 ? "CONNECTED" : "UNCONNECTED");
  }
}

void loop() {
  // --------- RFID Scanning ---------
  if (millis() - lastRFIDScanTime >= RFID_SCAN_INTERVAL) {
    allTagsCorrect = true;

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

      if (!(mappedIndexPerReader[i] - 1 == i)) {
        Serial.print("RFID_MISMATCH,READER_");
        Serial.print(i);
        Serial.print(",MAP_");
        Serial.println(mappedIndexPerReader[i]);
        allTagsCorrect = false;
      }
    }

    // Output RFID status block
    Serial.println("RFID_STATUS_BEGIN");
    for (uint8_t i = 0; i < NUM_RFID_READERS; i++) {
      Serial.print("READER_");
      Serial.print(i);
      Serial.print(",UID_VALID:");
      Serial.print(uidValid[i] ? "1" : "0");
      Serial.print(",MAP:");
      Serial.println(mappedIndexPerReader[i]);
    }
    Serial.println("RFID_STATUS_END");

    lastRFIDScanTime = millis();
  }

  // --------- Piezo Processing ---------
  for (int i = 0; i < NUM_PIEZO_SENSORS; i++) {
    if (!justPressed[i]) continue;

    Serial.print("PIEZO_");
    Serial.print(i);
    Serial.println(",ACTIVATED");

    if (i == 0) {
      if (allTagsCorrect && !puzzleSolved) {
        Serial.println("SOLENOID_TRIGGER");
        puzzleSolved = true;
        playNote('V');
        triggerSolenoid();
      } else {
        Serial.println("A0_IGNORED");  // Conditions not met
      }
    }
    else if (i >= 1 && i <= 6) {
      int readerIndex = i - 1;
      playNote(stepToNote[mappedIndexPerReader[readerIndex]-1]);
      if (uidValid[readerIndex] && mappedIndexPerReader[readerIndex] != -1) {
        Serial.print("A");
        Serial.println(mappedIndexPerReader[readerIndex]);
      } else {
        Serial.println("A99");  // Fallback
      }
    }
    else {
      playNote(stepToNote[i-1]);
      Serial.print("A");
      Serial.println(i);
    }

    justPressed[i] = false;
  }

  // ------- Solenoid Activation -------
  updateSolenoid();

  // --------- Version Polling ---------
  // if (millis() - lastVersionCheckTime >= VERSION_POLL_INTERVAL) {
  //   for (uint8_t i = 0; i < NUM_RFID_READERS; i++) {
  //     firmwareVersions[i] = readFirmwareVersion(SS_PINS[i]);

  //     Serial.print("READER_");
  //     Serial.print(i);
  //     Serial.print(",VERSION:0x");
  //     Serial.print(firmwareVersions[i], HEX);
  //     Serial.print(",STATUS:");
  //     Serial.println(firmwareVersions[i] == 0x92 ? "CONNECTED" : "UNCONNECTED");
  //   }

  //   lastVersionCheckTime = millis();
  // }
}
