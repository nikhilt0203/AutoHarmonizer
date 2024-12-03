#pragma once
#include <Arduino.h>

void displaySetup();

void displayText(String text, int line, bool isSelected, int size = 1, String prefix = "");

void displayChord(String chord);

void clearDisplay();