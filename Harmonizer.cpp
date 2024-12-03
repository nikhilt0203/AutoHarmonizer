#include "Harmonizer.h"

Harmonizer::Harmonizer(int midiChannel)
  : midiChannel(midiChannel) {
  isActive = false;
}

void Harmonizer::on() {
  isActive = true;
}

void Harmonizer::off() {
  isActive = false;
}

bool Harmonizer::isOn() {
  return isActive;
}

void Harmonizer::setChannel(int newChannel) {
  midiChannel = newChannel;
}

int* Harmonizer::getCurrentChord() {
  return currentChord;
}
