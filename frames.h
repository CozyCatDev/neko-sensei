#ifndef FRAMES_H
#define FRAMES_H

#include <Arduino.h>
#include <U8g2lib.h>

extern const unsigned char nekoSenseiText[1024] U8X8_PROGMEM;

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

extern const unsigned char winScreen1[1024] U8X8_PROGMEM;
extern const unsigned char winScreen2[1024] U8X8_PROGMEM;
extern const unsigned char winScreen3[1024] U8X8_PROGMEM;
extern const unsigned char winScreen4[1024] U8X8_PROGMEM;
extern const unsigned char winScreen5[1024] U8X8_PROGMEM;
extern const unsigned char winScreen6[1024] U8X8_PROGMEM;
extern const unsigned char winScreen7[1024] U8X8_PROGMEM;
extern const unsigned char winScreen8[1024] U8X8_PROGMEM;
extern const int winScreenLen;
extern const unsigned char* winScreenFrames[8];

extern const unsigned char loseScreen1[1024] U8X8_PROGMEM;
extern const unsigned char loseScreen2[1024] U8X8_PROGMEM;
extern const unsigned char loseScreen3[1024] U8X8_PROGMEM;
extern const unsigned char loseScreen4[1024] U8X8_PROGMEM;
extern const unsigned char loseScreen5[1024] U8X8_PROGMEM;
extern const unsigned char loseScreen6[1024] U8X8_PROGMEM;
extern const unsigned char loseScreen7[1024] U8X8_PROGMEM;
extern const unsigned char loseScreen8[1024] U8X8_PROGMEM;
extern const int loseScreenLen;
extern const unsigned char* loseScreenFrames[8];

#endif