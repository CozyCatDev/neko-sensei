#ifndef MACROS_H
#define MACROS_H

#include <Arduino.h>

// uncomment code below to enable demo mode
#define DEMO_MODE

// pin declarations
#define SDA 21
#define SCL 22
#define DAC 25
#define SERVO 18
#define BUTTON_A 12
#define BUTTON_B 13
#define BUTTON_C 14
#define BUTTON_D 16

// audio sample rate is 8kHz
#define SAMPLE_RATE 8000

// timer ISR interval
#define TIMER_INTERVAL (1000000 / SAMPLE_RATE)

// time between servo actions in milliseconds
#define SERVO_INTERVAL 2000

// small font size dimensions
#define SMALL_CHAR_WIDTH 5
#define SMALL_CHAR_HEIGHT 8
// big font size dimensions
#define BIG_CHAR_WIDTH 8
#define BIG_CHAR_HEIGHT 13
// screen dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// use constants below when need to explicitly set reset pin and I2C address of screen
// not currently used since U8G2 finds the address for us
//#define OLED_RESET -1
//#define SCREEN_ADDRESS 0x7B

// delay between frames
#define FRAME_DELAY 150
// time to wait until next screen
#define NEXT_SCREEN_DELAY 4000

// maximum no. of lines question text can have
#define MAX_QUESTION_LINES 4
// maximum characters each question line can have
#define MAX_QUESTION_LINE_LENGTH 30
// maximum pixel width each question line can have
#define MAX_QUESTION_LINE_WIDTH 128

// question pool size
#define QUESTION_POOL_LENGTH 15
// no. of selected questions from question pool
#define QUIZ_LENGTH 5
// height of each line
#define LINE_HEIGHT 12
// slight y-offset for question options
#define OPTIONS_Y_GAP 6

#endif