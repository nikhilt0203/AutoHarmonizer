#pragma once
#include <Arduino.h>
#include "Note.h"

//Base class for harmonizers
class Harmonizer {
protected:
  int midiChannel;                            //MIDI channel to send chords to
  bool active;                                //if harmonizer is currently on
  int currentChord[4];                        //indices 0-2 MIDI note #s, index 3 scale degree of chord
  virtual void generateChord(Note note) = 0;  //for generating a new chord
public:
  Harmonizer(int midiChannel);              //constructor
  virtual void on();                        //turn on harmonizer
  virtual void off();                       //turn off harmonizer
  virtual bool isOn();                      //if harmonizer is currently on
  virtual void setChannel(int newChannel);  //set MIDI channel of harmonizer
  virtual int* getCurrentChord();           //return currentChord[]
  virtual void chordOn(Note note) = 0;      //send a chord via MIDI on based on a given Note
  virtual void chordOff() = 0;              //turn off current chor