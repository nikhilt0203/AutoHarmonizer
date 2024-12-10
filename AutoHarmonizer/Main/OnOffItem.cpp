#include "OnOffItem.h"

OnOffItem::OnOffItem(bool initialState, void (*onClick)(int))
  : MenuItem(initialState ? "ON" : "OFF", *onClick)
{
  type = ItemType::OnOff;
}

void OnOffItem::select() 
{
  if (displayName.equals("OFF")) 
  {
    setDisplayName("ON");
    clickCallback(1);
  } 
  else 
  {
    setDisplayName("OFF");
    clickCallback(0);
  }
  return;
}
