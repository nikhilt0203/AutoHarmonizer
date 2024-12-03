#pragma once
#include "DiatonicHarmonizer.h"
#include "AtonalHarmonizer.h"

extern int midiNotes[16];

extern int currentKey;  //initialized to harmonizer.getKey()
extern int currentHarmonizer;
extern int litUpKey;
extern bool isRecording;
extern bool isPlaying;
extern bool allowButtonInput;

extern DiatonicHarmonizer diatonicHarmonizer;
extern AtonalHarmonizer atonalHarmonizer;
extern Harmonizer* harmonizers[2];
extern const int DIATONIC, ATONAL;

void recorderSetup();

void recordToggle();

void recordNoteOn(int keyNum);

void recordNoteOff(int keyNum);

void playMelody();

bool playbackInterrupt();

void clearMelody();

void changeKeyboardRange(int shift);

int getShift();