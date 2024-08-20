#ifndef OLED_CONTROL_H
#define OLED_CONTROL_H

#include <Arduino.h>
#include <U8g2lib.h>
#include "macros.h"
#include "typedefs.h"
#include "util.h"
#include "frames.h"

extern void showAnimatedScreen(U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2, const unsigned char* frames[], int frameCount, bool continuous, int x, int y, int w, int h);
extern void showStartScreen(U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2);
extern void showQuestionScreen(U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2, Question question);
extern void showScoreScreen(U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2, int score);
extern Screen nextScreen(Screen currentScreen, bool correctAnswer, bool lastQuestion);

#endif