#include "MenuItem.h"

//initialize static vector containing all created MenuItems
std::vector<MenuItem*> MenuItem::allItems = {};

MenuItem::MenuItem(String _displayName, void (*onClick)(int))
  : displayName(_displayName), clickCallback(onClick) 
{
  allItems.push_back(this);       //add newly created item to static vector allItem
  itemNum = allItems.size() - 1;  //set the itemNum based on the item's index in allItems vector
}

MenuItem::MenuItem(String _displayName, const std::vector<const char*>& _menuOptions, void (*onClick)(int))  //item containing options
  : MenuItem(_displayName, onClick) 
{
  numOptions = _menuOptions.size();
  menuOptions = _menuOptions;
}

MenuItem::MenuItem(String _displayName, const std::vector<MenuItem*>& _subItems, void (*onClick)(int))  //item containing other subitems
  : MenuItem(_displayName, onClick) 
{
  numOptions = _subItems.size();
  subItems = _subItems;
  //set the parentNum of this item's subitems to the itemNum of this item
  for (MenuItem* item : subItems) 
  {
    item->setParentNum(this->itemNum);
  }
}

std::vector<MenuItem*> MenuItem::getSubItems() 
{
  return subItems;
}

void MenuItem::next() 
{
  currentIndex = (currentIndex + 1) % numOptions;
}

void MenuItem::previous() 
{
  currentIndex = (currentIndex - 1 + numOptions) % numOptions;
}

void MenuItem::select() 
{
  selectedIndex = currentIndex;
  if (clickCallback != nullptr) clickCallback(currentIndex);
}

void MenuItem::setOption(int option) 
{
  if (option >= 0 && option < numOptions) currentIndex = option;
}

void MenuItem::setParentNum(int _parentNum) 
{
  parentNum = _parentNum;
}

void MenuItem::setDisplayName(String newName) 
{
  displayName = newName;
}

int MenuItem::nextOptionIndex() 
{
  next();
  return currentIndex;
}

int MenuItem::previousOptionIndex() 
{
  previous();
  return currentIndex;
}

int MenuItem::currentOptionIndex() 
{
  return currentIndex;
}

int MenuItem::selectedOptionIndex() 
{
  return selectedIndex;
}

int MenuItem::getNumOptions() 
{
  return numOptions;
}

int MenuItem::getItemNum() const 
{
  return itemNum;
}

int MenuItem::getParentNum() 
{
  return parentNum;
}

String MenuItem::name() 
{
  return displayName;
}

String MenuItem::currentOptionName() 
{
  if (subItems.empty()) return menuOptions[currentIndex];
  return subItems[currentIndex]->name();
}

String MenuItem::selectedOptionName() 
{
  if (subItems.empty()) return menuOptions[selectedIndex];
  return subItems[selectedIndex]->name();
}

MenuItem::ItemType MenuItem::itemType() 
{
  return type;
}