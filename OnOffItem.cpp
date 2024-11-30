#include "OnOffItem.h"

OnOffItem::OnOffItem(String initialState, int parentNum)
  : MenuItem(initialState, parentNum) {
  numOptions = 2;
}

String OnOffItem::currentOptionName() {
  return displayName;
}

String OnOffItem::itemType() {
  return "onoff";
}

void OnOffItem::select()  {
  if (displayName.equals("OFF")) {
    changeDisplayName("ON");
    if (pressCallback != nullptr) pressCallback(1);
  } else {
    changeDisplayName("OFF");
    if (pressCallback != nullptr) pressCallback(0);
  }
}