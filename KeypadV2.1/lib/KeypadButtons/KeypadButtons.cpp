#include "KeypadButtons.h"

void KeypadButtons::init() {
  for (uint8_t i = 0; i < NUM_ALL_BUTTONS; i++) {
    isPressed[i] = false;
    wasPressed[i] = false;
    wasReleased[i] = false;
  }
  initalizeButtons();
  runThroughButtons();
}

void KeypadButtons::initalizeButtons() {
  /*for (uint8_t i = 0; i < NUM_BUTTONS; i++)
  {
      pinMode(i, INPUT_PULLUP);
  }*/

  uint8_t pins[NUM_ALL_BUTTONS] = {16, 14, 15,

                               10, 8,  7,

                               6,  5,  4,

                               A3, A2, A1, A0};

  for (uint8_t i = 0; i < NUM_ALL_BUTTONS; i++) {
    Buttons[i] = new Button(pins[i], DEBOUNCE_TIME);
    Buttons[i]->begin();
  }
}

void KeypadButtons::runThroughButtons() {
  for (uint8_t i = 0; i < NUM_ALL_BUTTONS; i++) {
    Buttons[i]->read();
    isPressed[i] = Buttons[i]->isPressed();
    wasPressed[i] = Buttons[i]->wasPressed();
    wasReleased[i] = Buttons[i]->wasReleased();
  }
}

MenuKey KeypadButtons::getMenuButton() {
  if (wasPressed[1]) {
    return MenuKey::BT_UP;
  } else if (wasPressed[7]) {
    return MenuKey::BT_DOWN;
  } else if (wasPressed[3]) {
    return MenuKey::BT_LEFT;
  } else if (wasPressed[5]) {
    return MenuKey::BT_RIGHT;
  } else if (wasPressed[4] || wasPressed[ablrA]) {
    return MenuKey::BT_SELECT;
  }
  return MenuKey::BT_NONE;
}

MenuKey KeypadButtons::getToggleButton() {
  if (wasPressed[6] || wasPressed[7] || wasPressed[8] || wasPressed[ablrA]) {
    return MenuKey::BT_SELECT;
  } else if (wasPressed[3] || wasPressed[4] || wasPressed[5] || wasPressed[ablrB]) {
    return MenuKey::BT_DOWN;
  }
  return MenuKey::BT_NONE;
}