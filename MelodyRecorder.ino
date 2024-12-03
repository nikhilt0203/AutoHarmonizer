#include "ButtonManager.h"
#include "LEDManager.h"
#include "MenuItem.h"
#include "MenuManager.h"
#include "MelodyRecorder.h"
#include "DisplayManager.h"

void setup() {
  buttonSetup();
  ledSetup(); 
  menuSetup();
  displaySetup();
  recorderSetup();
}

void loop() {
  updateButtons();
}