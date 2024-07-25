#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>
#include "macros.h"
#include "typedefs.h"

extern void splitStringByWidth(const char* text, char lines[][MAX_QUESTION_LINE_LENGTH]);
extern char getButtonPressed();
extern char getDebouncedButton();
extern void getRandomQuestions(Question questionPool[QUESTION_POOL_LENGTH], Question randomQuestions[QUIZ_LENGTH]);

#endif