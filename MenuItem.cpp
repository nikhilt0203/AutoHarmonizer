#include "MenuItem.h"

std::vector<MenuItem*> MenuItem::menu;

MenuItem::MenuItem(String displayName, int parentNum)
  : displayName(displayName), parentNum(parentNum) {
  menu.push_back(this);
  itemNum = menu.size() - 1;
}

MenuItem::MenuItem(String displayName, int parentNum, const std::vector<const char*>& menuOptions)
  : MenuItem::MenuItem(displayName, parentNum) {
  this->menuOptions = menuOptions;
  numOptions = menuOptions.size();
}

MenuItem::MenuItem(String displayName, int parentNum, const std::vector<MenuItem*>& subItems)
  : MenuItem(displayName, parentNum) {
  numOptions = subItems.size();
  this->subItems = subItems;
}

MenuItem* MenuItem::getCurrentItem() {
  return subItems[currentIndex];
}

std::vector<MenuItem*> MenuItem::getSubItems() {
  return subItems;
}

void MenuItem::next() {
  currentIndex = (currentIndex + 1) % numOptions;
}

void MenuItem::previous() {
  currentIndex = (currentIndex - 1 + numOptions) % numOptions;
}

int MenuItem::nextOptionIndex() {
  next();
  return currentIndex;
}

int MenuItem::previousOptionIndex() {
  previous();
  return currentIndex;
}

int MenuItem::currentOptionIndex() {
  return currentIndex;
}

String MenuItem::name() {
  return displayName;
}

String MenuItem::currentOptionName() {
  if (subItems.empty()) return menuOptions[currentIndex];
  return subItems[currentIndex]->name();
}

int MenuItem::getNumOptions() {
  return numOptions;
}

void MenuItem::select() {
  selectedIndex = currentIndex;
  if (pressCallback != nullptr) pressCallback(currentIndex);
}

void MenuItem::onPress(void (*f)(int)) {
  pressCallback = f;
}

void MenuItem::setOption(int option) {
  if (option >= 0 && option < numOptions) currentIndex = option;
}

void MenuItem::changeDisplayName(String newName) {
  displayName = newName;
}

int MenuItem::getNum() {
  return itemNum;
}

int MenuItem::getParentNum() {
  return parentNum;
}

String MenuItem::itemType() {
  if (subItems.empty()) return "options";
  return "subitems";
}