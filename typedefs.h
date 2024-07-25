#ifndef TYPEDEFS_H
#define TYPEDEFS_H

#include <Arduino.h>

// enum used to store screen state
enum Screen {START, QUESTION, CORRECT, WRONG, SCORE};

// struct that defines information a question should have
struct Question{
  char* category;
  char* difficulty;
  char* text;
  char* options[4];
  char answer;
};

#endif