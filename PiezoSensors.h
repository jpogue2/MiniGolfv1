#ifndef PIEZO_SENSORS_H
#define PIEZO_SENSORS_H

#define NUM_PIEZO_SENSORS 10

extern bool pressed[NUM_PIEZO_SENSORS];
extern bool justPressed[NUM_PIEZO_SENSORS];

void setupPiezoSensors();
void readPiezoSensors();

#endif
