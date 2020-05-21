#include "SimpleMacros.h"

const char *MacrosPage1[] = {"KB Wake", "F14", "F15",  //
                                "M Wake", "F17", "F18",  //
                                "F19", "F20", "F21",  //
                                "", "", "<-", "->"};

void SimpleMacros::modeSetup() {
    _Display->clear();
  Keyboard.begin();
  Mouse.begin();
  _kbWiggle = false;
  _mWiggle = false;
  macroPage = 0;
  delay(1000);
}

void SimpleMacros::modeLoop() {
  modeWasPressed();
  //modeWasReleased();
  modeMenu();
}

void SimpleMacros::modeWasPressed() {
  
}

void SimpleMacros::modeMenu() {
  for (uint8_t i = 0; i < _keyCount; i++) {
      printInvertingButton(MacrosPage1[i], i);
  }
}
