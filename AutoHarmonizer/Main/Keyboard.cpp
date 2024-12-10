#include "Keyboard.h"
#include "MelodyRecorder.h"

const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = { 33, 34, 35, 36 };
byte colPins[COLS] = { 20, 21, 22, 23 };
int litUpKey = -1;

char keys[ROWS][COLS] = {
  { '0', '1', '2', '3' },
  { '4', '5', '6', '7' },
  { '8', '9', 'a', 'b' },
  { 'c', 'd', 'e', 'f' }
};

//MIDI note values corresponding to keypad keys
int midiNotes[16] = {
  60, 61, 62, 63,
  64, 65, 66, 67,
  68, 69, 70, 71,
  72, 73, 74, 75
};

Adafruit_Keypad keyboard = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void keyboardSetup() 
{
  keyboard.begin();
}

int getKeyNum(char keyPressed) 
{
  //calculate key # from ASCII value of its char identifier
  if (keyPressed <= '9') 
  {
    return keyPressed - '0';
  } 
  else if (keyPressed <= 'f') 
  {
    return keyPressed - 'a' + 10;
  } 
  else return -1;
}

void updateKeyboard() 
{
  keyboard.tick();
  while (keyboard.available()) 
  {
    keypadEvent e = keyboard.read();
    int keyNum = getKeyNum((char)e.bit.KEY);
    if (e.bit.EVENT == KEY_JUST_PRESSED && allowButtonInput) 
    {
      allowButtonInput = false;
      recordNoteOn(midiNotes[keyNum]);
      litUpKey = keyNum;
      delay(5);
    } 
    else if (e.bit.EVENT == KEY_JUST_RELEASED) 
    {
      recordNoteOff(midiNotes[keyNum]);
      litUpKey = -1;
      delay(5);
    }
  }
}

void changeKeyboardRange(int newRangeShift) 
{
  if (!allowButtonInput) return;

  static int previousRangeShift = 0;
  int shift = newRangeShift - previousRangeShift;
  previousRangeShift = newRangeShift;

  int newHighestNote = midiNotes[15] + shift;
  int newLowestNote = midiNotes[0] + shift;

  if (newHighestNote > 127 || newLowestNote < 0) 
  {
    Serial.println(String("Error: Range shift ") + shift + " results in invalid notes " + newHighestNote + " or " + newLowestNote + ".");
    return;
  }

  for (int i = 0; i < 16; i++) 
  {
      midiNotes[i] += shift;
  }
}

int getShift() 
{
  return midiNotes[0] - 60;
}