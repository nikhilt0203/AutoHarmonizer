#include "MenuManager.h"
#include "MenuItem.h"
#include "Keyboard.h"
#include "DisplayManager.h"
#include "LEDManager.h"
#include "MelodyRecorder.h"

//MenuItems containing an array of options
static std::vector<const char*> keyOptions = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" };
static std::vector<const char*> modeOptions = { "Major", "Minor", "Atonal" };
static std::vector<const char*> colorSchemeOptions = { "Chromatic", "Ice", "Rose", "Mint", "Alloy" };

MenuItem keyItem("Key", keyOptions, keyItemAction);
MenuItem modeItem("Mode", modeOptions, modeItemAction);
MenuItem colorSchemeItem("Color Scheme", colorSchemeOptions, colorSchemeItemAction);

//MenuItems that toggle between on or off
OnOffItem harmonizerOnOffItem(true, harmonizerOnOffItemAction);

//MenuItems that control a single parameter
ParameterItem harmonizerOctaveItem("Octave", -4, 4, harmonizerOctaveItemAction);
ParameterItem brightnessItem("Brightness", -8, 8, brightnessItemAction);
ParameterItem keyboardRangeItem("Keyboard Range", -60, 67, keyboardRangeItemAction);
ParameterItem melodyMidiChannelItem("Melody Channel", 1, 16, melodyMidiChannelItemAction);
ParameterItem harmonizerMidiChannelItem("Harmonizer Channel", 1, 16, harmonizerMidiChannelItemAction);

//MenuItems containing sub-MenuItems
std::vector<MenuItem*> harmonizerMenuItems = { &keyItem, &modeItem, &harmonizerOctaveItem, &harmonizerOnOffItem };
MenuItem harmonizerMenu("Harmonizer Settings", harmonizerMenuItems, harmonizerMenuAction);

std::vector<MenuItem*> midiMenuItems = { &melodyMidiChannelItem, &harmonizerMidiChannelItem };
MenuItem midiMenu("MIDI Settings", midiMenuItems, midiMenuAction);

std::vector<MenuItem*> ledMenuItems = { &brightnessItem, &colorSchemeItem };
MenuItem ledMenu("LED Settings", ledMenuItems, ledMenuAction);

std::vector<MenuItem*> mainMenuItems = { &harmonizerMenu, &ledMenu, &midiMenu, &keyboardRangeItem };
MenuItem mainMenu("Main Menu", mainMenuItems, mainMenuAction);

//the index of the current menu in MenuItem::allItems
int currentMenu;

void menuSetup() 
{
  //Go to main menu on startup
  currentMenu = mainMenu.getItemNum();
  updateMenuDisplay();

  //default midi channels
  melodyMidiChannelItem.setOption(1);
  harmonizerMidiChannelItem.setOption(2);
}

void updateMenuDisplay() 
{
  if (!(currentMenu >= 0 && currentMenu < (int)MenuItem::allItems.size())) {
    Serial.println(String("Error: Menu index ") + currentMenu + " is out of bounds for menu size " + MenuItem::allItems.size());
    return;
  }

  MenuItem* currentItem = MenuItem::allItems[currentMenu];
  if(currentItem == nullptr) {
    Serial.println("Error: current menu item is null.");
    return;
  }

  String prefix = "";
  clearDisplay();
  displayText(currentItem->name(), 0, false);  //display title at line 0

  //If displaying a single parameter item
  if (currentItem->itemType() == MenuItem::ItemType::Parameter) 
  {
    //add "+" before the number to indicate that it is positive (except for midi channel items, because it does not apply)
    if (currentItem->currentOptionIndex() > 0 && !(currentItem->name().equals("Melody Channel") || currentItem->name().equals("Harmonizer Channel")))
    {
      prefix = "+";
    }

    displayText(currentItem->currentOptionIndex(), 2, false, 4, prefix);
    return;
  }

  if (currentItem->name().equals("Color Scheme")) prefix = "> ";

  //For items with 4 or less options, list all the options vertically
  if (currentItem->getNumOptions() <= 4) 
  {
    for (int i = 0; i < currentItem->getNumOptions(); i++) 
    { 
      //currently selected option is displayed on first iteration, i = 0
      displayText(currentItem->currentOptionName(), currentItem->currentOptionIndex() + 2, (i == 0));
      currentItem->next(); //move to next option
    }
  } 
  else //if there are >4 options, just display the current option
  {
    displayText(currentItem->currentOptionName(), 2, false, 1, prefix);
  }
}

void previousMenu() 
{
  //Main menu has no previous menu
  if(currentMenu == mainMenu.getItemNum()) return; 

  MenuItem* currentItem = MenuItem::allItems[currentMenu];

  /* So that the selected option is displayed when the item is re-entered, 
   * rather than displaying the last cycled-through option
   */
  currentItem->setOption(currentItem->selectedOptionIndex());

  currentMenu = currentItem->getParentNum();
  updateMenuDisplay();
}

void encoderPress(int _buttonNum) 
{
  MenuItem::allItems[currentMenu]->select();
  updateMenuDisplay();
}

void clickLeft() 
{
  MenuItem::allItems[currentMenu]->next();
  updateMenuDisplay();
}

void clickRight() 
{
  MenuItem::allItems[currentMenu]->previous();
  updateMenuDisplay();
}

void mainMenuAction(int optionIndex) 
{
  currentMenu = mainMenu.getSubItems()[optionIndex]->getItemNum();
}

void harmonizerMenuAction(int optionIndex) 
{
  if (optionIndex == 3) 
  {
    harmonizerOnOffItem.select();
  }
  else 
  {
    currentMenu = harmonizerMenu.getSubItems()[optionIndex]->getItemNum();
  }
}

void keyItemAction(int optionIndex) 
{
  //past the key of F, shift range down so tonic isn't too far from the first key
  if (optionIndex > 5) keyboardRangeItem.setOption(optionIndex - 6 + optionIndex % 2);
  else keyboardRangeItem.setOption(0);
  diatonicHarmonizer.setKey(optionIndex);
}

void modeItemAction(int optionIndex) 
{
  if (optionIndex == 0) 
  {
    currentHarmonizer = DIATONIC;
    diatonicHarmonizer.toMajor();
  }
  else if (optionIndex == 1) 
  {
    currentHarmonizer = DIATONIC;
    diatonicHarmonizer.toMinor();
  }
  else if (optionIndex == 2) 
  {
    currentHarmonizer = ATONAL;
  }
}

void harmonizerOctaveItemAction(int optionIndex) {
  diatonicHarmonizer.setOctave(optionIndex);
}

void harmonizerOnOffItemAction(int optionIndex) 
{
  if (optionIndex == 1) 
  {
    harmonizers[DIATONIC]->on();
    harmonizers[ATONAL]->on();
  } 
  else 
  {
    harmonizers[DIATONIC]->off();
    harmonizers[ATONAL]->off();
  }
}

void melodyMidiChannelItemAction(int optionIndex) 
{
  setMelodyMidiChannel(optionIndex);
}

void harmonizerMidiChannelItemAction(int optionIndex) 
{
  harmonizers[DIATONIC]->setChannel(optionIndex);
  harmonizers[ATONAL]->setChannel(optionIndex);
}

void keyboardRangeItemAction(int optionIndex) 
{
  changeKeyboardRange(optionIndex);
}

void midiMenuAction(int optionIndex) 
{
  currentMenu = midiMenu.getSubItems()[optionIndex]->getItemNum();
}

void ledMenuAction(int optionIndex) 
{
  currentMenu = ledMenu.getSubItems()[optionIndex]->getItemNum();
}

void brightnessItemAction(int optionIndex) 
{
  setLedBrightness(map(optionIndex, brightnessItem.getMin(), brightnessItem.getMax(), 0, 255));
}

void colorSchemeItemAction(int optionIndex) 
{
  changeColorScheme(nameToColorScheme(colorSchemeItem.currentOptionName()));
}