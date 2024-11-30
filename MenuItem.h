#pragma once
#include <Arduino.h>
#include <vector>

class MenuItem {
protected:
  String displayName;
  int itemNum;
  int parentNum;
  std::vector<const char*> menuOptions;
  std::vector<MenuItem*> subItems;
  int numOptions = 0;
  int currentIndex, selectedIndex = 0;
  void (*pressCallback)(int) = nullptr;
public:
  //vector containing all created items
  static std::vector<MenuItem*> menu;
  //"default" constructor
  MenuItem(String displayName, int parentNum);
  //item containing options
  MenuItem(String displayName, int parentNum, const std::vector<const char*>& menuOptions);
  //item containing other items
  MenuItem(String displayName, int parentNum, const std::vector<MenuItem*>& subItems);

  MenuItem* getCurrentItem();                      //return the current item (if item contains other subitems)
  std::vector<MenuItem*> getSubItems();            //return the vector of subitems
  virtual void next();                             //go to next option, or item
  virtual void previous();                         //go to previous option, or item
  virtual void select();                           //select current option and execute its callback function
  virtual int nextOptionIndex();                   //get index of next option
  virtual int previousOptionIndex();               //get index of previous option
  virtual int currentOptionIndex();                //get index of current option
  virtual int getNumOptions();                     //get item's total number of options
  virtual String name();                           //get item's displayName
  virtual String currentOptionName();              //get name of current option, or item
  virtual void onPress(void (*f)(int));            //set item's callback function
  virtual void setOption(int option);              //set item's current option
  virtual void changeDisplayName(String newName);  //change item's displayName
  virtual int getNum();                            //get item's #
  virtual int getParentNum();                      //get the # of the item's parent item
  virtual String itemType();
};