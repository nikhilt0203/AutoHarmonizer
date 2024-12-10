#include "ButtonManager.h"
#include <Encoder.h>
#include <StevesAwesomeButton.h>
#include "MenuManager.h"
#include "MelodyRecorder.h"

const int recordButtonPin = 27;
const int playButtonPin = 30;
const int backButtonPin = 26;
const int encoderButtonPin = 38;

StevesAwesomeButton encoderButton = StevesAwesomeButton(encoderButtonPin, 0, INPUT_PULLUP);
StevesAwesomeButton recordButton = StevesAwesomeButton(recordButtonPin, 0, INPUT);
StevesAwesomeButton playButton = StevesAwesomeButton(playButtonPin, 1, INPUT);
StevesAwesomeButton backButton = StevesAwesomeButton(backButtonPin, 2, INPUT);

StevesAwesomeButton* buttons[3] = {
  &recordButton, &playButton, &backButton
};

Encoder enc = Encoder(37, 39);

void buttonRouter(int _buttonNum) 
{
  //Prevent buttons from being pressed simultaneously
  if (!allowButtonInput) return;
  allowButtonInput = false;

  if (_buttonNum == 0) //Record button
  {
    recordToggle();
  } 
  else if (_buttonNum == 1) //Play button
  {
    playMelody();
  } 
  else if (_buttonNum == 2) //Back button
  {
    previousMenu();
    allowButtonInput = true;
  }
}

void buttonSetup() 
{
  for (int i = 0; i < 3; i++) 
  {
    buttons[i]->pressHandler(buttonRouter);
  }
  encoderButton.pressHandler(encoderPress);
  pinMode(encoderButtonPin, INPUT_PULLUP);
}

void updateEncoder() 
{
  encoderButton.process();

  static int oldPosition = 0;
  int newPosition = enc.read();
  if (newPosition - oldPosition >= 4) 
  {
    oldPosition = newPosition;
    clickRight();
  } 
  else if (newPosition - oldPosition <= -4) 
  {
    oldPosition = newPosition;
    clickLeft();
  }
}

void updateButtons() 
{
  for (int i = 0; i < 3; i++) 
  {
    buttons[i]->process();
  }
  updateEncoder();
}