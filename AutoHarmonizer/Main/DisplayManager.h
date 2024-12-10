#pragma once
#include <Arduino.h>

/**
 * @brief setup function for the OLED display.
 */
void displaySetup();

/**
 * @brief displays text on the OLED display.
 * @param text the text to display
 * @param line the line # to display the text on
 * @param isSelected if the text is an option that is selected
 * @param size the text size
 * @param prefix prefix for the text
 */
void displayText(String text, int line, bool isSelected, int size = 1, String prefix = "");


/**
 * @brief displays the current chord on the OLED display.
 * @param chord the chord to display, in string format
 */
void displayChord(String chord);

/**
 * @brief clears the OLED display.
 */
void clearDisplay();