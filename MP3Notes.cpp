#include "MP3Notes.h"
#include <Arduino.h>

SoftwareSerial mp3(ARDUINO_RX, ARDUINO_TX);

void setupMP3() {
  Serial.begin(9600);
  mp3.begin(9600);
  delay(500);
  mp3_command(CMD_SEL_DEV, DEV_TF);
  delay(200);
  mp3_command(CMD_SET_VOLUME, 30);  // Volume 0–30
}

void mp3_command(int8_t command, int16_t dat) {
  int8_t frame[8] = {0x7e, 0xff, 0x06, command, 0x00, dat >> 8, dat & 0xff, 0xef};
  for (uint8_t i = 0; i < 8; i++) {
    mp3.write(frame[i]);
  }
}

// Note indices must match MP3 file numbers on SD card
void playC()  { mp3_command(CMD_PLAY_W_INDEX, 2); }
void playCs() { mp3_command(CMD_PLAY_W_INDEX, 3); }
void playD()  { mp3_command(CMD_PLAY_W_INDEX, 4); }
void playDs() { mp3_command(CMD_PLAY_W_INDEX, 6); }
void playE()  { mp3_command(CMD_PLAY_W_INDEX, 7); }
void playF()  { mp3_command(CMD_PLAY_W_INDEX, 8); }
void playFs() { mp3_command(CMD_PLAY_W_INDEX, 9); }
void playG()  { mp3_command(CMD_PLAY_W_INDEX, 10); }
void playGs() { mp3_command(CMD_PLAY_W_INDEX, 11); }
void playA()  { mp3_command(CMD_PLAY_W_INDEX, 12); }
void playAs() { mp3_command(CMD_PLAY_W_INDEX, 5); }
void playB()  { mp3_command(CMD_PLAY_W_INDEX, 1); }

void playNote(char note) {
  switch (note) {
    case 'C': playC();  break;
    case 'c': playCs(); break;

    case 'D': playD();  break;
    case 'd': playDs(); break;

    case 'E': playE();  break;

    case 'F': playF();  break;
    case 'f': playFs(); break;

    case 'G': playG();  break;
    case 'g': playGs(); break;

    case 'A': playA();  break;
    case 'a': playAs(); break;

    case 'B': playB();  break;

    default:
      Serial.print("Unknown note: ");
      Serial.println(note);
      break;
  }
}
