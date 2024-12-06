#include "ParameterItem.h"

ParameterItem::ParameterItem(String _displayName, int min, int max, void (*f)(int))  //subitem controlling a single parameter
  : MenuItem(_displayName, *f), min(min), max(max) {
  type = ItemType::Parameter;
}

void ParameterItem::next() {
  currentIndex++;
  if (currentIndex > max) currentIndex = max;
  select();
}

void ParameterItem::previous() {
  currentIndex--;
  if (currentIndex < min) currentIndex = min;
  select();
}

void ParameterItem::setOption(int option) {
  if (option <= max && option >= min) currentIndex = option;
  select();
  return;
}