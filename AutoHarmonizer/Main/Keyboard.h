#pragma once
#include "Adafruit_Keypad.h"

//which key is currently pressed and lit up
extern int litUpKey;

/**
 * @brief setup function for the keyboard
 */
void keyboardSetup();

/**
 * @brief update function for the keyboard
 */
void updateKeyboard();

/**
 * @brief shifts the array of MIDI notes that correspond
 *        to the keyboard keys up(+) or down(-) by the specified amount.
 * @param shift the range shift to apply
 */
void changeKeyboardRange(int shift);

/**
 * @brief returns the current keyboard range shift.
 *        first keyboard key is MIDI note 60 by default
 * @return the current shift
 */
int getShift();