#pragma once
#include "Adafruit_Keypad.h"
#include <Encoder.h>
#include <StevesAwesomeButton.h>

extern const int playButtonPin;
extern const int backButtonPin;
extern const int recordButtonPin;
extern const int encoderButtonPin;

void buttonSetup();
void updateKeypad();
void updateButtons();