#pragma once
#include "MenuItem.h"
#include "ParameterItem.h"
#include "OnOffItem.h"

//MenuItem object declarations
extern MenuItem keyItem;
extern MenuItem modeItem;
extern MenuItem colorSchemeItem;
extern OnOffItem harmonizerOnOffItem;
extern ParameterItem harmonizerOctaveItem;
extern ParameterItem brightnessItem;
extern ParameterItem keyboardRangeItem;
extern MenuItem harmonizerMenu;
extern MenuItem ledMenu;
extern MenuItem mainMenu;

/**
 * @brief begin the menu
 */
void menuSetup();

/**
 * @brief update the menu with the current information.
 */
void updateCurrentMenu();         

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

/**
 * @brief MenuItem callback functions. Called when the given item is selected.
 * @param optionIndex the index of the item's current option
 */
void mainMenuAction(int optionIndex);
void harmonizerMenuAction(int optionIndex);
void keyItemAction(int optionIndex);
void modeItemAction(int optionIndex);
void harmonizerOctaveItemAction(int optionIndex);
void harmonizerOnOffItemAction(int optionIndex);
void keyboardRangeItemAction(int optionIndex);
void ledMenuAction(int optionIndex);
void brightnessItemAction(int optionIndex);
void colorSchemeItemAction(int optionIndex);