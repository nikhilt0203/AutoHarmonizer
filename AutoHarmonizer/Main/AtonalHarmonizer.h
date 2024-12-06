#pragma once
#include "Harmonizer.h"
#include "Note.h"

class AtonalHarmonizer : public Harmonizer {
public:
  AtonalHarmonizer(int midiChannel);
  void chordOn(Note note) override;
  void chordOff() override;
  String chordToString() override;

private:
  int previousChord[4] = { -1 };
  int chordLength;
  void generateChord(Note note) override;
  bool verifyNextNote(int newNote);
};