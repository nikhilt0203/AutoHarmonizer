#include "MenuManager.h"
#include "MenuItem.h"
#include "Keyboard.h"
#include "DisplayManager.h"
#include "LEDManager.h"
#include "MelodyRecorder.h"

//Items containing an array of options
static std::vector<const char*> keyOptions = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" };
static std::vector<const char*> modeOptions = { "Major", "Minor", "Atonal" };
static std::vector<const char*> colorSchemeOptions = { "Chromatic", "Ice", "Rose", "Mint", "Alloy" };

MenuItem keyItem("Key", keyOptions, keyItemAction);
MenuItem modeItem("Mode", modeOptions, modeItemAction);
MenuItem colorSchemeItem("Color Scheme", colorSchemeOptions, colorSchemeItemAction);

//Items that are on or off
OnOffItem harmonizerOnOffItem(true, harmonizerOnOffItemAction);

//Items containing a single parameter
ParameterItem harmonizerOctaveItem("Octave", -4, 4, harmonizerOctaveItemAction);
ParameterItem brightnessItem("Brightness", -10, 10, brightnessItemAction);
ParameterItem keyboardRangeItem("Keyboard Range", -60, 67, keyboardRangeItemAction);

//Items containing subitems
std::vector<MenuItem*> harmonizerMenuItems = { &keyItem, &modeItem, &harmonizerOctaveItem, &harmonizerOnOffItem };
MenuItem harmonizerMenu("Harmonizer Settings", harmonizerMenuItems, harmonizerMenuAction);

std::vector<MenuItem*> ledMenuItems = { &brightnessItem, &colorSchemeItem };
MenuItem ledMenu("LED Settings", ledMenuItems, ledMenuAction);

std::vector<MenuItem*> mainMenuItems = { &harmonizerMenu, &keyboardRangeItem, &ledMenu };
MenuItem mainMenu("Main Menu", mainMenuItems, mainMenuAction);

int currentMenu;

void menuSetup() {
  currentMenu = mainMenu.getItemNum();
  updateCurrentMenu();
}

void updateCurrentMenu() {
  if (!(currentMenu >= 0 && currentMenu < (int)MenuItem::allItems.size())) return;

  MenuItem* item = MenuItem::allItems[currentMenu];
  String prefix = "";
  clearDisplay();
  displayText(item->name(), 0, false);  //display title at line 0

  if (item->itemType() == MenuItem::ItemType::Parameter) {
    //add "+" before the number if it is positive
    if (item->currentOptionIndex() > 0) prefix = "+";
    displayText(item->currentOptionIndex(), 2, false, 4, prefix);
    return;
  }

  if (item->name().equals("Color Scheme")) prefix = "> ";

  //if there are 4 or less options, list all of them vertically
  if (item->getNumOptions() <= 4) {
    for (int i = 0; i < item->getNumOptions(); i++) {
      int currentIndex = item->currentOptionIndex();
      displayText(item->currentOptionName(), currentIndex + 2, (i == 0));
      item->next();
    }
  } else {  //if there >4, just display the current option
    displayText(item->currentOptionName(), 2, false, 1, prefix);
  }
}

void previousMenu() {
  currentMenu = MenuItem::allItems[currentMenu]->getParentNum();
  updateCurrentMenu();
}

void encoderPress(int _buttonNum) {
  MenuItem::allItems[currentMenu]->select();
  updateCurrentMenu();
}

void clickLeft() {
  MenuItem::allItems[currentMenu]->next();
  updateCurrentMenu();
}

void clickRight() {
  MenuItem::allItems[currentMenu]->previous();
  updateCurrentMenu();
}

void mainMenuAction(int optionIndex) {
  currentMenu = mainMenu.getSubItems()[optionIndex]->getItemNum();
}

void harmonizerMenuAction(int optionIndex) {
  if (optionIndex == 3) {
    harmonizerOnOffItem.select();
  } else {
    currentMenu = harmonizerMenu.getSubItems()[optionIndex]->getItemNum();
  }
}

void keyItemAction(int optionIndex) {
  //past the key of F, shift range down so tonic isn't too far from the first key
  if (optionIndex > 5) keyboardRangeItem.setOption(optionIndex - 6 + optionIndex % 2);
  else keyboardRangeItem.setOption(0);
  diatonicHarmonizer.setKey(optionIndex);
}

void modeItemAction(int optionIndex) {
  if (optionIndex == 0) {
    currentHarmonizer = DIATONIC;
    diatonicHarmonizer.toMajor();
  }
  if (optionIndex == 1) {
    currentHarmonizer = DIATONIC;
    diatonicHarmonizer.toMinor();
  }
  if (optionIndex == 2)
    currentHarmonizer = ATONAL;
}

void harmonizerOctaveItemAction(int optionIndex) {
  diatonicHarmonizer.setOctave(optionIndex);
}

void harmonizerOnOffItemAction(int optionIndex) {
  if (optionIndex == 1) {
    harmonizers[DIATONIC]->on();
    harmonizers[ATONAL]->on();

  } else {
    harmonizers[DIATONIC]->off();
    harmonizers[ATONAL]->off();
  }
}

void keyboardRangeItemAction(int optionIndex) {
  changeKeyboardRange(optionIndex);
}

void ledMenuAction(int optionIndex) {
  currentMenu = ledMenu.getSubItems()[optionIndex]->getItemNum();
}

void brightnessItemAction(int optionIndex) {
  setLedBrightness(map(optionIndex, -10, 10, 0, 255));
}

void colorSchemeItemAction(int optionIndex) {
  changeColorScheme(colorSchemeItem.currentOptionName());
}