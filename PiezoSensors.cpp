#include "PiezoSensors.h"
#include <TimerOne.h>

const int analogPins[NUM_PIEZO_SENSORS] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
int thresholds[NUM_PIEZO_SENSORS] = {20, 20, 20, 20, 10, 20, 20, 20, 100, 20};

bool pressed[NUM_PIEZO_SENSORS] = {false};          // Current state
bool justPressed[NUM_PIEZO_SENSORS] = {false};      // Set to true on rising edge
int lastReadings[NUM_PIEZO_SENSORS] = {0};

unsigned long lastGlobalTriggerTime = 0;
const unsigned long PIEZO_COOLDOWN = 600;  // 600 ms


void setupPiezoSensors() {
  for (int i = 0; i < NUM_PIEZO_SENSORS; i++) {
    pinMode(analogPins[i], INPUT_PULLUP);
    lastReadings[i] = analogRead(analogPins[i]);
  }

  Timer1.initialize(2000);  // 200 Hz
  Timer1.attachInterrupt(readPiezoSensors);
}

void readPiezoSensors() {
  unsigned long now = millis();

  // Ignore all sensors if still in cooldown
  if (now - lastGlobalTriggerTime < PIEZO_COOLDOWN) {
    return;
  }

  for (int i = 0; i < NUM_PIEZO_SENSORS; i++) {
    int reading = analogRead(analogPins[i]);
    int delta = abs(reading - lastReadings[i]);

    if (!pressed[i] && delta > thresholds[i]) {
      pressed[i] = true;
      justPressed[i] = true;
      lastGlobalTriggerTime = now;  // Start cooldown for all sensors
      break;  // Only accept the first trigger
    } else if (pressed[i] && delta <= thresholds[i]) {
      pressed[i] = false;
    }

    lastReadings[i] = reading;
  }
}


