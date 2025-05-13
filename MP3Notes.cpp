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

// Individual note functions
// void playC1()     { mp3_command(CMD_PLAY_W_INDEX, 1); Serial.println("C"); }
// void playD()      { mp3_command(CMD_PLAY_W_INDEX, 2); Serial.println("D"); }
// void playE()      { mp3_command(CMD_PLAY_W_INDEX, 3); Serial.println("E"); }
// void playF()      { mp3_command(CMD_PLAY_W_INDEX, 4); Serial.println("F"); }
// void playB()      { mp3_command(CMD_PLAY_W_INDEX, 5); Serial.println("B"); }
// void playC2()     { mp3_command(CMD_PLAY_W_INDEX, 6); Serial.println("c"); }
// void playVictory(){ mp3_command(CMD_PLAY_W_INDEX, 7); Serial.println("V"); }
// void playBad()    { mp3_command(CMD_PLAY_W_INDEX, 8); Serial.println("X"); }

void playC1()     { Serial.println("C"); }
void playD()      { Serial.println("D"); }
void playE()      { Serial.println("E"); }
void playF()      { Serial.println("F"); }
void playB()      { Serial.println("B"); }
void playC2()     { Serial.println("c"); }
void playVictory(){ Serial.println("V"); }
void playBad()    { Serial.println("X"); }

void playNote(char note) {
  switch (note) {
    case 'C': playC1(); break;   // Low C
    case 'D': playD();  break;
    case 'E': playE();  break;
    case 'F': playF();  break;
    case 'B': playB();  break;
    case 'c': playC2(); break;   // High C

    case 'V': playVictory(); break;  // Victory sound
    case 'X': playBad();     break;  // Bad sound

    default:
      Serial.print("Unknown note: ");
      Serial.println(note);
      break;
  }
}