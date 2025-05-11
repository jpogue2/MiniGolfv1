#ifndef SOLENOID_CONTROL_H
#define SOLENOID_CONTROL_H

#include <Arduino.h>

void setupSolenoid();
void updateSolenoid();
void triggerSolenoid();  // External trigger (non-button)

#endif
