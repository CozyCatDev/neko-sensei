#include <Arduino.h>

#include "sound_effects.h"

#define DAC 25
#define SAMPLE_RATE 8000

void setup() {
  Serial.begin(115200);
}

void loop() {
  playSound();
  delay(10000);
}

void playSound() {
  for (int i = 0; i < wrongSoundLength; i++) {
    int sample = pgm_read_byte_near(wrongSoundBytes + i);
    dacWrite(DAC, sample);
    delayMicroseconds(1000000 / SAMPLE_RATE);
  }
}
