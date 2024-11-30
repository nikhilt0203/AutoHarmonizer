#pragma once

struct Note {
  Note* next = nullptr;   //pointer to the next note in the melody
  int note;               //MIDI note 0-127
  unsigned long timeOn;   //the time the note was triggered, in ms
  unsigned long timeOff;  //the time the note was released, in ms
};