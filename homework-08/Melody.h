#pragma once
#ifndef MELODY_H
#define MELODY_H

#include "pitches.h"

int melodyNBC[] = {
  NOTE_E5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5, NOTE_A4
};

int durationsNBC[] = {
  750, 750, 50, 50, 50, 50, 50, 50, 750, 750
};

unsigned long previousNoteTime = 0;
byte currentNote = 0;

/*
  Given the pin to which the buzzer is connected, a melody and
  its notes durations, use the tone function to reproduce 
  the melody, with a small delay between notes
*/
void playMelody(const byte buzzerPin, const int melody[], const int durations[]){
  unsigned long currentNoteTime = millis();

  if ((currentNoteTime - previousNoteTime) >= durations[currentNote]) {
    previousNoteTime = currentNoteTime;
    tone(buzzerPin, melody[currentNote], durations[currentNote]);
    currentNote += 1;
  
    if (currentNote >= sizeof(melody) / sizeof(melody[0])) {
      currentNote = 0;
    }
  }
};

#endif