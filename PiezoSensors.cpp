#include "PiezoSensors.h"
#include "config.h"
#include <TimerOne.h>
#include "GlobalState.h"

int8_t mappedIndexPerReader[NUM_READERS] = {-1};  // Initialize all to -1
bool uidValid[NUM_READERS] = {false};

const int numPins = 10;
const int analogPins[numPins] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
int thresholds[numPins] = {20, 20, 20, 20, 10, 20, 20, 1000, 1000, 1000};
bool pressed[numPins] = {false};
int lastReadings[numPins] = {0};

void setupPiezoSensors() {
  for (int i = 0; i < numPins; i++) {
    pinMode(analogPins[i], INPUT_PULLUP);
    lastReadings[i] = analogRead(analogPins[i]);
  }

  // Optional onboard LED for debugging
  pinMode(LED_BUILTIN, OUTPUT);

  Timer1.initialize(5000);  // 5ms = 200 Hz
  Timer1.attachInterrupt(readPiezoSensors);
}

void readPiezoSensors() {
  for (int i = 0; i < NUM_READERS; i++) {
    int reading = analogRead(analogPins[i]);
    int delta = abs(reading - lastReadings[i]);

    if (!pressed[i] && delta > thresholds[i]) {
      pressed[i] = true;

      if (uidValid[i] && mappedIndexPerReader[i] != -1) {
        int cardID = mappedIndexPerReader[i];
        int readerID = i;
        // Do something like: cardID is on readerID
      }
    
      // Optional: debug pulse
      digitalWrite(LED_BUILTIN, HIGH);
      delayMicroseconds(100);
      digitalWrite(LED_BUILTIN, LOW);

      // You can trigger additional behavior here based on uidPerReader[i]
    } else if (pressed[i] && delta <= thresholds[i]) {
      pressed[i] = false;
    }

    lastReadings[i] = reading;
  }
}
