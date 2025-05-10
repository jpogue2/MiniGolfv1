#include "BitBangSPI.h"
#include "MFRC522_BitBang.h"
#include "UidMap.h"

int mapUidToIndex(const byte *uid) {
  for (int i = 0; i < 8; i++) {
    bool match = true;
    for (int j = 0; j < 4; j++) {
      if (uid[j] != knownUids[i][j]) {
        match = false;
        break;
      }
    }
    if (match) return i;
  }
  return -1;  // Not found
}

uint8_t readFirmwareVersion(uint8_t ssPin) {
  setActiveSSPin(ssPin);
  return readRegister(0x37);  // 0x37 = VersionReg
}