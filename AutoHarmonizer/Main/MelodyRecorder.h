#pragma once
#include "DiatonicHarmonizer.h"
#include "AtonalHarmonizer.h"

extern const int DIATONIC, ATONAL;
extern int currentHarmonizer;
extern bool allowButtonInput;

extern DiatonicHarmonizer diatonicHarmonizer;
extern AtonalHarmonizer atonalHarmonizer;
extern Harmonizer* harmonizers[2];

enum class RecorderState {
  Off, Recording, Playing
};

/**
 * @brief setup function for the melody recorder.
 */
void melodyRecorderSetup();

/**
 * @brief toggles Recording mode on/off.
 */
void recordToggle();

/**
 * @brief records a MIDI note and the time it was played into the melody.
 */
void recordNoteOn(int midiNote);

/**
 * @brief records a MIDI note and the time it was released into the melody.
 */
void recordNoteOff(int midiNote);

/**
 * @brief plays the recorded melody, and harmony if the harmonizer is on, via MIDI.
 */
void playMelody();

/**
 * @brief clears the currently recorded melody.
 */
void clearMelody();

/**
 * @brief sets the MIDI out channel of the melody recorder.
 */
void setMelodyMidiChannel(int channel);

/**
 * @brief retrieves the current state of the melody recorder.
 * @return the current RecorderState
 */
RecorderState getRecorderState();