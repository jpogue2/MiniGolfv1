#ifndef MP3_NOTES_H
#define MP3_NOTES_H

#include <SoftwareSerial.h>

// --- MP3 Command Constants ---
#define CMD_PLAY_W_INDEX 0x03
#define CMD_SET_VOLUME   0x06
#define CMD_SEL_DEV      0x09
#define DEV_TF           0x02

// --- Serial Pins for MP3 Module ---
#define ARDUINO_RX       7
#define ARDUINO_TX       6

extern SoftwareSerial mp3;

void setupMP3();
void mp3_command(int8_t command, int16_t dat);

// Note playback functions
void playC();
void playCs();
void playD();
void playDs();
void playE();
void playF();
void playFs();
void playG();
void playGs();
void playA();
void playAs();
void playB();

void playNote(char note);

#endif
