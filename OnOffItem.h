#pragma once
#include <Arduino.h>
#include "MenuItem.h"

class OnOffItem : public MenuItem {
public:
  OnOffItem(String initialState, int parentNum);
  String currentOptionName();
  String itemType();
  void select();
};