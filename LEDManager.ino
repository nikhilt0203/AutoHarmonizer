#include <Adafruit_NeoPixel.h>
#include <IntervalTimer.h>

IntervalTimer ledUpdate;

const int indicatorLedPin = 24;
const int keypadLedsPin = 15;

//convert the default neopixel index
const int ledNum[16] = {
  0, 1, 2, 3,
  7, 6, 5, 4,
  8, 9, 10, 11,
  15, 14, 13, 12
};

//current led
int keypadLedValues[16];

Adafruit_NeoPixel indicatorLed = Adafruit_NeoPixel(1, indicatorLedPin, NEO_RGB);
Adafruit_NeoPixel keypadLeds = Adafruit_NeoPixel(16, keypadLedsPin, NEO_GRB);

uint32_t RECORD_COLOR, PLAY_COLOR, PLAY_HARMONIZER_COLOR, NOTE_COLOR, TONIC_COLOR, DIATONIC_COLOR, NON_DIATONIC_COLOR;

String currentScheme;

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

void ledSetup() {
  indicatorLed.begin();
  indicatorLed.clear();
  indicatorLed.show();
  keypadLeds.begin();
  keypadLeds.clear();
  keypadLeds.show();
  int r, g, b;
  for (int i = 0; i < 16; i++) {
    r = rainbow[i][0];
    g = rainbow[i][1];
    b = rainbow[i][2];
    keypadLeds.setPixelColor(i, keypadLeds.Color(r, g, b));
    keypadLeds.show();
    delay(75);
  }
  for (int i = 0; i < 16; i++) {
    keypadLeds.setPixelColor(i, 0, 0, 0);
    keypadLeds.show();
    delay(75);
  }
  ledUpdate.begin(updateLeds, 10000);  //update LEDs every 10ms
  changeColorScheme("Ice"); //default scheme
}

void updateLeds() {
  if (isRecording) {
    indicatorLed.setPixelColor(0, RECORD_COLOR);
    indicatorLed.show();
  } else if (isPlaying) {
    if (harmonizers[DIATONIC]->isOn() || harmonizers[ATONAL]->isOn()) {
      indicatorLed.setPixelColor(0, PLAY_HARMONIZER_COLOR);
    } else {
      indicatorLed.setPixelColor(0, PLAY_COLOR);
    }
    indicatorLed.show();
  } else {
    //turn off LED
    indicatorLed.setPixelColor(0, 0, 0, 0);
    indicatorLed.show();
  }
  if (brightnessIncrease != -1) {
    keypadLeds.setPixelColor(ledNum[brightnessIncrease], NOTE_COLOR);  //light up pressed note
    keypadLeds.show();
  }
  if (diatonicHarmonizer.getKey() != currentKey || brightnessIncrease == -1) {
    updateKeypadLeds();  //update key LEDs if key has changed
  }
}

void updateKeypadLeds() {
  if (isPlaying) return;
  //update current key
  currentKey = diatonicHarmonizer.getKey();

  //scale patterns (1 = tonic, 0 = diatonic, -1 = not in key)
  int majorScale[12] = {
    1, -1, 0, -1,
    0, 0, -1, 0,
    -1, 0, -1, 0
  };

  int minorScale[12] = {
    1, -1, 0, 0,
    -1, 0, -1, 0,
    0, -1, -1, 0
  };

  //find the index of the tonic note, accounting for any range shift
  int tonicIndex = (currentKey - midiNotes[0] + 120) % 12;
  for (int i = 0; i < 16; i++) {
    if (!diatonicHarmonizer.isMinor()) {
      //Start at the lowest index before the tonic
      keypadLedValues[i] = majorScale[(12 - tonicIndex + i) % 12];
    } else {
      keypadLedValues[i] = minorScale[(12 - tonicIndex + i) % 12];
    }
  }

  //Set diatonic and non-diatonic note colors
  for (int i = 0; i < 16; i++) {
    if (!diatonicHarmonizer.isOn() && !atonalHarmonizer.isOn()) {  //Turn off all LEDs if harmonizer is off
      keypadLeds.setPixelColor(ledNum[i], 0, 0, 0);
      continue;
    }
    if (currentScheme.equals("Rainbow")) {
      updateRainbowColors();
    }
    if (keypadLedValues[i] == 1) {
      keypadLeds.setPixelColor(ledNum[i], TONIC_COLOR);
    } else if (keypadLedValues[i] == 0) {
      keypadLeds.setPixelColor(ledNum[i], DIATONIC_COLOR);
    } else {
      keypadLeds.setPixelColor(ledNum[i], NON_DIATONIC_COLOR);
    }
  }
  keypadLeds.show();
}

void changeColorScheme(String newScheme) {
  currentScheme = newScheme;

  if (currentScheme.equals("Rainbow")) {
    //brightnessItemAction(2);
    RECORD_COLOR = indicatorLed.Color(255, 0, 0);
    PLAY_COLOR = indicatorLed.Color(0, 255, 0);
    PLAY_HARMONIZER_COLOR = indicatorLed.Color(0, 0, 255);
    NON_DIATONIC_COLOR = keypadLeds.Color(0, 0, 0);
    NOTE_COLOR = keypadLeds.Color(50, 50, 50);
    updateRainbowColors(); //set DIATONIC_COLOR and TONIC_COLOR dependent on key
  }
  else if (currentScheme.equals("Mint")) {
    //brightnessItemAction(1);
    RECORD_COLOR = indicatorLed.Color(128, 0, 0);
    PLAY_COLOR = indicatorLed.Color(0, 128, 64);
    PLAY_HARMONIZER_COLOR = indicatorLed.Color(0, 255, 128);
    NOTE_COLOR = keypadLeds.Color(64, 128, 64);
    DIATONIC_COLOR = keypadLeds.Color(192, 255, 128);
    NON_DIATONIC_COLOR = keypadLeds.Color(25, 57, 25);
    NOTE_COLOR = keypadLeds.Color(64, 128, 64);
    TONIC_COLOR = keypadLeds.Color(160, 250, 160);
  }
  else if (currentScheme.equals("Alloy")) {
    //brightnessItemAction(1);
    RECORD_COLOR = indicatorLed.Color(255, 69, 0);
    PLAY_COLOR = indicatorLed.Color(255, 215, 0);
    PLAY_HARMONIZER_COLOR = indicatorLed.Color(255, 140, 0);
    NOTE_COLOR = keypadLeds.Color(155, 128, 81);
    DIATONIC_COLOR = keypadLeds.Color(150, 69, 19);
    NON_DIATONIC_COLOR = keypadLeds.Color(255, 165, 79);
    TONIC_COLOR = keypadLeds.Color(220, 100, 50);
  }
  else if (currentScheme.equals("Rose")) {
    //brightnessItemAction(0);
    RECORD_COLOR = indicatorLed.Color(255, 0, 50);
    PLAY_COLOR = indicatorLed.Color(200, 50, 50);
    PLAY_HARMONIZER_COLOR = indicatorLed.Color(150, 30, 38);
    NOTE_COLOR = keypadLeds.Color(100, 0, 25);
    DIATONIC_COLOR = keypadLeds.Color(255, 64, 128);
    NON_DIATONIC_COLOR = keypadLeds.Color(50, 0, 12);
    TONIC_COLOR = keypadLeds.Color(220, 80, 180);
  }
  else if (currentScheme.equals("Ice")) {
    //brightnessItemAction(0);
    RECORD_COLOR = indicatorLed.Color(255, 0, 0);
    PLAY_COLOR = indicatorLed.Color(0, 255, 0);
    PLAY_HARMONIZER_COLOR = indicatorLed.Color(0, 0, 255);
    NOTE_COLOR = keypadLeds.Color(100, 100, 100);
    DIATONIC_COLOR = keypadLeds.Color(0, 255, 255);
    NON_DIATONIC_COLOR = keypadLeds.Color(0, 0, 50);
    TONIC_COLOR = keypadLeds.Color(100, 225, 255);
  }
}

void updateRainbowColors() {
  if (!diatonicHarmonizer.isMinor()) {
    DIATONIC_COLOR = keypadLeds.Color(colors[currentKey][0], colors[currentKey][1], colors[currentKey][2]);
    TONIC_COLOR = keypadLeds.Color(colors[(currentKey + 1) % 12][0], colors[(currentKey + 1) % 12][1], colors[(currentKey + 1) % 12][2]);
  } else {
    int minorIndex = (currentKey + 3 + 12) % 12;
    DIATONIC_COLOR = keypadLeds.Color(colors[minorIndex][0], colors[minorIndex][1], colors[minorIndex][2]);
    TONIC_COLOR = keypadLeds.Color(colors[(minorIndex + 1) % 12][0], colors[(minorIndex + 1) % 12][1], colors[(minorIndex + 1) % 12][2]);
  }
}

void displayChordLeds() {
  for (int i = 0; i < 16; i++) {
    keypadLeds.setPixelColor(i, 0, 0, 0);
  }
  keypadLeds.show();
  //atonal harmonizer has chord length of 4
  for (int i = 0; i < (currentHarmonizer == 0 ? 3 : 4); i++) {
    int index = (harmonizers[currentHarmonizer]->getCurrentChord()[i] - midiNotes[0] + 120) % 12;
    keypadLeds.setPixelColor(ledNum[index], DIATONIC_COLOR);
  }
  keypadLeds.show();
}

void displayNoteLeds(Note note, bool on) {
  if (!on) {
    for (int i = 0; i < 16; i++) {
      keypadLeds.setPixelColor(i, 0, 0, 0);
    }
    keypadLeds.show();
    return;
  }
  int index = (note.note - midiNotes[0] + 160) % 16;
  keypadLeds.setPixelColor(ledNum[index], DIATONIC_COLOR);
  keypadLeds.show();
}