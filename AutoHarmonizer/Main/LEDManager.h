#pragma once
#include <Adafruit_NeoPixel.h>
#include "Note.h"

extern Adafruit_NeoPixel indicatorLed;
extern Adafruit_NeoPixel keypadLeds;

void ledSetup();

void updateLeds();

void updateKeypadLeds();

void changeColorScheme(String newScheme);

void updateRainbowColors();

void displayChordLeds();

void displayNoteLeds(Note note, bool on);

void setLedBrightness(int brightness);

int indexToLedNum(int index);