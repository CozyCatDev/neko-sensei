#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include <driver/timer.h>
#include <driver/dac.h>
#include "macros.h"
#include "typedefs.h"
#include "sound_effects.h"

extern void splitStringByWidth(const char* text, char lines[][MAX_QUESTION_LINE_LENGTH]);
extern char getButtonPressed();
extern char getDebouncedButton();
extern void getRandomQuestions(Question questionPool[QUESTION_POOL_LENGTH], Question randomQuestions[QUIZ_LENGTH]);
extern void setupTimerDACServo();
extern void playSoundEffect(Screen currentScreen, int score, bool newQuiz);

#endif