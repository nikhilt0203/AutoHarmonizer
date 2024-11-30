#include "ParameterItem.h"

ParameterItem::ParameterItem(String displayName, int parentMenuNum, int low, int high)
  : MenuItem(displayName, parentMenuNum), low(low), high(high) {
  numOptions = abs(low) + abs(high);
}

void ParameterItem::next() {
  currentIndex++;
  if (currentIndex > high) currentIndex = high;
  select();
}

void ParameterItem::previous() {
  currentIndex--;
  if (currentIndex < low) currentIndex = low;
  select();
}

void ParameterItem::setOption(int option) {
  if (option <= high && option >= low) {
    currentIndex = option;
    select();
  }
}

String ParameterItem::itemType() {
  return "parameter";
}