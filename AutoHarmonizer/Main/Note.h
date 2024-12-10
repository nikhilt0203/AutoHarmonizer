#pragma once

/**
 * @brief represents a single note of the melody.
 * despite being stored in an array, Notes have a next pointer 
 * to allow the harmonizer to generate chords based on a single Note
 * while being able to see what the next note is.
 */
struct Note {
  Note* next = nullptr;   //pointer to the next note in the melody
  int note;               //MIDI note 0-127
  unsigned long timeOn;   //the time the note was triggered, in ms
  unsigned long timeOff;  //the time the note was released, in ms
};