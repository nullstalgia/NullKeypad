#pragma once
#include <Arduino.h>
#include <JC_Button.h>
//#include <vector.h>

#include "KeypadSettings.h"

enum class MenuKey : uint8_t {
  BT_NONE = 0,
  BT_RIGHT = 1,
  BT_UP = 2,
  BT_DOWN = 3,
  BT_LEFT = 4,
  BT_SELECT = 5,
};

class KeypadButtons {
 public:
  Button *Buttons[NUM_ALL_BUTTONS];
  void init();
  void initalizeButtons();
  void runThroughButtons();
  bool isPressed[NUM_ALL_BUTTONS];
  bool wasPressed[NUM_ALL_BUTTONS];
  bool wasReleased[NUM_ALL_BUTTONS];
  MenuKey getMenuButton();
  MenuKey getToggleButton();
};