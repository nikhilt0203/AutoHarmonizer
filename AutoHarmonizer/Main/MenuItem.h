#pragma once
#include <Arduino.h>
#include <variant>
#include <vector>

/**
 * Represents a single item in the menu.
 */
class MenuItem {
public:
  //vector containing all created MenuItems
  static std::vector<MenuItem*> allItems;

  //represents the type of the MenuItem
  enum class ItemType {
    Options,
    SubItems,
    Parameter,
    OnOff
  };

public:
  /**
   * @brief delegating constructor. Adds item to allItems vector
   * @param _displayName the item's display name
   * @param onClick the item's callback function
   */
  MenuItem(String _displayName, void (*onClick)(int));

  /**
   * @brief constructor for an item containing options, not other subitems
   * @param _displayName the item's display name
   * @param _menuOptions vector containing the item's options
   * @param onClick the item's callback function
   */
  MenuItem(String _displayName, const std::vector<const char*>& _menuOptions, void (*onClick)(int));

  /**
   * @brief constructor for an item containing subitems
   * @param _displayName the item's display name
   * @param _subItem vector containing the item's subitems
   * @param onClick the item's callback function
   */
  MenuItem(String _displayName, const std::vector<MenuItem*>& _subItems, void (*onClick)(int));

  /**
   * @brief returns the vector containing this item's subitems.
   * @return the subItems vector
   */
  std::vector<MenuItem*> getSubItems();

  /**
   * @brief increment the current option index.
   */
  virtual void next();

  /**
   * @brief decrement the current option index.
   */
  virtual void previous();

  /**
   * @brief select the current option and execute the callback.
   */
  virtual void select();

  /**
   * @brief set the current option of this item.
   */
  virtual void setOption(int option);

  /**
   * @brief set the parentNum (itemNum of the parent item) for this item.
   */
  void setParentNum(int _parentNum);

  /**
   * @brief set the display name of this item.
   */
  void setDisplayName(String newName);

  /**
   * @brief return the index of the next option.
   * @return the index of the next option.
   */
  int nextOptionIndex();

  /**
   * @brief return the index of the previous option.
   * @return the index of the previous option.
   */
  int previousOptionIndex();

  /**
   * @brief return the index of the current option.
   * @return the index of the current option.
   */
  int currentOptionIndex();

  /**
   * @brief return the index of the selected option.
   * @return the index of the selected option.
   */
  int selectedOptionIndex();

  /**
   * @brief return the total number of options that this item has.
   * @return the total number of options.
   */
  int getNumOptions();

  /**
   * @brief return the itemNum of this item (its index in the allItems vector)
   * @return the itemNum.
   */
  int getItemNum() const;

  /**
   * @brief return the parentNum of this item (the itemNum of the parent item).
   * @return the index of the next option.
   */
  int getParentNum();

  /**
   * @brief return the display name of this item.
   * @return the display name.
   */
  String name();

  /**
   * @brief return the name of the current option.
   * @return the name of the current option.
   */
  String currentOptionName();

  /**
   * @brief return the name of the selected option.
   * @return the name of the selected option.
   */
  String selectedOptionName();

  /**
   * @brief return this item's item type.
   * @return the item type.
   */
  ItemType itemType();

protected:
  String displayName;                    //the item's display name
  std::vector<const char*> menuOptions;  //vector containing options
  std::vector<MenuItem*> subItems;       //vector containing other items
  int numOptions = 0;                    //total number of options the item has
  int currentIndex = 0;                  //index of the current option
  int selectedIndex = 0;                 //index of the selected option
  int itemNum;                           //the index of the item in allItems vector
  int parentNum;                         //the index of the parent item in allItems vector, if this item is a subitem
  void (*clickCallback)(int) = nullptr;  //callback executed when an option is chosen
  ItemType type;                         //the type of the item
};