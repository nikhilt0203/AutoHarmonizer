#pragma once
#include <Adafruit_NeoPixel.h>
#include "Note.h"

enum class ColorScheme {
  Chromatic, Ice, Mint, Rose, Alloy
};
ColorScheme nameToColorScheme(String scheme);

/**
 * @brief setup function for the neopixel LEDs.
 */
void ledSetup();

/**
 * @brief update the LEDs.
 */
void updateLeds();

/**
 * @brief changes the current color scheme.
 * @param newScheme the new ColorScheme
 */
void changeColorScheme(ColorScheme newScheme);

/**
 * @brief displays the current harmonizer chord on the keyboard LEDs.
 */
void displayChordLeds();

/**
 * @brief displays the current note on the keyboard LEDs on playback
 *  if the harmonizer is off.
 * @param note the Note to display
 * @param on if the note is on or off
 */
void displayNoteLeds(Note note, bool on);


/**
 * @brief changes the brightness of the LEDs.
 * @param brightness the brightness of the LEDs (>= 0, <= 255)
 */
void setLedBrightness(int brightness);