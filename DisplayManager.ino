#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void displaySetup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Don't proceed, loop forever
  }
  display.clearDisplay();
}

void displayText(String text, int line, bool isSelected, int size = 1, String prefix = "") {
  if (!text.equals("ON") && !text.equals("OFF")) {
    text.length() <= 2 ? display.setTextSize(4) : display.setTextSize(size);
  } else {
    display.setTextSize(size);
  }

  display.setCursor(0, line > 0 ? (line * 12 - 3) : (line * 12));
  display.setTextColor(SSD1306_WHITE);

  if (!isSelected) {
    display.println(prefix + text);
  } else {
    display.println("> " + text);
  }

  display.display();
}

void displayChord(String chord) {
  if (!harmonizers[DIATONIC]->isOn() && !harmonizers[DIATONIC]->isOn()) return;
  displayChordLeds(); //display chord on LEDs
  display.clearDisplay();

  if (currentHarmonizer == ATONAL) {
    display.setTextSize(2);
    display.setCursor(0, 25);
  } else if (currentHarmonizer == DIATONIC) {
    display.setTextSize(4);
    display.setCursor(66 - chord.length() * 12, 19);
  }
  display.setTextColor(SSD1306_WHITE);
  display.println(chord);
  display.display();
}