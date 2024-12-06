#pragma once
#include "MenuItem.h"

class ParameterItem : public MenuItem {
private:
  int min;  //min of the parameter
  int max;  //max of the parameter
public:
  ParameterItem(String _displayName, int min, int max, void (*f)(int));
  void next() override;
  void previous() override;
  void setOption(int option) override;
};