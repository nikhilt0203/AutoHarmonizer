#include "MelodyRecorder.h"
#include "Note.h"
#include "ButtonManager.h"
#include "MenuManager.h"
#include "DisplayManager.h"
#include "LEDManager.h"

const int DIATONIC = 0, ATONAL = 1;
const int MAX_LENGTH = 100;

int noteCount = 0;
int currentKey = -1;
int currentHarmonizer = 0;

bool isRecording = false;
bool isPlaying = false;
bool allowButtonInput = true;
bool maxReached = false;

unsigned long playbackStartTime;

//Initialize harmonizers
DiatonicHarmonizer diatonicHarmonizer = DiatonicHarmonizer(0, false, 2);
AtonalHarmonizer atonalHarmonizer = AtonalHarmonizer(2);
Harmonizer* harmonizers[2] = { &diatonicHarmonizer, &atonalHarmonizer };

//Array containing the current melody
Note melody[MAX_LENGTH];

void melodyRecorderSetup() {
  harmonizers[DIATONIC]->on();
  harmonizers[ATONAL]->on();
}

void recordToggle() {
  isRecording = !isRecording;
  if (isRecording) clearMelody();
  allowButtonInput = true;
}

void recordNoteOn(int midiNote) {
  usbMIDI.sendNoteOn(midiNote, 127, 1);  //monitoring note
  if (!isRecording) return;

  if (noteCount < MAX_LENGTH) {
    melody[noteCount].note = midiNote;
    melody[noteCount].timeOn = millis();
  }
}

void recordNoteOff(int midiNote) {
  usbMIDI.sendNoteOff(midiNote, 0, 1);  //monitoring note
  litUpKey = -1;

  if (!isRecording) {
    allowButtonInput = true;
    return;
  }

  if (noteCount < MAX_LENGTH && !allowButtonInput) {
    melody[noteCount].timeOff = millis();
    //Set "next" pointer of the previous note to the current note
    if (noteCount > 0) {
      melody[noteCount - 1].next = &melody[noteCount];
    }
    melody[noteCount].next = nullptr;
    noteCount++;
  }

  if (noteCount == MAX_LENGTH && !maxReached) {
    maxReached = true;
    recordToggle();
  }

  allowButtonInput = true;
}

bool playbackInterrupt() {
  //digitalRead play button directly to bypass allowButtonInput,
  //return true if 500ms elapsed between first and second press (stop playback)
  if (isPlaying && digitalRead(playButtonPin) && millis() - playbackStartTime > 500) {
    return true;
  }
  return false;
}

void playMelody() {
  if (noteCount == 0) {
    allowButtonInput = true;
    return;
  }

  if (isRecording) recordToggle();

  isPlaying = true;
  bool playStop = false;
  playbackStartTime = millis();

  Harmonizer* harmonizer = harmonizers[currentHarmonizer];
  unsigned long noteStartTime, noteEndTime, length;
  int chordFrequency;
  int i = 0;

  while (i < noteCount) {
    if (playStop) break;

    Note& curNote = melody[i];
    noteStartTime = playbackStartTime + (curNote.timeOn - melody[0].timeOn);
    noteEndTime = playbackStartTime + (curNote.timeOff - melody[0].timeOn);
    length = noteEndTime - noteStartTime;

    while (millis() < noteStartTime) {
      //check if playback should be stopped during pre-note delay
      playStop = playbackInterrupt();
      if (playStop) {
        harmonizer->chordOff();
        break;
      }
    }

    harmonizer->chordOff();  //placed here so that the previous chord will sustain into the next
    usbMIDI.sendNoteOn(curNote.note, 127, 1);
    if(!harmonizer->isOn()) displayNoteLeds(curNote, true);

    if (harmonizer->isOn()) {
       //send chord every 2 notes if note length is <250ms, every 4 notes if <100ms
      if(length < 250 && i != noteCount - 1) {
        chordFrequency = length < 100 ? 4 : 2;
        if (i % chordFrequency == 0) harmonizer->chordOn(curNote);
      } else {
        harmonizer->chordOn(curNote);
      }
      displayChord(harmonizer->chordToString());
    }

    while (millis() < noteEndTime) {
      //check if playback should be stopped during sustain
      playStop = playbackInterrupt();
      if (playStop) {
        harmonizer->chordOff();
        break;
      }
    }

    usbMIDI.sendNoteOff(curNote.note, 0, 1);
    if(!harmonizer->isOn()) displayNoteLeds(curNote, false);

    //To prevent delay after last note
    if (i == noteCount - 1) {
      harmonizer->chordOff();
      break;
    }
    //proceed to next note in melody[]
    i++;
  }

  isPlaying = false;
  allowButtonInput = true;
  updateCurrentMenu();
}

void clearMelody() {
  noteCount = 0;
}