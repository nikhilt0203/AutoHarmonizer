#pragma once
#include <Arduino.h>
#include <variant>
#include <vector>

class MenuItem {
public:
  //vector containing all created items
  static std::vector<MenuItem*> allItems;

  enum class ItemType {
    Options,
    SubItems,
    Parameter,
    OnOff
  };

protected:
  String displayName;                    //the item's display name
  std::vector<const char*> menuOptions;  //vector containing options
  std::vector<MenuItem*> subItems;       //vector containing other items
  int numOptions = 0;                    //total number of options the item has
  int currentIndex = 0;                  //index of the current option
  int selectedIndex = 0;                 //index of the selected option
  int itemNum;                           //initialized to the index of the item in the allItems vector
  int parentNum;                         //the itemNum of the parent item, if this item is a subitem
  void (*clickCallback)(int) = nullptr;  //callback executed when an option is chosen
  ItemType type;                         //the type of the item

public:
  MenuItem(String _displayName, void (*onClick)(int));
  MenuItem(String _displayName, const std::vector<const char*>& _menuOptions, void (*onClick)(int));
  MenuItem(String _displayName, const std::vector<MenuItem*>& _subItems, void (*onClick)(int));
  std::vector<MenuItem*> getSubItems();
  virtual void next();
  virtual void previous();
  virtual void select();
  virtual void setOption(int option);
  void setParentNum(int _parentNum);
  void setDisplayName(String newName);
  int nextOptionIndex();
  int previousOptionIndex();
  int currentOptionIndex();
  int getNumOptions();
  int getItemNum() const;
  int getParentNum();
  String name();
  String currentOptionName();
  String selectedOptionName();
  ItemType itemType();
};