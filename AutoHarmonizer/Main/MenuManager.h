#pragma once
#include "MenuItem.h"
#include "ParameterItem.h"
#include "OnOffItem.h"

/**
 * @brief declares MenuItem objects and manages the menu system.
 */

//MenuItem objects
extern MenuItem midiMenu, harmonizerMenu, ledMenu, mainMenu, keyItem, modeItem, colorSchemeItem;
extern ParameterItem harmonizerOctaveItem, brightnessItem, keyboardRangeItem, melodyMidiChannelItem, harmonizerMidiChannelItem;
extern OnOffItem harmonizerOnOffItem;

/**
 * @brief MenuItem callback functions. Called when the given item is selected.
 * @param optionIndex the index of the item's current option
 */
void mainMenuAction(int optionIndex);
void harmonizerMenuAction(int optionIndex);
void ledMenuAction(int optionIndex);
void midiMenuAction(int optionIndex);
void keyItemAction(int optionIndex);
void modeItemAction(int optionIndex);
void harmonizerOctaveItemAction(int optionIndex);
void harmonizerOnOffItemAction(int optionIndex);
void keyboardRangeItemAction(int optionIndex);
void ledMenuAction(int optionIndex);
void melodyMidiChannelItemAction(int optionIndex);
void harmonizerMidiChannelItemAction(int optionIndex);
void brightnessItemAction(int optionIndex);
void colorSchemeItemAction(int optionIndex);

/**
 * @brief begin the menu
 */
void menuSetup();

/**
 * @brief update the menu display with the current information.
 */
void updateMenuDisplay();         

/**
 * @brief return to the previous menu.
 */
void previousMenu();                

/**
 * @brief select the current item. Called when encoder button is pressed.
 */
void encoderPress(int _buttonNum);

/**
 * @brief proceed to next item. Called when encoder is clicked left.
 */
void clickLeft();

/**
 * @brief return to previous item. Called when encoder is clicked right.
 */
void clickRight();