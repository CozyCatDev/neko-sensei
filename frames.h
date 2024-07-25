#ifndef FRAMES_H
#define FRAMES_H

#include <Arduino.h>
#include <U8g2lib.h>

// 128x64 = 8192 pixels or bits, 8192 / 8 = 1024 bytes
extern const unsigned char startScreen1[1024] U8X8_PROGMEM;
extern const unsigned char startScreen2[1024] U8X8_PROGMEM;
extern const unsigned char startScreen3[1024] U8X8_PROGMEM;
extern const unsigned char startScreen4[1024] U8X8_PROGMEM;
extern const unsigned char startScreen5[1024] U8X8_PROGMEM;
extern const unsigned char startScreen6[1024] U8X8_PROGMEM;
extern const unsigned char startScreen7[1024] U8X8_PROGMEM;
extern const unsigned char startScreen8[1024] U8X8_PROGMEM;
extern const int startScreenLen;
extern const unsigned char* startScreenFrames[8];

extern const unsigned char correctScreen1[1024] U8X8_PROGMEM;
extern const unsigned char correctScreen2[1024] U8X8_PROGMEM;
extern const unsigned char correctScreen3[1024] U8X8_PROGMEM;
extern const unsigned char correctScreen4[1024] U8X8_PROGMEM;
extern const int correctScreenLen;
extern const unsigned char* correctScreenFrames[4];

extern const unsigned char wrongScreen1[1024] U8X8_PROGMEM;
extern const unsigned char wrongScreen2[1024] U8X8_PROGMEM;
extern const unsigned char wrongScreen3[1024] U8X8_PROGMEM;
extern const unsigned char wrongScreen4[1024] U8X8_PROGMEM;
extern const int wrongScreenLen;
extern const unsigned char* wrongScreenFrames[4];

#endif