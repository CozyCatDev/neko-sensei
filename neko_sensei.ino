/*
=============== MAIN PROBLEMS ===============
1. There's not enough flash memory to store animation frames.
2. U8G2 library taking up a lot of RAM even though using recommended
   page buffer mode which takes us less RAM. This limits the program
   functionality so features like random questions can't be added.
   Currently, 99% of program storage space is in use. Adding even a
   little more code causes the program to fail or the display to
   become erratic.
3. Due to limited RAM, other libraries such as Servo and audio control
   libraries might not fit into the program.
4. Even after adding SD card to store as many images and audio files
   as we want, RAM issue still persists. Easiest solution is to use
   Arduino Mega?

=============== ACHIEVEMENTS ===============
1. OLED can display limited animations due to limited flash memory (PROGMEM).
2. Quiz is working as intended.

=============== TODO ===============
1. Implement reading of animation frames and audio files from SD card.
2. Implement selection of random questions from question pool.
3. Implement audio playback for all scenarios.
4. Implement prize giving mechanism with servo at the end of each quiz.
5. Look into other alternatives. Arduino Mega has 256kB of flash memory
   and 8kB of SRAM. The Arduino UNO only has 32kB of flash memory and
   2kB of SRAM. If Mega is used and works, SD card is no longer required.
   Adding RAM modules could also be possible?
6. Test program with Arduino Mega on Wokwi.
7. Re-draw some animations.
8. Add more questions to pool.
9. Re-think how questions are arranged. Sort by difficulty? How many
   questions per quiz? Quiz evolves during STEM showcase?

=============== INSTRUCTIONS ===============
1. When start screen is shown, press any button to continue.
2. When question is shown, choose an option (A,B,C,D). In Wokwi, buttons
   are A, B, C, D from top to bottom. Answers are A, D, D, B, A for quick
   reference.
3. Final score is shown at the end. After that, quiz loops.
4. Enable demo mode by uncommenting the code in "macros.h". Demo mode
   scrolls through all the possible screens automatically without using
   push buttons. The correct and wrong screens are shown randomly. The
   score summed up at the end is thus random as well.
*/

// include Arduino libraries
#include <Arduino.h>
#include <pgmspace.h>
#include <driver/dac.h>
#include <driver/timer.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <ESP32Servo.h>

// include created header files
#include "macros.h"
#include "typedefs.h"
#include "frames.h"
#include "sound_effects.h"
#include "util.h"
#include "oled_control.h"

// is current question the last question?
bool lastQuestion = false;
// is answer chosen correct?
bool correctAnswer;
// has any button been pressed?
char buttonPressed;
// boolean to check if next quiz is starting
bool newQuiz = true;
// keep track of current question
int questionCounter = 0;
// keep track of score
int scoreCounter = 0;

// create u8g2 object for SSD1306 I2C display
// '1' stands for page screen buffer which saves more RAM
// '2' stores 2 pages in buffer, so it's x2 faster than '1' but eats more RAM
// 'F' stands for full screen buffer which is faster but eats more RAM
U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 22, 21);

// enum for storing current screen state
Screen currentScreen;

// question pool using "Question" struct
Question questions[QUESTION_POOL_LENGTH] = {
  {
    "Circuit Theory",
    "<Easy>",
    "Which equation represents Ohm's Law.",
    {"A. R=V/I", "B. V=I*V", "C. I=2*R", "D. V=2*I"},
    'A'
  },
  {
    "Programming",
    "<Easy>",
    "Which data type stores a whole number?",
    {"A. long", "B. float", "C. byte", "D. int"},
    'D'
  },
  {
    "Circuit Theory",
    "<Easy>",
    "If voltage is 12V and current is 6A, what is the power?",
    {"A. 72W","B. 18W","C. 32W","D. 2W"},
    'A'
  },
  {
    "Circuit Theory",
    "<Easy>",
    "What does an ammeter measure?",
    {"A. Current", "B. Voltage", "C. Reactance", "D. Power"},
    'A'
  },
  {
    "Circuit Theory",
    "<Medium>",
    "What stores energy in a magnetic field?",
    {"A. Resistor","B. Capacitor","C. Diode","D. Inductor"},
    'D'
  },
  {
    "Rectifiers",
    "<Medium>",
    "What is the typical forward diode voltage drop?",
    {"A. 1.2V", "B. 0.7V", "C. 0.3V", "D. 3V"},
    'B'
  },
  {
    "General",
    "<Medium>",
    "What level of supply voltage does Malaysia use?",
    {"A. 120V,60Hz","B. 240V,60Hz","C. 100V,60Hz","D. 240V,50Hz"},
    'D'
  },
  {
    "Programming",
    "<Medium>",
    "How many bits does 3 bytes represent?",
    {"A. 18","B. 24","C. 36","D. 48"},
    'B'
  },
  {
    "Logic",
    "<Medium>",
    "What type of signal is used in digital electronics?",
    {"A. Analog", "B. Sine wave", "C. Binary", "D. Sawtooth"},
    'C'
  },
  {
    "Logic",
    "<Hard>",
    "What is the hexadecimal representation of 38?",
    {"A. 0x26", "B. 0x22", "C. 0x12", "D. 0xC0"},
    'A'
  },
  {
    "Transformers",
    "<Hard>",
    "Input is 1200V. Turn ratio is 5:1. What is the output?",
    {"A. 360V", "B. 300V", "C. 480V", "D. 240V"},
    'D'
  },
  {
    "Logic",
    "<Hard>",
    "If A = 1011, B = 0101, what is A XOR B?",
    {"A. 1111", "B. 0000", "C. 1110", "D. 0001"},
    'C'
  }
};

// random questions [NOT IMPLEMENTED YET]
Question randomQuestions[QUIZ_LENGTH];

// returns amount of free RAM (not accurate?)
// source: https://docs.arduino.cc/learn/programming/memory-guide/
/*
int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0  
    ? (int)&__heap_start : (int) __brkval);  
}
*/

void setup() {
  // declare all button pins as input with pullup resistor enabled
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  pinMode(BUTTON_D, INPUT_PULLUP);
  
  setupTimerDACServo();

  #ifdef DEMO_MODE
  // ensure random() is different each time the sketch runs
  randomSeed(analogRead(0));
  #endif
  Serial.begin(115200);
  
  // initialize display
  u8g2.begin();
  // set current screen to start screen
  currentScreen = START;
  // set reference position for all text to be its top, e.g. (0,0) is top left
  u8g2.setFontPosTop();
  // set font family and font size, character has width of 5 and height of 8
  u8g2.setFont(u8g2_font_spleen5x8_mf);
}

// PROGRAM FLOW:
// START -> QUESTION_TITLE -> QUESTION -> WRONG/CORRECT -> EXPLANATION -> SCORE -> START
// QUESTION -> EXPLANATION -> QUESTION (if not last question)

void loop() {
  // print amount of RAM at start of each loop
  //Serial.print(F("SRAM left: "));
  //Serial.println(freeRam());
  playSoundEffect(currentScreen, scoreCounter, questionCounter == 0);
  if(newQuiz){
    getRandomQuestions(questions, randomQuestions);
    newQuiz = false;
  }
  if(currentScreen == START){
    // show animated start screen if currentScreen == START
    #ifdef DEMO_MODE
    showAnimatedScreen(u8g2, startScreenFrames, startScreenLen, false, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    #else
    showStartScreen(u8g2);
    #endif
  }
  else if(currentScreen == QUESTION){
    // show question screen if currentScreen == QUESTION
    showQuestionScreen(u8g2, randomQuestions[questionCounter]);
    #ifdef DEMO_MODE
    // randomly show correct or wrong screen
    correctAnswer = random(0, 2) == 0 ? true : false;
    delay(NEXT_SCREEN_DELAY);
    #else
    // get button after debouncing it
    buttonPressed = getDebouncedButton();
    // check if answer chosen is correct, update score and check if last question accordingly
    correctAnswer = buttonPressed == randomQuestions[questionCounter].answer;
    #endif
    scoreCounter = correctAnswer ? scoreCounter + 1 : scoreCounter;
    lastQuestion = questionCounter == QUIZ_LENGTH - 1;
    questionCounter++;
  }
  else if(currentScreen == CORRECT){
    // show animated correct screen if currentScreen == CORRECT
    showAnimatedScreen(u8g2, correctScreenFrames, correctScreenLen, false, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  }
  else if(currentScreen == WRONG){
    // currently prints "Wrong...", no animated screen implemented due to insufficient RAM
    showAnimatedScreen(u8g2, wrongScreenFrames, wrongScreenLen, false, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // code below used for Arduino since it doesn't have enough RAM
    /*
    u8g2.firstPage();
    do{
      u8g2.drawStr((SCREEN_WIDTH - 8 * SMALL_CHAR_WIDTH) / 2, (SCREEN_HEIGHT - SMALL_CHAR_HEIGHT) / 2, "Wrong...");
    }while(u8g2.nextPage());
    // show wrong screen for NEXT_SCREEN_DELAY seconds
    delay(NEXT_SCREEN_DELAY);
    */
    //showAnimatedScreen(u8g2, wrongScreenFrames, wrongScreenLen, false);
  }
  else if(currentScreen == SCORE){
    // show final score if currentScreen == SCORE
    showScoreScreen(u8g2, scoreCounter);
    // reset scoreCounter after quiz ends
    scoreCounter = 0;
    // reset questionCounter after quiz ends
    questionCounter = 0;
    newQuiz = true;
  }
  // move on to next screen
  currentScreen = nextScreen(currentScreen, correctAnswer, lastQuestion);
}