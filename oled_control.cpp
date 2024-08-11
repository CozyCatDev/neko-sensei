#include "oled_control.h"

// show an animated screen
void showAnimatedScreen(U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2, const unsigned char* frames[], int frameCount, bool continuous){
  int counter = 0;
  unsigned long animationTimer = millis();
  // animation keeps playing if any of the buttons were not pressed
  // or animation keeps playing for NEXT_SCREEN_DELAY seconds if continous option was not chosen
  while((!getButtonPressed() && continuous) || (millis() - animationTimer < NEXT_SCREEN_DELAY && !continuous)){
    // typical of page screen buffer mode uses a do while loop
    u8g2.firstPage();
    do{
      // draw bitmap, x = 0, y = 0, w = SCREEN_WIDTH, h = SCREEN_HEIGHT, bitmap = frames[counter]
      u8g2.drawXBMP(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, frames[counter]);
    }while(u8g2.nextPage());
    // counter used to keep track of current frame
    // if 8 frames, counter sequence is
    // 0, 1, 2, 3, 4, 5, 6, 7, 0, ...
    counter = (counter + 1) % frameCount;
    // wait FRAME_DELAY seconds before next frame
    delay(FRAME_DELAY);
  }
}

// show question category and difficulty, then question itself along with options
void showQuestionScreen(U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2, Question question){
  // array used to store question lines that can fit onto the screen
  char lines[MAX_QUESTION_LINES][MAX_QUESTION_LINE_LENGTH] = {};
  // split question.text into multiple lines, that is stored in lines array
  splitStringByWidth(question.text, lines);
  // show question category and difficulty
  u8g2.firstPage();
  do{
    // use larger font size, character width is 8px and character height is 13px
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr((SCREEN_WIDTH - strlen(question.category) * BIG_CHAR_WIDTH) / 2, (SCREEN_HEIGHT - BIG_CHAR_HEIGHT) / 2 - 12, question.category);
    u8g2.drawStr((SCREEN_WIDTH - strlen(question.difficulty) * BIG_CHAR_WIDTH) / 2, (SCREEN_HEIGHT - BIG_CHAR_HEIGHT) / 2 + 12, question.difficulty);
  }while(u8g2.nextPage());
  // set font size back to small
  u8g2.setFont(u8g2_font_spleen5x8_mf);
  // wait for NEXT_SCREEN_DELAY seconds
  delay(NEXT_SCREEN_DELAY);
  unsigned long questionTimer = millis();
  // show question along with options
  while(millis() - questionTimer < QUESTION_DURATION && !getButtonPressed()){
    int progressBarPixels = round(((millis() - questionTimer) * SCREEN_WIDTH) / QUESTION_DURATION);
    u8g2.firstPage();
    do{
      int cursorX = 0;
      int cursorY = 0;
      // show question lines that can fit onto the screen
      for (int i = 0; i < MAX_QUESTION_LINES; i++){
        Serial.println(strlen(lines[i]));
        if (strlen(lines[i]) > 0){
          u8g2.drawStr(cursorX, cursorY, lines[i]);
          cursorY += LINE_HEIGHT;
        }
      }
      u8g2.drawStr(cursorX, cursorY + QUESTIONS_OPTIONS_GAP, question.options[0]);
      u8g2.drawStr(cursorX + 64, cursorY + QUESTIONS_OPTIONS_GAP, question.options[1]);
      u8g2.drawStr(cursorX, cursorY + LINE_HEIGHT + OPTIONS_GAP, question.options[2]);
      u8g2.drawStr(cursorX + 64, cursorY + LINE_HEIGHT + OPTIONS_GAP, question.options[3]);
      u8g2.drawBox(0, SCREEN_HEIGHT - PROGRESS_BAR_HEIGHT, progressBarPixels, PROGRESS_BAR_HEIGHT);
    }while(u8g2.nextPage());
  }
}

// show final score
void showScoreScreen(U8G2_SSD1309_128X64_NONAME0_1_HW_I2C u8g2, int score){
  // e.g. text format is "5 / 5"
  String scoreString = String(score) + " / " + String(QUIZ_LENGTH);
  // show score
  u8g2.firstPage();
  do{
    u8g2.setFont(u8g2_font_8x13_mf);
    u8g2.drawStr((SCREEN_WIDTH - strlen("Your score:") * BIG_CHAR_WIDTH) / 2, (SCREEN_HEIGHT - BIG_CHAR_HEIGHT) / 2 - 12, "Your Score:");
    u8g2.setCursor((SCREEN_WIDTH - scoreString.length() * BIG_CHAR_WIDTH) / 2, (SCREEN_HEIGHT - BIG_CHAR_HEIGHT) / 2 + 12);
    u8g2.print(scoreString);
  }while(u8g2.nextPage());
}

// returns next screen based on current screen
Screen nextScreen(Screen currentScreen, bool correctAnswer, bool lastQuestion){
  if(currentScreen == START){
    return QUESTION;
  }
  else if(currentScreen == QUESTION){
    return correctAnswer ? CORRECT : WRONG;
  }
  else if(currentScreen == CORRECT || currentScreen == WRONG){
    return lastQuestion ? SCORE : QUESTION;
  }
  else if(currentScreen == SCORE){
    return START;
  }
}