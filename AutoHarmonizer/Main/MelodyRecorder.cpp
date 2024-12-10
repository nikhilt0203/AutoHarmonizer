#include "MelodyRecorder.h"
#include "Note.h"
#include "ButtonManager.h"
#include "MenuManager.h"
#include "DisplayManager.h"
#include "LEDManager.h"

const int DIATONIC = 0, ATONAL = 1;
const int MAX_LENGTH = 100;  //default max

int noteCount = 0;
int currentHarmonizer = DIATONIC;
int midiChannel = 1;

bool allowButtonInput = true;
bool maxLengthReached = false;

unsigned long playbackStartTime;

//Current state of the melody recorder
RecorderState state;

//Initialize harmonizers
DiatonicHarmonizer diatonicHarmonizer = DiatonicHarmonizer(0, false, 2);
AtonalHarmonizer atonalHarmonizer = AtonalHarmonizer(2);
Harmonizer* harmonizers[2] = { &diatonicHarmonizer, &atonalHarmonizer };

//Array of Notes representing the current melody
Note melody[MAX_LENGTH];

void melodyRecorderSetup() {
  harmonizers[DIATONIC]->on();
  harmonizers[ATONAL]->on();
  state = RecorderState::Off;
}

void updateState(RecorderState newState) {
  state = newState;
}

void recordToggle() 
{
  if (state == RecorderState::Off) 
  {
    updateState(RecorderState::Recording);
    clearMelody();
    clearDisplay();
    displayText("      Recording...", 3, false);
  } 
  else if (state == RecorderState::Recording) 
  {
    updateState(RecorderState::Off);
    updateMenuDisplay();
  }
  allowButtonInput = true;
}

void recordNoteOn(int midiNote) 
{
  usbMIDI.sendNoteOn(midiNote, 127, midiChannel);  //note for monitoring
  if (state != RecorderState::Recording) return;

  if (noteCount < MAX_LENGTH) 
  {
    melody[noteCount].note = midiNote;
    melody[noteCount].timeOn = millis();
  }
}

void recordNoteOff(int midiNote) 
{
  usbMIDI.sendNoteOff(midiNote, 0, midiChannel);  //note for monitoring

  if (state != RecorderState::Recording) 
  {
    allowButtonInput = true;
    return;
  }

  if (noteCount < MAX_LENGTH && !allowButtonInput) 
  {
    melody[noteCount].timeOff = millis();
    //Set "next" pointer of the previous note to the current note
    if (noteCount > 0) 
    {
      melody[noteCount - 1].next = &melody[noteCount];
    }
    melody[noteCount].next = nullptr;
    noteCount++;
  }

  if (noteCount == MAX_LENGTH && !maxLengthReached) 
  {
    maxLengthReached = true;
    recordToggle();
  }

  allowButtonInput = true;
}

bool checkForPlaybackStop() 
{
  //digitalRead play button directly to bypass allowButtonInput,
  //return true if 500ms elapsed between first and second press (stop playback)
  if (state == RecorderState::Playing && digitalRead(playButtonPin) && millis() - playbackStartTime > 500)
    return true;

  return false;
}

void playMelody() 
{
  if (noteCount == 0) 
  {
    allowButtonInput = true;
    return;
  }

  updateState(RecorderState::Playing);
  updateLeds();

  playbackStartTime = millis();
  unsigned long noteStartTime, noteEndTime, noteLength;

  Harmonizer* harmonizer = harmonizers[currentHarmonizer];
  int index = 0;
  bool stopPlayback = false;

  while (index < noteCount) 
  {
    if (stopPlayback) break;
    
    Note& curNote = melody[index];
    noteStartTime = playbackStartTime + (curNote.timeOn - melody[0].timeOn);
    noteEndTime = playbackStartTime + (curNote.timeOff - melody[0].timeOn);
    noteLength = noteEndTime - noteStartTime;

    //DELAY BEFORE NOTE
    while (millis() < noteStartTime) 
    {
      //check if playback should be stopped during pre-note delay
      stopPlayback = checkForPlaybackStop();
      if (stopPlayback) 
      {
        harmonizer->chordOff();
        break;
      }
    }

    //PLAY NOTE
    harmonizer->chordOff();  //chordOff placed here so that the previous chord will sustain into the next
    usbMIDI.sendNoteOn(curNote.note, 127, midiChannel);
    if (!harmonizer->isOn()) displayNoteLeds(curNote, true);

    if (harmonizer->isOn()) 
    {
      //send chord every 2 notes if note length is <250ms, every 4 notes if <100ms
      if (noteLength < 400 && index != noteCount - 1) 
      {
        int chordFrequency = noteLength < 200 ? 4 : 2;
        if (index % chordFrequency == 0)
          harmonizer->chordOn(curNote);
      } 
      else 
      {
        //otherwise send chord for every note
        harmonizer->chordOn(curNote);
      }

      displayChord(harmonizer->chordToString());
    }

    //SUSTAIN NOTE
    while (millis() < noteEndTime) 
    {
      //check if playback should be stopped during sustain
      stopPlayback = checkForPlaybackStop();
      if (stopPlayback) 
      {
        harmonizer->chordOff();
        break;
      }
    }

    //TURN NOTE OFF
    usbMIDI.sendNoteOff(curNote.note, 0, midiChannel);
    if (!harmonizer->isOn()) displayNoteLeds(curNote, false);

    //To prevent delay after last note
    if (index == noteCount - 1) 
    {
      harmonizer->chordOff();
      break;
    }

    //Proceed to next note
    index++;
  }

  allowButtonInput = true;
  updateState(RecorderState::Off);
  updateMenuDisplay();
}

void clearMelody() 
{
  noteCount = 0;
}

void setMelodyMidiChannel(int channel)
{
  if (channel < 1) 
  {
    Serial.println(String("Error: Invalid MIDI channel ") + channel);
    return;
  }
  midiChannel = channel;
}

RecorderState getRecorderState() 
{
  return state;
}