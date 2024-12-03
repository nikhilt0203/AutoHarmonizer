#pragma once
#include "MenuItem.h"

class ParameterItem : public MenuItem {
private:
  int min;  //min of the parameter
  int max;  //max of the parameter
public:
  ParameterItem(String _displayName, int min, int max, void (*f)(int))  //subitem controlling a single parameter
    : MenuItem(_displayName, *f), min(min), max(max) {
    type = ItemType::Parameter;
  }

  void next() {
    currentIndex++;
    if (currentIndex > max) currentIndex = max;
    select();
  }

  void previous() {
    currentIndex--;
    if (currentIndex < min) currentIndex = min;
    select();
  }

  void setOption(int option) {
    if (option <= max && option >= min) currentIndex = option;
    select();
    return;
  }
};