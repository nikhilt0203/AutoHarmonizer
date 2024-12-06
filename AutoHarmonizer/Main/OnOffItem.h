#pragma once
#include "MenuItem.h"

class OnOffItem : public MenuItem {
public:
  OnOffItem(bool initialState, void (*f)(int));
  void select() override;
};
