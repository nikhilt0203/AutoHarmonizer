#include "Adafruit_Keypad.h"
#include <Encoder.h>
#include <StevesAwesomeButton.h>

const int recordButtonPin = 27;
const int playButtonPin = 30;
const int backButtonPin = 26;
const int encoderButtonPin = 38;

int oldPosition = 0;  //old pos of the encoder

const byte ROWS = 4;
const byte COLS = 4;
byte rowPins[ROWS] = { 33, 34, 35, 36 };
byte colPins[COLS] = { 20, 21, 22, 23 };

char keys[ROWS][COLS] = {
  { '0', '1', '2', '3' },
  { '4', '5', '6', '7' },
  { '8', '9', 'a', 'b' },
  { 'c', 'd', 'e', 'f' }
};

Adafruit_Keypad keypad = Adafruit_Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Encoder enc(37, 39);

StevesAwesomeButton encoderButton(encoderButtonPin, 0, INPUT_PULLUP);
StevesAwesomeButton recordButton(recordButtonPin, 0, INPUT);
StevesAwesomeButton playButton(playButtonPin, 1, INPUT);
StevesAwesomeButton backButton(backButtonPin, 2, INPUT);

StevesAwesomeButton* buttons[3] = {
  &recordButton, &playButton, &backButton
};

void setup() {
  Serial.begin(9600);
  buttonSetup();
  ledSetup();
  // menuSetup();
  recorderSetup();
}

void loop() {
  for (int i = 0; i < 3; i++) {
    buttons[i]->process();
  }
  updateEncoder();
  updateKeypad();
}

void buttonSetup() {
  keypad.begin();
  for (int i = 0; i < 3; i++) {
    buttons[i]->pressHandler(buttonRouter);
  }
  //encoderButton.pressHandler(encoderPress);
  pinMode(encoderButtonPin, INPUT_PULLUP);
}

void buttonRouter(int _buttonNum) {
  //Prevent buttons from being pressed simultaneously
  if (!allowButtonInput) return;
  allowButtonInput = false;

  if (_buttonNum == 0) {  //Record button
    recordToggle();
  } else if (_buttonNum == 1) {  //Play button
    playMelody();
  } else if (_buttonNum == 2) {  //Back button
    //previousMenu();
    allowButtonInput = true;
  }
}

void updateEncoder() {
  encoderButton.process();
  int newPosition = enc.read();
  if (newPosition - oldPosition >= 4) {
    oldPosition = newPosition;  // update the encoder's position
    //clickRight();
  }

  if (newPosition - oldPosition <= -4) {
    oldPosition = newPosition;
    //clickLeft();
  }
}

void updateKeypad() {
  keypad.tick();
  while (keypad.available()) {
    keypadEvent e = keypad.read();
    int keyNum = getKeyNum((char)e.bit.KEY);
    if (e.bit.EVENT == KEY_JUST_PRESSED && allowButtonInput) {
      allowButtonInput = false;
      Serial.println(keyNum);
      brightnessIncrease = keyNum;
      recordNoteOn(keyNum);
    } else if (e.bit.EVENT == KEY_JUST_RELEASED)
      recordNoteOff(keyNum);
  }
}

int getKeyNum(char keyPressed) {
  switch (keyPressed) {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a': return 10;
    case 'b': return 11;
    case 'c': return 12;
    case 'd': return 13;
    case 'e': return 14;
    case 'f': return 15;
    default: return -1;
  }
}