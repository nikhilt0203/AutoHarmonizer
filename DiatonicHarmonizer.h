#pragma once
#include "Harmonizer.h"
#include "Note.h"

class DiatonicHarmonizer : public Harmonizer {
private:
  int key;     //current key, 0 - 11, C as 0
  bool minor;  //if harmonizer is in minor mode
  int range;   //adjusts the octave that the chords will be played in

  /* possible chord for each scale degree. first array contains all triads that contain DO, 
    second array contains all triads that contain RE..
  */
  int possibleChords[7][3] = {
    { 1, 4, 6 },
    { 2, 5, 7 },
    { 1, 3, 6 },
    { 2, 4, 7 },
    { 1, 3, 5 },
    { 2, 4, 6 },
    { 3, 5, 7 }
  };

  //probabilities for each chord for each scale degree, corresponds with possibleChords[] above
  int chordWeights[7][3] = {
    { 10, 8, 8 },
    { 7, 10, 1 },
    { 10, 1, 8 },
    { 6, 10, 1 },
    { 7, 1, 10 },
    { 6, 8, 10 },
    { 1, 10, 2 }
  };

  //scale degrees contained in the 7 diatonic triads
  int scaleDegreesInChords[7][3] = {
    { 1, 3, 5 },
    { 2, 4, 6 },
    { 3, 5, 7 },
    { 4, 6, 1 },
    { 5, 7, 2 },
    { 6, 1, 3 },
    { 7, 2, 4 },
  };

  void generateChord(Note note) override;       //populates currentChord[] with MIDI note #s based on a given Note
  int selectChord(Note note);                   //returns the index of the selected chord in possibleChords[]
  int scaleDegreeToSemitones(int scaleDegree);  //converts scale degree into # of semitones above the tonic
  int findScaleDegree(int midiNote);            //finds the scale degree of a given MIDI note based on the key
  
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
};