#pragma once
#include <Arduino.h>

void setupPiezoSensors();
void readPiezoSensors();

// For integration with UID lookup
extern bool uidValid[];
extern byte uidPerReader[][4];
