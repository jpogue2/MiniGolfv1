#include "PiezoSensors.h"
#include <TimerOne.h>

const int analogPins[NUM_PIEZO_SENSORS] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9};
int thresholds[NUM_PIEZO_SENSORS] = {20, 20, 20, 20, 10, 20, 20, 20, 20, 20};

bool pressed[NUM_PIEZO_SENSORS] = {false};          // Current state
bool justPressed[NUM_PIEZO_SENSORS] = {false};      // Set to true on rising edge
int lastReadings[NUM_PIEZO_SENSORS] = {0};

void setupPiezoSensors() {
  for (int i = 0; i < NUM_PIEZO_SENSORS; i++) {
    pinMode(analogPins[i], INPUT_PULLUP);
    lastReadings[i] = analogRead(analogPins[i]);
  }

  Timer1.initialize(2000);  // 200 Hz
  Timer1.attachInterrupt(readPiezoSensors);
}

void readPiezoSensors() {
  for (int i = 0; i < NUM_PIEZO_SENSORS; i++) {
    int reading = analogRead(analogPins[i]);
    int delta = abs(reading - lastReadings[i]);

    if (!pressed[i] && delta > thresholds[i]) {
      pressed[i] = true;
      justPressed[i] = true;  // Will be picked up in loop()
    } else if (pressed[i] && delta <= thresholds[i]) {
      pressed[i] = false;
    }

    lastReadings[i] = reading;
  }
}
