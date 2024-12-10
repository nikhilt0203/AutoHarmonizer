#include "ButtonManager.h"
#include "Keyboard.h"
#include "LEDManager.h"
#include "MenuItem.h"
#include "MenuManager.h"
#include "MelodyRecorder.h"
#include "DisplayManager.h"

void setup() 
{
  buttonSetup();
  keyboardSetup();
  ledSetup(); 
  displaySetup();
  menuSetup();
  melodyRecorderSetup();
}

void loop() 
{
  updateButtons();
  updateKeyboard();
  updateLeds();
}