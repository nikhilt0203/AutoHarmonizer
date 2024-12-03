#pragma once
#include "MenuItem.h"
#include "ParameterItem.h"
#include "OnOffItem.h"

// Item objects
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

//Menu functions
void menuSetup();
void updateCurrentMenu();
void previousMenu();
void encoderPress(int _buttonNum);
void clickLeft();
void clickRight();

//Item actions
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