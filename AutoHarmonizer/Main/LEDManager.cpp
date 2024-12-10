#include "LEDManager.h"
#include <Adafruit_NeoPixel.h>
#include "ButtonManager.h"
#include "Keyboard.h"
#include "MenuManager.h"
#include "MelodyRecorder.h"

const int indicatorLedPin = 24;
const int keyboardLedsPin = 15;

Adafruit_NeoPixel indicatorLed = Adafruit_NeoPixel(1, indicatorLedPin, NEO_RGB);
Adafruit_NeoPixel keyboardLeds = Adafruit_NeoPixel(16, keyboardLedsPin, NEO_GRB);
//IntervalTimer ledUpdate;

uint32_t RECORD_COLOR, PLAY_COLOR, PLAY_HARMONIZER_COLOR, NOTE_COLOR, TONIC_COLOR, DIATONIC_COLOR, NON_DIATONIC_COLOR;
ColorScheme currentScheme;
int currentKey = -1;

int keyboardLedValues[16];

const int rainbow[16][3] = {
  { 255, 0, 0 },
  { 255, 50, 0 },
  { 255, 100, 0 },
  { 255, 200, 0 },
  { 255, 255, 0 },
  { 180, 255, 0 },
  { 127, 255, 0 },
  { 0, 255, 0 },
  { 0, 255, 127 },
  { 0, 255, 255 },
  { 0, 127, 255 },
  { 0, 0, 255 },
  { 127, 0, 255 },
  { 200, 0, 255 },
  { 255, 0, 255 },
  { 255, 0, 127 }
};

const int colors[12][3] = {
  { 255, 0, 0 },
  { 255, 100, 0 },
  { 255, 255, 0 },
  { 127, 255, 0 },
  { 0, 255, 0 },
  { 0, 255, 127 },
  { 0, 255, 255 },
  { 0, 127, 255 },
  { 0, 0, 255 },
  { 127, 0, 255 },
  { 255, 0, 255 },
  { 255, 0, 127 }
};

/* Neokeys wrap around from the right by default. This function
 * is used to convert an index to the expected neopixel index (wrap around from
 * the left)
 */
int indexToLedNum(int index) 
{
  static int ledNum[16] = {
    0, 1, 2, 3,
    7, 6, 5, 4,
    8, 9, 10, 11,
    15, 14, 13, 12
  };
  return ledNum[index];
}

void ledSetup() 
{
  indicatorLed.begin();
  indicatorLed.clear();
  indicatorLed.show();
  keyboardLeds.begin();
  keyboardLeds.clear();
  keyboardLeds.show();

  for (int i = 0; i < 16; i++) 
  {
    keyboardLeds.setPixelColor(i, keyboardLeds.Color(rainbow[i][0], rainbow[i][1], rainbow[i][2]));
    keyboardLeds.show();
    delay(75);
  }
  for (int i = 0; i < 16; i++) 
  {
    keyboardLeds.setPixelColor(i, 0, 0, 0);
    keyboardLeds.show();
    delay(75);
  }
  changeColorScheme(ColorScheme::Chromatic);  //default scheme
}

void updateChromaticScheme() 
{
  if (!diatonicHarmonizer.isMinor()) 
  {
    DIATONIC_COLOR = keyboardLeds.Color(colors[currentKey][0], colors[currentKey][1], colors[currentKey][2]);
    TONIC_COLOR = keyboardLeds.Color(colors[(currentKey + 1) % 12][0], colors[(currentKey + 1) % 12][1], colors[(currentKey + 1) % 12][2]);
  } 
  else 
  {
    int minorIndex = (currentKey + 3 + 12) % 12;
    DIATONIC_COLOR = keyboardLeds.Color(colors[minorIndex][0], colors[minorIndex][1], colors[minorIndex][2]);
    TONIC_COLOR = keyboardLeds.Color(colors[(minorIndex + 1) % 12][0], colors[(minorIndex + 1) % 12][1], colors[(minorIndex + 1) % 12][2]);
  }
}

void updateKeyboardLeds() 
{
  if (getRecorderState() == RecorderState::Playing) return;
  //update current key
  currentKey = diatonicHarmonizer.getKey();

  //scale patterns for 1 octave (1 = tonic, 0 = diatonic, -1 = not in key)
  int majorScale[12] = {
    1, -1, 0, -1,
    0, 0, -1, 0,
    -1, 0, -1, 0
  };

  int minorScale[12] = {
    1, -1, 0, 0,
    -1, 0, -1, 0,
    0, -1, 0, -1
  };

  //find the index of the tonic note, accounting for any range shift
  int tonicIndex = (currentKey - getShift() + 120) % 12;
  for (int i = 0; i < 16; i++) 
  {
    if (!diatonicHarmonizer.isMinor()) 
    {
      //Start at the lowest index before the tonic
      keyboardLedValues[i] = majorScale[(12 - tonicIndex + i) % 12];
    } 
    else 
    {
      keyboardLedValues[i] = minorScale[(12 - tonicIndex + i) % 12];
    }
  }

  //Set diatonic and non-diatonic note colors
  for (int i = 0; i < 16; i++) 
  {
    if (!diatonicHarmonizer.isOn() && !atonalHarmonizer.isOn()) 
    { 
      keyboardLeds.setPixelColor(indexToLedNum(i), 0, 0, 0);
      continue;
    }
    if (currentScheme == ColorScheme::Chromatic) 
    {
      updateChromaticScheme();
    }

    if (keyboardLedValues[i] == 1) 
    {
      keyboardLeds.setPixelColor(indexToLedNum(i), TONIC_COLOR);
    } 
    else if (keyboardLedValues[i] == 0) 
    {
      keyboardLeds.setPixelColor(indexToLedNum(i), DIATONIC_COLOR);
    } 
    else 
    {
      keyboardLeds.setPixelColor(indexToLedNum(i), NON_DIATONIC_COLOR);
    }
  }
  keyboardLeds.show();
}

void updateLeds() 
{
  switch (getRecorderState())
  {
    case RecorderState::Playing:
      if (harmonizers[DIATONIC]->isOn() || harmonizers[ATONAL]->isOn()) 
      {
        indicatorLed.setPixelColor(0, PLAY_HARMONIZER_COLOR);
      } 
      else
      {
        indicatorLed.setPixelColor(0, PLAY_COLOR);
      }
      indicatorLed.show();
      break;

    case RecorderState::Recording:
      indicatorLed.setPixelColor(0, RECORD_COLOR);
      indicatorLed.show();
      break;

    case RecorderState::Off:
      indicatorLed.setPixelColor(0, 0, 0, 0);
      indicatorLed.show();
      break;
  }

  if (litUpKey != -1) 
  {
    keyboardLeds.setPixelColor(indexToLedNum(litUpKey), NOTE_COLOR);  //light up pressed note
    keyboardLeds.show();
  }

  if (diatonicHarmonizer.getKey() != currentKey || litUpKey == -1) 
  {
    updateKeyboardLeds();
  }
}

void changeColorScheme(ColorScheme newScheme) 
{
  switch (newScheme)
  {
    case ColorScheme::Chromatic:
      RECORD_COLOR = indicatorLed.Color(255, 0, 0);
      PLAY_COLOR = indicatorLed.Color(0, 255, 0);
      PLAY_HARMONIZER_COLOR = indicatorLed.Color(0, 0, 255);
      NON_DIATONIC_COLOR = keyboardLeds.Color(0, 0, 0);
      NOTE_COLOR = keyboardLeds.Color(50, 50, 50);
      updateChromaticScheme();  //DIATONIC_COLOR and TONIC_COLOR updated dynamically based on key
      break;

    case ColorScheme::Mint:
      RECORD_COLOR = indicatorLed.Color(128, 0, 0);
      PLAY_COLOR = indicatorLed.Color(0, 128, 64);
      PLAY_HARMONIZER_COLOR = indicatorLed.Color(0, 255, 128);
      NOTE_COLOR = keyboardLeds.Color(64, 128, 64);
      DIATONIC_COLOR = keyboardLeds.Color(192, 255, 128);
      NON_DIATONIC_COLOR = keyboardLeds.Color(6, 13, 6);
      NOTE_COLOR = keyboardLeds.Color(64, 128, 64);
      TONIC_COLOR = keyboardLeds.Color(160, 250, 160);
      break;

    case ColorScheme::Alloy:
      RECORD_COLOR = indicatorLed.Color(255, 69, 0);
      PLAY_COLOR = indicatorLed.Color(255, 215, 0);
      PLAY_HARMONIZER_COLOR = indicatorLed.Color(255, 140, 0);
      NOTE_COLOR = keyboardLeds.Color(155, 128, 81);
      DIATONIC_COLOR = keyboardLeds.Color(255, 165, 79);
      NON_DIATONIC_COLOR = keyboardLeds.Color(150, 69, 19);
      TONIC_COLOR = keyboardLeds.Color(220, 100, 50);
      break;

    case ColorScheme::Rose:
      RECORD_COLOR = indicatorLed.Color(255, 0, 50);
      PLAY_COLOR = indicatorLed.Color(200, 50, 50);
      PLAY_HARMONIZER_COLOR = indicatorLed.Color(150, 30, 38);
      NOTE_COLOR = keyboardLeds.Color(100, 0, 25);
      DIATONIC_COLOR = keyboardLeds.Color(255, 64, 128);
      NON_DIATONIC_COLOR = keyboardLeds.Color(50, 0, 12);
      TONIC_COLOR = keyboardLeds.Color(220, 80, 180);
      break;

    case ColorScheme::Ice:
      RECORD_COLOR = indicatorLed.Color(255, 0, 0);
      PLAY_COLOR = indicatorLed.Color(0, 255, 0);
      PLAY_HARMONIZER_COLOR = indicatorLed.Color(0, 0, 255);
      NOTE_COLOR = keyboardLeds.Color(100, 100, 100);
      DIATONIC_COLOR = keyboardLeds.Color(0, 255, 255);
      NON_DIATONIC_COLOR = keyboardLeds.Color(0, 0, 50);
      TONIC_COLOR = keyboardLeds.Color(100, 225, 255);
      break;
  }
  currentScheme = newScheme;
}

void displayChordLeds() 
{
  for (int i = 0; i < 16; i++) 
  {
    keyboardLeds.setPixelColor(i, 0, 0, 0);
  }
  keyboardLeds.show();
  //atonal harmonizer has chord length of 4
  for (int i = 0; i < (currentHarmonizer == 0 ? 3 : 4); i++) 
  {
    int chordNote = harmonizers[currentHarmonizer]->getCurrentChord()[i];
    int index = (chordNote - getShift() + 120) % 12;
    keyboardLeds.setPixelColor(indexToLedNum(index), DIATONIC_COLOR);
  }
  keyboardLeds.show();
}

void displayNoteLeds(Note note, bool on) 
{
  if (on) 
  { 
    Serial.println(note.note);
    int index = (note.note - getShift() + 160) % 16;
    Serial.println(index);
    keyboardLeds.setPixelColor(indexToLedNum(index), DIATONIC_COLOR);
    keyboardLeds.show();
  }
  else
  {
    for (int i = 0; i < 16; i++) 
    {
      keyboardLeds.setPixelColor(i, 0, 0, 0);
    }
    keyboardLeds.show();
    return;
  }
}

void setLedBrightness(int brightness) 
{
  if (brightness < 0 || brightness > 255) 
  {
    Serial.println(String("Error: Invalid LED brightness ") + brightness + ".");
    return;
  }

  keyboardLeds.setBrightness(brightness);
  indicatorLed.setBrightness(brightness);
}

ColorScheme nameToColorScheme(String scheme) 
{
  if (scheme.equals("Chromatic")) return ColorScheme::Chromatic;
  if (scheme.equals("Ice")) return ColorScheme::Ice;
  if (scheme.equals("Rose")) return ColorScheme::Rose;
  if (scheme.equals("Mint")) return ColorScheme::Mint;
  if (scheme.equals("Alloy")) return ColorScheme::Alloy;
  
  Serial.println(String("Error: No color scheme named '") + scheme + "' found.");
  return ColorScheme::Chromatic; //default
}