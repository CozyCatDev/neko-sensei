# About
This repo contains code for a quiz box that displays E&E related questions presented in an MCQ format. This project was created for the [CMSB IEEE Stem Playground](). To progress in the quiz, users can press any of the four buttons (representing A, B, C and D).

Different animations will be displayed on screen depending on whether the questions were answered correctly or not. A prize (candy) will be dished out using a servo-controlled arm for each question answered correctly.

# Equipment
- ESP32 dev kit
- SSD1309 2.42" OLED display
- 8&Omega;, 0.5W speaker
- LM386 audio amplifier module
- Micro-servo
- ...

# Todo
### Display
- [x] Show question number, category and difficulty.
- [x] Show 5 E&E-related questions per quiz attempt where each question is of varying difficulty and category.
- [x] Show different animations when answering correctly/incorrectly.
- [x] Show final score at the end of each quiz.
- [x] Restart a new quiz after each quiz.
- [ ] Each new quiz chooses 5 questions randomly from the question pool.
- [ ] Show victory/defeat animation at end of quiz depending on score.

### Speaker
- [ ] Test speaker w/o LM386 audio amplifier, sound might be loud enough w/o amplifier.
- [x] Play different sound files when answering correctly/incorrectly.
- [x] Play victory/defeat sound effect at end of quiz depending on score.

### Servo
- [x] Control servo to give out prizes if score meets threshold value.

### 3D Model
- [ ] ...

# Concept