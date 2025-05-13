#include "GlobalState.h"
#include <stdint.h>

int8_t mappedIndexPerReader[NUM_RFID_READERS] = {-1};
bool uidValid[NUM_RFID_READERS] = {false};

bool allTagsCorrect = false;
bool puzzleSolved = false;
