#pragma once
#include "MenuItem.h"

/**
 * Represents a single item in the menu that controls an integer parameter.
 */
class ParameterItem : public MenuItem {
public:
  /**
   * @brief constructor for ParameterItem. 
   * @param _displayName the item's displayName
   * @param min the minimum value of the parameter
   * @param max the maximum value of the parameter
   * @param onClick the item's callback function
   */
  ParameterItem(String _displayName, int min, int max, void (*onClick)(int));

  /**
   * @brief increment the parameter.
   */
  void next() override;

  /**
   * @brief decrement the parameter.
   */
  void previous() override;

  /**
   * @brief set the current value and select it
   * @param value the value to set the parameter to
   */
  void setOption(int value) override;

  /**
   * @brief return the min value of the parameter
   * @return min value
   */
  int getMin();

  /**
   * @brief return the max value of the parameter
   * @return max value
   */
  int getMax();

private:
  int min;  //min value of the parameter
  int max;  //max value of the parameter
};