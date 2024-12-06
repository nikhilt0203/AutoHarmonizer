#pragma once
#include <Arduino.h>
#include <vector>
#include "Note.h"

class Harmonizer {
protected:
  int midiChannel;                            //MIDI channel to send chords to
  bool isActive = false;                      //if harmonizer is currently on
  int currentChord[4];                        //the current chord of the harmonizer, represented with MIDI note #s
  virtual void generateChord(Note note) = 0;  //populates currentChord[] with MIDI note #s based on a given Note

public:
  Harmonizer(int midiChannel);
  virtual void on();                            //puts harmonizer in active mode
  virtual void off();                           //puts harmonizer in inactive mode
  virtual bool isOn();                          //returns whether or not harmonizer is active
  virtual void setChannel(int newChannel);      //changes the MIDI channel
  virtual int* getCurrentChord();               //returns the currentChord array
  virtual void chordOn(Note note) = 0;          //sends currentChord notes via MIDI
  virtual void chordOff() = 0;                  //turns off currentChord notes via MIDI
  virtual String chordToString() = 0;           //returns a string representation of the current chord
};