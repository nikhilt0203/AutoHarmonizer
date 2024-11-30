#pragma once
#include <Arduino.h>
#include "Harmonizer.h"
#include "Note.h"

class DiatonicHarmonizer : public Harmonizer {
private:
  int key;     //0 - 11, C major as 0
  bool minor;  //if harmonizer is in minor mode
  int range;   //octave of the harmonizer's chords

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

  //probabilities for each chord for each scale degree, corresponds with possibleChords array above
  int chordWeights[7][3] = {
    { 10, 8, 8 },
    { 7, 10, 1 },
    { 14, 1, 10 },
    { 6, 10, 1 },
    { 10, 1, 14 },
    { 6, 8, 10 },
    { 1, 12, 4 }
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

  void generateChord(Note note) override;
  int selectChord(Note note);
  int scaleDegreeToSemitones(int scaleDegree);
  int findScaleDegree(int midiNote);
public:
  DiatonicHarmonizer(int key, bool minor, int midiChannel);
  void chordOn(Note note) override;
  void chordOff() override;
  void setKey(int newKey);
  void setOctave(int newOctave);
  void toMinor();
  void toMajor();
  bool isMinor();
  int getKey();
  String chordToString() override;
};