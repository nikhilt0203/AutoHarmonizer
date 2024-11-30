#pragma once
#include <Arduino.h>
#include "MenuItem.h"

class ParameterItem : public MenuItem {
private:
  int low, high;
public:
  ParameterItem(String displayName, int parentMenuNum, int low, int high);
  void next();
  void previous();
  void setOption(int option);
  String itemType();
};