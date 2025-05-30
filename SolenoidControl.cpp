#include "SolenoidControl.h"
#include "GlobalState.h"
#include "PiezoSensors.h"

const int solenoidPin = 25;
const int buttonPin = 24;

static bool solenoidOn = false;
static bool lastButtonState = HIGH;
static bool stableButtonState = HIGH;
static unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setupSolenoid() {
  pinMode(solenoidPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(solenoidPin, LOW);
}

void updateSolenoid() {
  bool reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != stableButtonState) {
      stableButtonState = reading;
      if (stableButtonState == LOW) {
        solenoidOn = !solenoidOn;
        digitalWrite(solenoidPin, solenoidOn ? HIGH : LOW);
        puzzleSolved = false;
        allTagsCorrect = false;
        delay(2000);
        justPressed[0] = false;
        justPressed[1] = false;
      }
    }
  }

  lastButtonState = reading;
}

void triggerSolenoid() {
  solenoidOn = !solenoidOn;
  digitalWrite(solenoidPin, solenoidOn ? HIGH : LOW);
}
