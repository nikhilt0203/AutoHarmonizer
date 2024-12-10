#pragma once
#include "MenuItem.h"

/**
 * Represents a single item in the menu that is either ON or OFF.
 */
class OnOffItem : public MenuItem {
public:
  /**
   * @brief constructor for OnOffItem. 
   * @param initialState true to initialize item as ON, false for OFF
   * @param onClick the item's callback function
   */
  OnOffItem(bool initialState, void (*onClick)(int));

  /**
   * @brief toggle and select ON or OFF. 
   */
  void select() override;
};