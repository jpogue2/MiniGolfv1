#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define NUM_RFID_READERS 6
#define NUM_PIEZO_SENSORS 10

extern const uint8_t SS_PINS[NUM_RFID_READERS];  // Declare globally accessible SS pins

#endif
