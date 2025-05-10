#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H

#include "config.h"
#include <stdint.h>

extern int8_t mappedIndexPerReader[NUM_RFID_READERS];
extern bool uidValid[NUM_RFID_READERS];

#endif
