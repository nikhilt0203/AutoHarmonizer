#pragma once
#include "MenuItem.h"

class OnOffItem : public MenuItem {
public:
  OnOffItem(bool initialState, void (*f)(int))  //subitem controlling a single parameter
    : MenuItem(initialState ? "ON" : "OFF", *f) {
    type = ItemType::OnOff;
  }

  void select() override {
    if (displayName.equals("OFF")) {
      setDisplayName("ON");
      clickCallback(1);
    } else {
      setDisplayName("OFF");
      clickCallback(0);
    }
    return;
  }
};
