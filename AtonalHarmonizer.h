#pragma once
#include <Arduino.h>
#include "Harmonizer.h"
#include "Note.h"

class AtonalHarmonizer : public Harmonizer {
private:
  int previousChord[4] = { -1 };
  int chordLength;
  void generateChord(Note note) override;
  bool verifyNextNote(int newNote);
public:
  AtonalHarmonizer(int midiChannel);
  void chordOn(Note note) override;
  void chordOff() override;
  String chordToString() override;
};