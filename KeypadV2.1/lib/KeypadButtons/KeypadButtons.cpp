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

  const uint8_t pins[NUM_ALL_BUTTONS] = {
      16, 14, 15,

      10, 8,  7,

      6,  5,  4,  // B2, B3, B1, B6, B4, E6, D7, C6, D4

      A3, A2, A1, A0};  // F4, F5, F6, F7

  // RGB Led (pin 9)
  // B5

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

// 0 - isPressed
// 1 - wasPressed
// 2 - wasReleased
// 3 - wasPressed all False, wasReleased all True
void KeypadButtons::clearAction(uint8_t type) {
  for (uint8_t i = 0; i < NUM_ALL_BUTTONS; i++) {
    switch (type) {
      case 0:
        isPressed[i] = false;
        break;

      case 1:
        wasPressed[i] = false;
        break;

      case 2:
        wasReleased[i] = false;
        break;

      case 3:
        //isPressed[i] = false;
        wasPressed[i] = false;
        wasReleased[i] = true;
        break;

      default:
        break;
    }
  }
}

void KeypadButtons::clearActions() {
  clearAction(0);
  clearAction(1);
  clearAction(2);
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
  if (wasPressed[6] || wasPressed[7] || wasPressed[8] || wasPressed[ablrB]) {
    return MenuKey::BT_DOWN;
  } else if (wasPressed[3] || wasPressed[4] || wasPressed[5] ||
             wasPressed[ablrA]) {
    return MenuKey::BT_SELECT;
  }
  return MenuKey::BT_NONE;
}