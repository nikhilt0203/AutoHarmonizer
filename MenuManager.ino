// #include "MenuItem.h"
// #include "ParameterItem.h"
// #include "OnOffItem.h"

// //Items containing an array of options
// std::vector<const char*> keyOptions = { "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" };
// MenuItem keyItem("Key", 1, keyOptions);

// std::vector<const char*>  modeOptions = { "Major", "Minor", "Atonal" };
// MenuItem modeItem("Mode", 1, modeOptions);

// std::vector<const char*> colorSchemeOptions = { "Ice", "Mint", "Rose", "Alloy", "Rainbow" };
// MenuItem colorSchemeItem("Color Scheme", 3, colorSchemeOptions);

// //Items that toggle on/off
// OnOffItem harmonizerOnOffItem("ON", 1);

// //Items containing a single parameter
// ParameterItem harmonizerOctaveItem("Octave", 1, -4, 4);
// ParameterItem brightnessItem("Brightness", 3, -5, 5);
// ParameterItem keyboardRangeItem("Keyboard Range", 0, -60, 67);

// //Items containing other items
// std::vector<MenuItem*> harmonizerMenuItems = { &keyItem, &modeItem, &harmonizerOctaveItem, &harmonizerOnOffItem };
// MenuItem harmonizerMenu("Harmonizer Settings", 0, harmonizerMenuItems);

// std::vector<MenuItem*> ledMenuItems = { &brightnessItem, &colorSchemeItem };
// MenuItem ledMenu("LED Settings", 0, ledMenuItems);

// std::vector<MenuItem*> mainMenuItems = { &harmonizerMenu, &keyboardRangeItem, &ledMenu };
// MenuItem mainMenu("Main Menu", 0, mainMenuItems);

// //Functions that items execute
// void (*itemActions[10])(int) = {
//   keyItemAction, modeItemAction, colorSchemeItemAction,
//   harmonizerOnOffItemAction, harmonizerOctaveItemAction,
//   brightnessItemAction, keyboardRangeItemAction,
//   harmonizerMenuAction, ledMenuAction, mainMenuAction
// };

// int currentMenu;

// void menuSetup() {
//   displaySetup();
//   for (size_t i = 0; i < MenuItem::menu.size(); i++) {
//     MenuItem::menu[i]->onPress(itemActions[i]);
//   }
//   //begin at main menu
//   currentMenu = mainMenu.getNum();
//   updateCurrentMenu();
// }

// void updateCurrentMenu() {
//   MenuItem* item = MenuItem::menu[currentMenu];

//   String prefix = "";
//   display.clearDisplay();
//   displayText(item->name(), 0, false);

//   if (item->itemType().equals("parameter")) {
//     if (item->currentOptionIndex() > 0) prefix = "+";
//     displayText(item->currentOptionIndex(), 2, false, 4, prefix);
//     return;
//   }

//   if (item->name().equals("Color Scheme")) prefix = "> ";
//   if (item->getNumOptions() <= 4) {
//     for (int i = 0; i < item->getNumOptions(); i++) {
//       int currentIndex = item->currentOptionIndex();
//       displayText(item->currentOptionName(), currentIndex + 2, (i == 0));
//       item->next();
//     }
//   } else {
//     displayText(item->currentOptionName(), 2, false, 1, prefix);
//   }
// }

// void previousMenu() {
//   currentMenu = MenuItem::menu[currentMenu]->getParentNum();
//   updateCurrentMenu();
// }

// void encoderPress(int _buttonNum) {
//   MenuItem::menu[currentMenu]->select();
//   updateCurrentMenu();
// }

// void clickLeft() {
//   MenuItem::menu[currentMenu]->next();
//   updateCurrentMenu();
// }

// void clickRight() {
//   MenuItem::menu[currentMenu]->previous();
//   updateCurrentMenu();
// }

// void keyItemAction(int optionNum) {
//   diatonicHarmonizer.setKey(optionNum);
//   //past the key of F, shift range down so tonic isn't too far from the first key
//   if (optionNum > 5) keyboardRangeItem.setOption(optionNum - 6 + optionNum % 2);
//   else keyboardRangeItem.setOption(0);
// }

// void modeItemAction(int optionNum) {
//   if (optionNum == 0) {
//     currentHarmonizer = DIATONIC;
//     diatonicHarmonizer.toMajor();
//   } else if (optionNum == 1) {
//     currentHarmonizer = DIATONIC;
//     diatonicHarmonizer.toMinor();
//   } else if (optionNum == 2)
//     currentHarmonizer = ATONAL;
// }

// void brightnessItemAction(int optionNum) {
//   int mappedBrightness = map(optionNum, -5, 5, 0, 255);
//   keypadLeds.setBrightness(mappedBrightness);
//   indicatorLed.setBrightness(mappedBrightness);
// }

// void colorSchemeItemAction(int optionNum) {
//   changeColorScheme(colorSchemeItem.currentOptionName());
// }

// void keyboardRangeItemAction(int optionNum) {
//   static int previousShift = 0;
//   int shift = optionNum - previousShift;
//   previousShift = optionNum;
//   //verify lowest and highest notes are in MIDI range
//   int upper = midiNotes[15] + shift;
//   int lower = midiNotes[0] + shift;
//   if (upper <= 127 && lower >= 0) {
//     for (int i = 0; i < 16; i++) {
//       midiNotes[i] += shift;
//     }
//   }
// }

// void harmonizerOctaveItemAction(int optionNum) {
//   diatonicHarmonizer.setOctave(optionNum);
// }

// void harmonizerOnOffItemAction(int optionNum) {
//   if (optionNum == 1) {
//     harmonizers[DIATONIC]->on();
//     harmonizers[ATONAL]->on();
//   } else if (optionNum == 0) {
//     harmonizers[DIATONIC]->off();
//     harmonizers[ATONAL]->off();
//   }
// }

// void harmonizerMenuAction(int optionNum) {
//   if (optionNum == 3) harmonizerOnOffItem.select();
//   else currentMenu = harmonizerMenu.getCurrentItem()->getNum();
// }

// void ledMenuAction(int optionNum) {
//   currentMenu = ledMenu.getCurrentItem()->getNum();
// }

// void mainMenuAction(int optionNum) {
//   currentMenu = mainMenu.getCurrentItem()->getNum();
// }

// void changeKeyboardRange(int newRangeShift) {
//   //calculate the shift
//   static int previousShift = 0;
//   int shift = newRangeShift - previousShift;
//   previousShift = newRangeShift;
//   //verify lowest and highest notes are in range
//   int upper = midiNotes[15] + shift;
//   int lower = midiNotes[0] + shift;
//   if (upper <= 127 && lower >= 0) {
//     for (int i = 0; i < 16; i++) {
//       midiNotes[i] += shift;
//     }
//   }
// }