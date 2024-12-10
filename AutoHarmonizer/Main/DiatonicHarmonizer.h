#pragma once
#include "Harmonizer.h"
#include "Note.h"

class DiatonicHarmonizer : public Harmonizer {
public:
  DiatonicHarmonizer(int key, bool minor, int midiChannel);
  void chordOn(Note note) override;  //sends currentChord notes via MIDI
  void chordOff() override;          //turns off currentChord notes via MIDI
  void toMinor();                    //sets the harmonizer to Minor mode
  void toMajor();                    //sets the harmonizer to Major mode
  bool isMinor();                    //if the harmonizer is in Minor mode
  int getKey();                      //returns the current key
  void setKey(int newKey);           //sets the key of the harmonizer, 0 <= newKey <= 11
  void setOctave(int newOctave);     //sets the octave of the harmonizer, -4 <= newOctave <= 4
  String chordToString() override;   //returns a string representation of the current chord

private:
  int key;                                      //current key, 0 - 11, C as 0
  bool minor;                                   //if harmonizer is in minor mode
  int range;                                    //adjusts the octave that the chords will be played in
  void generateChord(Note note) override;       //populates currentChord[] with MIDI note #s based on a given Note
  int selectChord(Note note);                   //returns the index of the selected chord in possibleChords[]
  int scaleDegreeToSemitones(int scaleDegree);  //converts scale degree into # of semitones above the tonic
  int findScaleDegree(int midiNote);            //finds the scale degree of a given MIDI note based on the key
};