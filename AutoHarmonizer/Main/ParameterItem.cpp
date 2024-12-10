#include "ParameterItem.h"

ParameterItem::ParameterItem(String _displayName, int min, int max, void (*onClick)(int))
  : MenuItem(_displayName, *onClick), min(min), max(max) 
{
  type = ItemType::Parameter;
}

void ParameterItem::next() 
{
  currentIndex++;
  if (currentIndex > max) currentIndex = max;
  select();
}

void ParameterItem::previous()
{
  currentIndex--;
  if (currentIndex < min) currentIndex = min;
  select();
}

void ParameterItem::setOption(int value) 
{
  if (value <= max && value >= min) currentIndex = value;
  select();
  return;
}

int ParameterItem::getMin() 
{
  return min;
}

int ParameterItem::getMax() 
{
  return max;
}