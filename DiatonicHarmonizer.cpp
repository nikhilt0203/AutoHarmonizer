#include "DiatonicHarmonizer.h"

DiatonicHarmonizer::DiatonicHarmonizer(int key, bool minor, int midiChannel)
  : Harmonizer(midiChannel), key(key), minor(minor) {}

void DiatonicHarmonizer::generateChord(Note note) {
  int midiNote = note.note;
  int chord = possibleChords[findScaleDegree(midiNote) - 1][selectChord(note)];
  currentChord[3] = chord;  //store scale degree of the chord
  int octave;
  for (int i = 0; i < 3; i++) {
    //voicing of the triad
    if (i == 0) octave = 48;
    if (i == 1) octave = 60;
    if (i == 2) octave = 60;

    currentChord[i] = scaleDegreeToSemitones(scaleDegreesInChords[chord - 1][i]) + key + octave + range * 12;
  }
}

int DiatonicHarmonizer::selectChord(Note note) {
  int index = findScaleDegree(note.note) - 1;

  /* Bypass typical chord weights if it is the last note AND its scale degree is 1 or 3 
        (5 not included to allow for half cadence)
    */
  if (note.next == nullptr && (index == 0 || index == 2)) {
    switch (random(0, 10)) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7: return 0;  //80% I chord
      case 8:
      case 9: return 2;  //20% vi chord
    }
  }

  //Find total weight of the three potential chords
  int totalWeight = 0;
  for (int i = 0; i < 3; i++) {
    totalWeight += chordWeights[index][i];
  }

  //Randomly choose chord (index of chord array) based on weights
  int randWeight = random(0, totalWeight + 1);
  int sum = 0;
  int selectedChordIndex = 0;
  for (int i = 0; i < 3; i++) {
    sum += chordWeights[index][i];
    if (randWeight < sum) {
      selectedChordIndex = i;
      break;
    }
  }
  return selectedChordIndex;
}

//Converts a scale degree into the # of semitones above the tonic (0)
int DiatonicHarmonizer::scaleDegreeToSemitones(int scaleDegree) {
  switch (scaleDegree) {
    case 1: return 0;              //do
    case 2: return 2;              //re
    case 3: return minor ? 3 : 4;  //mi or me
    case 4: return 5;              //fa
    case 5: return 7;              //sol
    case 6: return minor ? 8 : 9;  //la or le
    case 7: return 11;             //ti
  }
  return 0;
}

int DiatonicHarmonizer::findScaleDegree(int midiNote) {
  //map MIDI note 0-127 to chromatic scale 0-11
  int note = midiNote % 12;
  //find distance between the note and the key
  int interval = (note - key + 12) % 12;

  switch (interval) {
    case 0: return 1;
    case 2: return 2;
    case 3: return 3;  //return scale degree 3 for both flat 3...
    case 4: return 3;  //...and 3
    case 5: return 4;
    case 7: return 5;
    case 8: return 6;  //return scale degree 6 for both flat 6...
    case 9: return 6;  //...and 6
    case 11: return 7;
    default: return 0;
  }
}

void DiatonicHarmonizer::chordOn(Note note) {
  if (!active) return;

  generateChord(note);
  for (int i = 0; i < 3; i++) {
    delay(10);
    usbMIDI.sendNoteOn(currentChord[i], 127, midiChannel);
  }
}

void DiatonicHarmonizer::chordOff() {
  if (!active) return;

  for (int i = 0; i < 3; i++) {
    usbMIDI.sendNoteOff(currentChord[i], 0, midiChannel);
  }
}

void DiatonicHarmonizer::toMinor() {
  minor = true;
}

void DiatonicHarmonizer::toMajor() {
  minor = false;
}

bool DiatonicHarmonizer::isMinor() {
  return minor;
}

int DiatonicHarmonizer::getKey() {
  return key;
}

void DiatonicHarmonizer::setKey(int newKey) {
  if (newKey >= 0 && newKey <= 11) {
    key = newKey;
  }
}

void DiatonicHarmonizer::setOctave(int newOctave) {
  if (newOctave >= -4 && newOctave <= 4) range = newOctave;
}

String DiatonicHarmonizer::chordToString() {
  const char* sharpNoteNames[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
  const char* flatNoteNames[12] = { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };

  int interval = (currentChord[0] - key + 12) % 12;  //num half steps above key
  String chord, quality;

  //MAJOR KEYS
  if (!minor) {
    //flat keys (Eb, F, Ab, Bb)
    if (key == 3 || key == 5 || key == 8 || key == 10) {
      chord = flatNoteNames[(key + interval) % 12];
      //sharp keys ((C), C#, D, E, F#, G, A, B)
    } else {
      chord = sharpNoteNames[(key + interval) % 12];
    }
  }

  //MINOR KEYS
  //if raised 7th needs to be handled (keys C#m, Ebm, F#m, Gm)
  if (minor && (key == 1 || key == 2 || key == 6 || key == 7) && interval == 11) {
    switch (key) {
      case 1: chord = "Bx"; break;  //C#m
      case 2: chord = "C#"; break;  //Ebm
      case 6: chord = "Ex"; break;  //F#m
      case 7: chord = "F#"; break;  //Gm
    }
    //all other minor keys
  } else if (minor) {
    //sharp keys (C#m, Em, F#m, (Am), Bm)
    if (key == 1 || key == 4 || key == 6 || key == 9 || key == 11) {
      chord = sharpNoteNames[(key + interval) % 12];
      //flat keys (Cm, Dm, Ebm, Fm, Gm, Abm, Bbm)
    } else {
      chord = flatNoteNames[(key + interval) % 12];
    }
  }

  //get quality of chord from scale degree + mode
  switch (currentChord[3]) {
    case 1:
      quality = minor ? "m" : "";
      break;
    case 2:
      quality = minor ? "dim" : "m";
      break;
    case 3:
      quality = minor ? "+" : "m";
      break;
    case 4:
      quality = minor ? "m" : "";
      break;
    case 5:
      quality = "";
      break;
    case 6:
      quality = minor ? "" : "m";
      break;
    case 7:
      quality = "dim";
      break;
  }
  return chord + quality;
}