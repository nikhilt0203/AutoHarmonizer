#include "Note.h"
#include "Harmonizer.h"

Harmonizer::Harmonizer(int midiChannel)
  : midiChannel(midiChannel), active(false) {
}

void Harmonizer::on() {
  active = true;
}

void Harmonizer::off() {
  active = false;
}

bool Harmonizer::isOn() {
  return active;
}

void Harmonizer::setChannel(int newChannel) {
  midiChannel = newChannel;
}

int* Harmonizer::getCurrentChord() {
  return currentChord;
}