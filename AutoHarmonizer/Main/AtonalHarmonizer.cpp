#include "AtonalHarmonizer.h"

AtonalHarmonizer::AtonalHarmonizer(int midiChannel)
  : Harmonizer(midiChannel) {}

void AtonalHarmonizer::chordOn(Note note) {
  if (!isActive) return;
  generateChord(note);
  for (int i = 0; i < 4; i++) {
    delay(10);
    if (i > 0 && currentChord[i - 1] == currentChord[i]) continue;  //don't play duplicate notes
    usbMIDI.sendNoteOn(currentChord[i], 127, midiChannel);
  }
}

void AtonalHarmonizer::chordOff() {
  if (!isActive) return;
  for (int i = 0; i < 4; i++) {
    usbMIDI.sendNoteOff(currentChord[i], 0, midiChannel);
  }
}

String AtonalHarmonizer::chordToString() {
  //so that notes print in ascending order
  std::sort(currentChord, currentChord + 4);

  const char* noteNames[12] = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" };
  String chord;
  for (int i = 0; i < 4; i++) {
    if (i > 0 && currentChord[i - 1] == currentChord[i]) continue;
    chord += noteNames[currentChord[i] % 12];
    chord += " ";
  }
  return chord;
}

void AtonalHarmonizer::generateChord(Note note) {
  currentChord[0] = note.note;
  chordLength = 1;
  while (chordLength < 4) {
    int nextNote = random(48, 85);
    while (!verifyNextNote(nextNote)) {
      nextNote = random(48, 85);
    }
    currentChord[chordLength] = nextNote;
    chordLength++;
  }

  /* sort only last 3 elements. User melody is contained in first index,
        so if user leaps by >7 semitones then chord will re-generate infinitely
    */
  std::sort(currentChord + 1, currentChord + 4);
  std::sort(previousChord + 1, previousChord + 4);

  //re-generate chord if min2 in bass
  if (abs(currentChord[0] - currentChord[1]) == 1) {
    generateChord(note);
    return;
  }

  //Skip if previousChord hasn't been initialized yet
  if (previousChord[0] != -1) {
    //re-generate chord if a voice leaps by more than 5 semitones
    for (int i = 1; i < 4; i++) {
      if (abs(currentChord[i] - previousChord[i]) > 7) {
        generateChord(note);
        return;
      }
    }
  }

  //update previousChord
  for (int i = 0; i < 4; i++) {
    previousChord[i] = currentChord[i];
  }
}

bool AtonalHarmonizer::verifyNextNote(int newNote) {
  int minSeconds = 0, majSeconds = 0, majSevenths = 0, tritones = 0;
  
  for (int i = 0; i < chordLength; i++) {
    int interval = abs(currentChord[i] - newNote);
    if (interval == 13) return false;
    if (interval == 11) majSevenths++;
    if (interval == 6) tritones++;
    if (interval == 2) majSeconds++;
    if (interval == 1) minSeconds++;
  }
  /*conditions: 
      - no minor 9ths
      - max 1 minor 2nds
      - max 1 tritone
      - max 2 major 2nds
      - max 1 major 7th
    */
  if (minSeconds > 1) return false;
  if (majSeconds > 2) return false;
  if (tritones > 1) return false;
  if (majSevenths > 1) return false;
  return true;
}