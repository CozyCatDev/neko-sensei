#include "util.h"

// hardware timer for handling sound playback
hw_timer_t* timer = NULL;
// index for keeping track of sound byte being played
volatile int soundIdx = 0;
volatile uint32_t startSongIdx = 0;
volatile uint32_t questionSongIdx = 0;
volatile const uint8_t* currentSoundBytes = nullptr;
volatile unsigned int currentSoundLength = 0;

bool playSound = false;
bool controlServo = false;
bool turnServo = false;
unsigned long currentMillis = 0;

bool onStartScreen = false;

Servo servo;

// takes long "text" argument to split and stores it in "lines" array argument passed in
// used to split long text that overflows OLED screen into multiple lines that can fit
// source: ChatGPT
void splitStringByWidth(const char* text, char lines[][MAX_QUESTION_LINE_LENGTH]) {
  int lineCount = 0;
  char currentLine[MAX_QUESTION_LINE_LENGTH] = "";
  int currentLineWidth = 0;

  const char* wordStart = text;
  const char* currentPos = text;

  while (*currentPos != '\0') {
    // Find the next word (delimited by whitespace)
    if (*currentPos == ' ' || *(currentPos + 1) == '\0') {
      int wordLength = currentPos - wordStart + (*(currentPos + 1) == '\0' ? 1 : 0);
      int wordWidth = wordLength * SMALL_CHAR_WIDTH;

      // Check if adding this word would exceed the max width
      if (currentLineWidth + wordWidth > MAX_QUESTION_LINE_WIDTH) {
        // Add current line to array
        strncpy(lines[lineCount], currentLine, MAX_QUESTION_LINE_LENGTH);
        lineCount++;
        if (lineCount >= MAX_QUESTION_LINES) {
          break; // Exit loop if MAX_QUESTION_LINES reached
        }

        // Start new line with the current word
        strncpy(currentLine, wordStart, wordLength);
        currentLine[wordLength] = '\0';
        currentLineWidth = wordWidth;
      } else {
        if (currentLineWidth > 0) {
          strncat(currentLine, " ", MAX_QUESTION_LINE_LENGTH - strlen(currentLine) - 1);
          currentLineWidth += SMALL_CHAR_WIDTH; // Add space width
        }
        strncat(currentLine, wordStart, wordLength);
        currentLineWidth += wordWidth;
      }

      wordStart = currentPos + 1;
    }
    currentPos++;
  }

  // Add the last line to the array if not empty
  if (strlen(currentLine) > 0 && lineCount < MAX_QUESTION_LINES) {
    strncpy(lines[lineCount], currentLine, MAX_QUESTION_LINE_LENGTH);
    lineCount++;
  }
}

// returns button pressed or false if no button press detected
// inaccurate, so used to create getDebouncedButton() which
// is more accurate
char getButtonPressed(){
  if(!digitalRead(BUTTON_A)){
    return 'A';
  }
  else if(!digitalRead(BUTTON_B)){
    return 'B';
  }
  else if(!digitalRead(BUTTON_C)){
    return 'C';
  }
  else if(!digitalRead(BUTTON_D)){
    return 'D';
  }
  else{
    return false;
  }
}

// get button pressed after debouncing it
char getDebouncedButton(){
  char reading;
  char buttonState;
  char lastButtonState = HIGH;
  int debounceDelay = 25;
  unsigned long lastDebounceTime = 0;

  // loop runs at least once, after that, if change in button
  // state (default is HIGH) is detected, loop runs as long as
  // time since debounce timer was started is less than
  // debounceDelay + 20 seconds
  do{
    // initial button state
    reading = getButtonPressed();
    if(reading != lastButtonState){
      // start timer if button state has changed
      lastDebounceTime = millis();
    }
    if((millis() - lastDebounceTime) > debounceDelay && reading != buttonState) {
      // change buttonState after waiting debounceDelay seconds
      // and if reading has changed so that reading != buttonState
      buttonState = reading;
    }
    // set lastButtonState to current reading
    lastButtonState = reading;
  }while(millis() - lastDebounceTime < debounceDelay + 20);

  // return the button pressed
  return buttonState;
}

// takes question pool and question array to store random questions in as arguments
void getRandomQuestions(Question questionPool[QUESTION_POOL_LENGTH], Question randomQuestions[QUIZ_LENGTH]){
  // used to check if any question from pool has already been added to randomQuestions
  // TODO: change below to QUESTION_POOL_LENGTH after adding more questions
  bool chosenIdxs[QUESTION_POOL_LENGTH] = {false};
  static int prevChosenIdxs[QUIZ_LENGTH];
  static int numQuiz = 0;
  int numQuestionsAdded = 0;
  int numEasyQuestionsAdded = 0;
  int numMediumQuestionsAdded = 0;
  int numHardQuestionsAdded = 0;
  bool easyFilled = false;
  bool mediumFilled = false;
  while(numQuestionsAdded < QUIZ_LENGTH){
    bool prevUsed = false;
    easyFilled = numEasyQuestionsAdded == NUM_EASY;
    mediumFilled = numMediumQuestionsAdded == NUM_MEDIUM;
    // TODO: change below to QUESTION_POOL_LENGTH after adding more questions
    int randomIdx = random(0, QUESTION_POOL_LENGTH);
    Serial.print(randomIdx);
    Serial.print(", ");
    if(numQuiz > 0){
      for(int i = 0; i < QUIZ_LENGTH; i++){
        prevUsed = (randomIdx == prevChosenIdxs[i]) || prevUsed;
      }
    }
    // if question has not been added to randomQuestions, add it
    if(!chosenIdxs[randomIdx] && !prevUsed){
      // randomQuestions[numQuestionsAdded] = questionPool[randomIdx];
      // chosenIdxs[randomIdx] = true;
      // numQuestionsAdded++;
      if(numEasyQuestionsAdded < NUM_EASY && questionPool[randomIdx].difficulty == "<Easy>"){
        randomQuestions[numQuestionsAdded] = questionPool[randomIdx];
        chosenIdxs[randomIdx] = true;
        prevChosenIdxs[numQuestionsAdded] = randomIdx;
        numEasyQuestionsAdded++;
        numQuestionsAdded++;
      }else if(easyFilled && numMediumQuestionsAdded < NUM_MEDIUM && questionPool[randomIdx].difficulty == "<Medium>"){
        randomQuestions[numQuestionsAdded] = questionPool[randomIdx];
        chosenIdxs[randomIdx] = true;
        prevChosenIdxs[numQuestionsAdded] = randomIdx;
        numMediumQuestionsAdded++;
        numQuestionsAdded++;
      }else if(mediumFilled && numHardQuestionsAdded < NUM_HARD && questionPool[randomIdx].difficulty == "<Hard>"){
        randomQuestions[numQuestionsAdded] = questionPool[randomIdx];
        chosenIdxs[randomIdx] = true;
        prevChosenIdxs[numQuestionsAdded] = randomIdx;
        numHardQuestionsAdded++;
        numQuestionsAdded++;
      }
      // Serial.println(randomQuestions[numQuestionsAdded].category);
    }
  }
  numQuiz++;
}

void IRAM_ATTR soundEffectsISR(void* arg){
  // Clear the interrupt status
  timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_0);
  
  // Enable the alarm for the next period
  timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_0);
  if(playSound){
    if (soundIdx < currentSoundLength){
      uint8_t sample = pgm_read_byte(&currentSoundBytes[soundIdx]);
      dac_output_voltage(DAC_CHANNEL_1, sample);
      soundIdx++;
    }
  }
  if(controlServo){
    if(!turnServo){
      currentMillis = millis();
      turnServo = true;
    }
    if(millis() - currentMillis < SERVO_INTERVAL){
      servo.write(90);
    }else{
      // servo is reset here instead of below so that servo does not have to wait for sound to finish playing
      servo.write(0);
    }
  }
  if(playSound && !controlServo && soundIdx >= currentSoundLength){
    soundIdx = 0;
    timer_pause(TIMER_GROUP_0, TIMER_0);
  }
  else if(!playSound && controlServo && millis() - currentMillis >= SERVO_INTERVAL){
    turnServo = false;
    servo.write(0);
    timer_pause(TIMER_GROUP_0, TIMER_0);
  }
  else if(playSound && controlServo && soundIdx >= currentSoundLength && millis() - currentMillis >= SERVO_INTERVAL){
    soundIdx = 0;
    turnServo = false;
    // servo is not reset here otherwise it needs to wait for sound to finish playing before resetting
    timer_pause(TIMER_GROUP_0, TIMER_0);
  }
}

void IRAM_ATTR songISR(void* arg){
    // Clear the interrupt status
    timer_group_clr_intr_status_in_isr(TIMER_GROUP_0, TIMER_1);
    
    // Enable the alarm for the next period
    timer_group_enable_alarm_in_isr(TIMER_GROUP_0, TIMER_1);
    if(onStartScreen){
      startSongIdx = startSongIdx == startSoundLength - 1 ? 38188 : startSongIdx + 1;
      uint8_t sample = pgm_read_byte(&startSoundBytes[startSongIdx]);
      dac_output_voltage(DAC_CHANNEL_1, sample);
    }else{
      questionSongIdx = (questionSongIdx + 1) % questionSoundLength;
      uint8_t sample = pgm_read_byte(&questionSoundBytes[questionSongIdx]);
      dac_output_voltage(DAC_CHANNEL_1, sample);
    }
}

void setupTimerDACServo(){
  dac_output_enable(DAC_CHANNEL_1);

  servo.attach(SERVO);
  servo.write(0);

  // Configure the timer
  timer_config_t soundEffectsConfig = {
    .alarm_en = TIMER_ALARM_EN,         // Enable the alarm
    .counter_en = TIMER_PAUSE,          // Start with the timer paused
    .intr_type = TIMER_INTR_LEVEL,      // Use level interrupt
    .counter_dir = TIMER_COUNT_UP,      // Count up
    .auto_reload = TIMER_AUTORELOAD_EN, // Auto-reload the counter
    .divider = 80                       // 1 tick = 1 microsecond (80 MHz / 80)
  };

  // Initialize the timer
  timer_init(TIMER_GROUP_0, TIMER_0, &soundEffectsConfig);
  
  // Set the counter value to 0
  timer_set_counter_value(TIMER_GROUP_0, TIMER_0, 0);
  
  // Set the alarm value for the sample rate
  timer_set_alarm_value(TIMER_GROUP_0, TIMER_0, SOUND_EFFECTS_INTERVAL);
  
  // Enable the timer interrupt
  timer_enable_intr(TIMER_GROUP_0, TIMER_0);
  
  // Attach the ISR to the timer
  timer_isr_register(TIMER_GROUP_0, TIMER_0, soundEffectsISR, NULL, ESP_INTR_FLAG_IRAM, NULL);
  
  // Do not start the timer here; start it based on the button press

  timer_config_t songConfig = {
    .alarm_en = TIMER_ALARM_EN,         // Enable the alarm
    .counter_en = TIMER_PAUSE,          // Start with the timer paused
    .intr_type = TIMER_INTR_LEVEL,      // Use level interrupt
    .counter_dir = TIMER_COUNT_UP,      // Count up
    .auto_reload = TIMER_AUTORELOAD_EN, // Auto-reload the counter
    .divider = 80                       // 1 tick = 1 microsecond (80 MHz / 80)
  };

  timer_init(TIMER_GROUP_0, TIMER_1, &songConfig);
  timer_set_counter_value(TIMER_GROUP_0, TIMER_1, 0);
  timer_set_alarm_value(TIMER_GROUP_0, TIMER_1, SONG_INTERVAL);
  timer_enable_intr(TIMER_GROUP_0, TIMER_1);
  timer_isr_register(TIMER_GROUP_0, TIMER_1, songISR, NULL, ESP_INTR_FLAG_IRAM, NULL);
}

void playSoundEffect(Screen currentScreen, int score, bool newQuiz){
  playSound = true;
  controlServo = false;
  if(currentScreen == START){
    onStartScreen = true;
    startSongIdx = newQuiz ? 0 : startSongIdx;
    timer_pause(TIMER_GROUP_0, TIMER_0);
    timer_start(TIMER_GROUP_0, TIMER_1);
  }else if(currentScreen == QUESTION){
    onStartScreen = false;
    questionSongIdx = newQuiz ? 0 : questionSongIdx;
    timer_pause(TIMER_GROUP_0, TIMER_0);
    timer_start(TIMER_GROUP_0, TIMER_1);
  }
  else if(currentScreen == CORRECT){
    currentSoundBytes = correctSoundBytes;
    currentSoundLength = correctSoundLength;
    timer_pause(TIMER_GROUP_0, TIMER_1);
    timer_start(TIMER_GROUP_0, TIMER_0);
  }else if(currentScreen == WRONG){
    currentSoundBytes = wrongSoundBytes;
    currentSoundLength = wrongSoundLength;
    timer_pause(TIMER_GROUP_0, TIMER_1);
    timer_start(TIMER_GROUP_0, TIMER_0);
  }else if(currentScreen == SCORE){
    controlServo = score >= 3;
    currentSoundBytes = score >= 3 ? victorySoundBytes : defeatSoundBytes;
    currentSoundLength = score >= 3 ? victorySoundLength : defeatSoundLength;
    timer_pause(TIMER_GROUP_0, TIMER_1);
    timer_start(TIMER_GROUP_0, TIMER_0);
  }
}