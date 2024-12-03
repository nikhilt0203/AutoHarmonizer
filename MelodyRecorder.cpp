#include "MelodyRecorder.h"
#include "Note.h"
#include "ButtonManager.h"
#include "MenuManager.h"
#include "DisplayManager.h"
#include "LEDManager.h"

const int DIATONIC = 0, ATONAL = 1;
const int MAX_LENGTH = 75;

int noteCount = 0;
int currentKey = -1;
int currentHarmonizer = 0;
int litUpKey = -1;

bool isRecording = false;
bool isPlaying = false;
bool allowButtonInput = true;
bool maxReached = false;

unsigned long playbackStartTime;

Note melody[MAX_LENGTH];

int midiNotes[16 ]= {
  60, 61, 62, 63,
  64, 65, 66, 67,
  68, 69, 70, 71,
  72, 73, 74, 75
};

DiatonicHarmonizer diatonicHarmonizer = DiatonicHarmonizer(0, false, 2);
AtonalHarmonizer atonalHarmonizer = AtonalHarmonizer(2);
Harmonizer* harmonizers[2] = { &diatonicHarmonizer, &atonalHarmonizer };

void recorderSetup() {
  harmonizers[DIATONIC]->on();
  harmonizers[ATONAL]->on();
}

void recordToggle() {
  isRecording = !isRecording;
  if (isRecording) clearMelody();
  allowButtonInput = true;
}

void recordNoteOn(int keyNum) {
  int note = midiNotes[keyNum];
  litUpKey = keyNum;
  usbMIDI.sendNoteOn(note, 127, 1);  //monitoring note
  if (!isRecording) return;          //don't proceed if not in record mode

  //Save MIDI note # and the time at which it was triggered
  if (noteCount < MAX_LENGTH) {
    melody[noteCount].note = note;
    melody[noteCount].timeOn = millis();
  }
}

void recordNoteOff(int keyNum) {
  int note = midiNotes[keyNum];
  usbMIDI.sendNoteOff(note, 0, 1);  //monitoring note
  litUpKey = -1;
  if (!isRecording) {  //don't proceed if not in record mode
    allowButtonInput = true;
    return;
  }

  //Save the time at which the note was released
  if (noteCount < MAX_LENGTH && !allowButtonInput) {
    melody[noteCount].timeOff = millis();

    //Set the "next" pointer of the previous note to the current note
    if (noteCount > 0) melody[noteCount - 1].next = &melody[noteCount];
    noteCount++;
  }
  //Stop recording if max melody length is reached
  if (noteCount == MAX_LENGTH && !maxReached) {
    maxReached = true;
    recordToggle();
  }
  allowButtonInput = true;
}

void playMelody() {
  //Return if there are no notes to play
  if (noteCount == 0) {
    allowButtonInput = true;
    return;
  }

  if (isRecording) recordToggle();
  isPlaying = true;
  bool playStop = false;
  playbackStartTime = millis();

  unsigned long noteStartTime, noteEndTime, length;
  int chordFrequency;
  Harmonizer* harmonizer = harmonizers[currentHarmonizer];
  int i = 0;

  while (i < noteCount) {
    if (playStop) break;

    Note& curNote = melody[i];
    noteStartTime = playbackStartTime + (curNote.timeOn - melody[0].timeOn);
    noteEndTime = playbackStartTime + (curNote.timeOff - melody[0].timeOn);
    length = noteEndTime - noteStartTime;

    //Don't proceed until it is time to start the note
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
       //send chord every 2 notes if note length <250ms, every 4 notes if <100ms
      if(length < 250 && i != noteCount - 1) {
        chordFrequency = length < 100 ? 4 : 2;
        if (i % chordFrequency == 0) harmonizer->chordOn(curNote);
      //otherwise send chord for every note
      } else {
        harmonizer->chordOn(curNote);
      }
      //display chord on screen with size 4
      displayChord(harmonizer->chordToString());
    }

    //Don't proceed for the duration of the note
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

bool playbackInterrupt() {
  //Check if playback should be stopped
  //digitalRead play button directly to bypass allowButtonInput,
  //return true if 500ms elapsed between first and second press (stop playback)
  if (isPlaying && digitalRead(30) && millis() - playbackStartTime > 500) return true;
  return false;
}

void clearMelody() {
  noteCount = 0;
}

void changeKeyboardRange(int newRangeShift) {
  if (!allowButtonInput) return;  //prevent simultaneous input

  static int previousRangeShift = 0;
  int shift = newRangeShift - previousRangeShift;
  previousRangeShift = newRangeShift;
  //verify lowest and highest notes are in range
  int upper = midiNotes[15] + shift;
  int lower = midiNotes[0] + shift;
  if (upper <= 127 && lower >= 0) {
    for (int i = 0; i < 16; i++) {
      midiNotes[i] += shift;
    }
  }
}

int getShift() {
  return midiNotes[0] - 60;
}