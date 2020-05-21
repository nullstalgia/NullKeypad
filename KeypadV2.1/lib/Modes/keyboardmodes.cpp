#include "keyboardmodes.h"

/*
void KeyboardMode::modeIsPressed() {
  uint8_t keyCount;
  if (_ablr_buttons) {
    keyCount = NUM_ALL_BUTTONS;
  } else {
    keyCount = NUM_KEYPAD_BUTTONS;
  }

  for (uint8_t i = 0; i < _keyCount; i++) {
    if(_Buttons->wasPressed[i]){
      Keyboard.press(_keyboardbuttons[i]);
    }
  }
}
*/

const char *F13toF21Labels[] = {"F13", "F14", "F15",  //
                                "F16", "F17", "F18",  //
                                "F19", "F20", "F21",  //
                                "F22", "F23", "F24"};
const KeyboardKeycode F13toF21Buttons[] = {KEY_F13, KEY_F14, KEY_F15,  //
                                           KEY_F16, KEY_F17, KEY_F18,  //
                                           KEY_F19, KEY_F20, KEY_F21,  //
                                           KEY_F22, KEY_F23, KEY_F24};

const char pause[] = {(char)32, (char)16, (char)186, '\0'};
const char *MediaKeyLabels[] = {
    "V+",   "Mute", "V-",    //
    "Prev", pause,  "Next",  //
    "Rew",  "Stop", "FF"     //
};

const ConsumerKeycode MediaKeyButtons[] = {
    MEDIA_VOLUME_UP, MEDIA_VOLUME_MUTE, MEDIA_VOLUME_DOWN,   //
    MEDIA_PREVIOUS,  MEDIA_PLAY_PAUSE,  MEDIA_NEXT,          //
    MEDIA_REWIND,    MEDIA_STOP,        MEDIA_FAST_FORWARD,  //
};

const char *ArrowKeyLabels[] = {"Tab", up,     "LAlt",  //
                                left,  down,   right,   //
                                "ESC", "Ctrl", "Enter"};
const KeyboardKeycode ArrowKeyButtons[] = {
    KEY_TAB,        KEY_UP_ARROW,   KEY_LEFT_ALT,     //
    KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW,  //
    KEY_ESC,        KEY_LEFT_CTRL,  KEY_RETURN};

const char *WASDLabels[] = {"E",    "W",  "R",  //
                            "A",    "S",  "D",  //
                            "Shft", "X",  "F",  //
                            "Tab",  " Q", "C", " Ctrl"};
const KeyboardKeycode WASDButtons[] = {KEY_E,          KEY_W, KEY_R,  //
                                       KEY_A,          KEY_S, KEY_D,  //
                                       KEY_LEFT_SHIFT, KEY_X, KEY_F,  //
                                       KEY_TAB,        KEY_Q, KEY_C,
                                       KEY_LEFT_CTRL};

const char *WASDMouseLabels[] = {"E",   "W", "R",    //
                                 left,  "S", right,  //
                                 "M2",  "F", "M1",   //
                                 "Tab", "Q", "Sp-", "Sp+"};

// Function keys are acting as mouse keys.

// F1 - Mouse 1
// F2 - Mouse 2

// F3 - Move Mouse Left
// F4 - Move Mouse Right

// F5 - Speed Minus
// F6 - Speed Plus

const KeyboardKeycode WASDMouseButtons[] = {KEY_E,   KEY_W, KEY_R,   //
                                            KEY_F3,  KEY_S, KEY_F4,  //
                                            KEY_F2,  KEY_F, KEY_F1,  //
                                            KEY_TAB, KEY_Q, KEY_F5, KEY_F6};

bool activeKeyboardButtons[NUM_ALL_BUTTONS] = {
    false, false, false, false, false, false, false,
    false, false, false, false, false, false};

void clearactiveKeyboardButtons() {
  for (uint8_t i = 0; i < NUM_ALL_BUTTONS; i++) {
    activeKeyboardButtons[i] = false;
  }
}

void KeyboardMode::modeSetup() {
  _Display->clear();
  _keyboardConfig = new KeyboardConfig();
  _keyboardConfig->init();
  Keyboard.begin();
  delay(500);
  //_Buttons->runThroughButtons();
}

void KeyboardMode::modeWasPressed() {
  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_Buttons->wasPressed[i]) {
      // Keyboard.press(_keyboardbuttons[i]);
      keyboardAction(_keyboardbuttons, _keyboardConfig, i, NOT_RELEASED,
                     USING_BUTTON);
                     _redraw_menu = true;
    }
  }
}

void KeyboardMode::modeWasReleased() {
  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_Buttons->wasReleased[i]) {
      // Keyboard.release(_keyboardbuttons[i]);
      keyboardAction(_keyboardbuttons, _keyboardConfig, i, RELEASED,
                     USING_BUTTON);
                     _redraw_menu = true;
    }
  }
}

void KeyboardMode::modeMenu() {
  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_keyboardConfig->toggle_buttons) {
      printInvertingButton(_keylabels[i], i,
                           keyboardAction(_keyboardbuttons, _keyboardConfig, i,
                                          RELEASED, ONLY_READING));
    } else {
      printInvertingButton(_keylabels[i], i);
    }
  }
}

void KeyboardMode::modeLoop() {
  _redraw_menu = false;
  modeWasPressed();
  modeWasReleased();
  if(_redraw_menu)
  modeMenu();
}

void ConsumerMode::modeSetup() {
  _Display->clear();
  _keyboardConfig = new KeyboardConfig();
  _keyboardConfig->init();
  Consumer.begin();
  delay(500);
  //_Buttons->runThroughButtons();
}

void ConsumerMode::modeWasPressed() {
  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_Buttons->wasPressed[i]) {
      // Keyboard.release(_keyboardbuttons[i]);
      keyboardAction(_consumerbuttons, _keyboardConfig, i, NOT_RELEASED,
                     USING_BUTTON);
                    _redraw_menu = true;
    }
  }
}

void ConsumerMode::modeWasReleased() {
  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_Buttons->wasReleased[i]) {
      // Keyboard.release(_keyboardbuttons[i]);
      keyboardAction(_consumerbuttons, _keyboardConfig, i, RELEASED,
                     USING_BUTTON);
                     _redraw_menu = true;
    }
  }
}

void ConsumerMode::modeMenu() {
  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_keyboardConfig->toggle_buttons) {
      printInvertingButton(_keylabels[i], i,
                           keyboardAction(_consumerbuttons, _keyboardConfig, i,
                                          RELEASED, ONLY_READING));
    } else {
      printInvertingButton(_keylabels[i], i);
    }
  }
}

void ConsumerMode::modeLoop() {
  _redraw_menu = false;
  modeWasPressed();
  modeWasReleased();

  if(_redraw_menu)
  modeMenu();
}

bool keyboardAction(const KeyboardKeycode *KeyboardButtons,
                    KeyboardConfig *kbConfig, uint8_t physical_button,
                    bool being_released, bool only_reading_value) {
  KeyboardKeycode keyboardButton = KeyboardButtons[physical_button];

  if (keyboardButton == KEY_ERROR_UNDEFINED) {
    return false;
  }

  bool toggle_buttons = kbConfig->toggle_buttons;

  // If we're not just ignoring everything to get the current state
  if (!only_reading_value) {
    // Basic toggling. Turns on when being pressed, and will not toggle it off
    // when the button is being released
    if (toggle_buttons && !being_released) {
      if (activeKeyboardButtons[physical_button]) {
        activeKeyboardButtons[physical_button] = false;
        Keyboard.release(keyboardButton);
        return false;
      } else {
        activeKeyboardButtons[physical_button] = true;
        Keyboard.press(keyboardButton);
        return true;
      }
    } else if (toggle_buttons && being_released) {
      // activeKeyboardButtons[physical_button] = false;
      return false;
    }

    // And just a proper button making a kb button turn on or off
    if (being_released) {
      activeKeyboardButtons[physical_button] = false;
      Keyboard.release(keyboardButton);
      return false;
    } else {
      activeKeyboardButtons[physical_button] = true;
      Keyboard.press(keyboardButton);
      return true;
    }
  } else {
    // But if we're just reading this and not changing anything...
    // Return the state of the wheel if that's what we're here for

    // Otherwise just say the state of the asked button
    return activeKeyboardButtons[physical_button];
  }

  // If nothing happened, just say false
  return false;
}

bool keyboardAction(const ConsumerKeycode *KeyboardButtons,
                    KeyboardConfig *kbConfig, uint8_t physical_button,
                    bool being_released, bool only_reading_value) {
  ConsumerKeycode consumerButton = KeyboardButtons[physical_button];

  bool toggle_buttons = kbConfig->toggle_buttons;

  // If we're not just ignoring everything to get the current state
  if (!only_reading_value) {
    // Basic toggling. Turns on when being pressed, and will not toggle it off
    // when the button is being released
    if (toggle_buttons && !being_released) {
      if (activeKeyboardButtons[physical_button]) {
        activeKeyboardButtons[physical_button] = false;
        Consumer.release(consumerButton);
        return false;
      } else {
        activeKeyboardButtons[physical_button] = true;
        Consumer.press(consumerButton);
        return true;
      }
    } else if (toggle_buttons && being_released) {
      // activeKeyboardButtons[physical_button] = false;
      return false;
    }

    // And just a proper button making a mouse button turn on or off
    if (being_released) {
      activeKeyboardButtons[physical_button] = false;
      Consumer.release(consumerButton);
      return false;
    } else {
      activeKeyboardButtons[physical_button] = true;
      Consumer.press(consumerButton);
      return true;
    }
  } else {
    // But if we're just reading this and not changing anything...
    // Return the state of the wheel if that's what we're here for

    // Otherwise just say the state of the asked button
    return activeKeyboardButtons[physical_button];
  }

  // If nothing happened, just say false
  return false;
}

// Function keys are acting as mouse keys.

// F1 - Mouse 1
// F2 - Mouse 2

// F3 - Move Mouse Left
// F4 - Move Mouse Right

// F5 - Speed Minus
// F6 - Speed Plus

void WASDMode::modeSetup() {
  _Display->clear();
  _keyboardConfig = new KeyboardConfig();
  _keyboardConfig->init();
  _mouseConfig = new MouseConfig();
  _mouseConfig->init();
  _mouse_enabled = _keyboardConfig->wasd_mouse;
  _mouse_speed = 1;
  _x_velocity = 0;
  _hotswapMouse = false;
  Keyboard.begin();
  Mouse.begin();
  delay(1000);
  //_Buttons->runThroughButtons();
}

void WASDMode::modeWasPressed() {
  if (_Buttons->wasPressed[ablrL]) {
    _mouse_speed -= 1;
    if (_mouse_speed < _mouseConfig->min_speed) {
      //_Display->printFixed(0,0,"smol");
      _mouse_speed = _mouseConfig->min_speed;
    }
  }

  if (_Buttons->wasPressed[ablrR]) {
    _mouse_speed += 1;
    if (_mouse_speed > _mouseConfig->max_speed) {
      //_Display->printFixed(0,0,"big");
      _mouse_speed = _mouseConfig->max_speed;
    }
  }
  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_Buttons->wasPressed[i]) {
      if (_mouse_enabled == false) {
        WASDAction(WASDButtons, i, NOT_RELEASED, USING_BUTTON);
      } else {
        WASDAction(WASDMouseButtons, i, NOT_RELEASED, USING_BUTTON);
      }
      _redraw_menu = true;
    }
  }
}

void WASDMode::modeWasReleased() {
  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_Buttons->wasReleased[i]) {
      if (_mouse_enabled == false) {
        WASDAction(WASDButtons, i, RELEASED, USING_BUTTON);
      } else {
        WASDAction(WASDMouseButtons, i, RELEASED, USING_BUTTON);
      }
      _redraw_menu = true;
    }
  }
}

void WASDMode::modeMenu() {
  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_mouse_enabled == false) {
      printInvertingButton(WASDLabels[i], i,
                           WASDAction(WASDButtons, i, RELEASED, ONLY_READING));

    } else {
      printInvertingButton(
          WASDMouseLabels[i], i,
          WASDAction(WASDMouseButtons, i, RELEASED, ONLY_READING));
    }
  }
  _Display->setInvertMode(false);
  if (_mouse_enabled) {
    char buffer[12];
    _Display->setCursor(45, 1);
    sprintf(buffer, "Speed: %02d", _mouse_speed);
    _Display->print(buffer);
  }
  //_Display->setCursor(45, 1);
  // sprintf(buffer, "X: %02d", _x_velocity);
  //_Display->print(buffer);
}

void WASDMode::modeIsPressed() {
  bool current_buttons =
      (_Buttons->isPressed[ablrL] && _Buttons->isPressed[ablrR]);
  if (current_buttons != _hotswapMouse) {
    if (current_buttons) {
      _mouse_enabled = !_mouse_enabled;
      clearactiveKeyboardButtons();
      _x_velocity = 0;
      Keyboard.releaseAll();
      Mouse.releaseAll();
      _Display->clear();
      _redraw_menu = true;
    }
  }
  _hotswapMouse = current_buttons;
}

void WASDMode::modeLoop() {
  _redraw_menu = false;
  modeWasPressed();
  modeWasReleased();
  modeIsPressed();
  if (_redraw_menu) modeMenu();

  if (_x_velocity != 0) {
    Mouse.move(_x_velocity * _mouse_speed, 0);
  }
}

bool WASDMode::WASDAction(const KeyboardKeycode *KeyboardButtons,
                          uint8_t physical_button, bool being_released,
                          bool only_reading_value) {
  KeyboardKeycode keyboardButton = KeyboardButtons[physical_button];

  // Just in case
  if (_mouse_enabled == false && keyboardButton >= KEY_F1 &&
      keyboardButton <= KEY_F12) {
    return false;
  }

  bool toggle_buttons = _keyboardConfig->toggle_buttons;
  bool toggle_movement = _mouseConfig->toggle_movement;

  if (keyboardButton >= KEY_F1 && keyboardButton <= KEY_F12) {
    // Mouse action

    return basicMouseMove(&_x_velocity, _mouseConfig,
                          _Buttons->isPressed[wasdLEFT],
                          _Buttons->isPressed[wasdRIGHT], physical_button,
                          being_released, only_reading_value);

    // return true;

  } else {
    // Keyboard action

    // If we're not just ignoring everything to get the current state
    if (!only_reading_value) {
      // Basic toggling. Turns on when being pressed, and will not toggle it off
      // when the button is being released
      if (toggle_buttons && !being_released) {
        if (activeKeyboardButtons[physical_button]) {
          activeKeyboardButtons[physical_button] = false;
          Keyboard.release(keyboardButton);
          return false;
        } else {
          activeKeyboardButtons[physical_button] = true;
          Keyboard.press(keyboardButton);
          return true;
        }
      } else if (toggle_buttons && being_released) {
        // activeKeyboardButtons[physical_button] = false;
        return false;
      }

      // And just a proper button making a mouse button turn on or off
      if (being_released) {
        activeKeyboardButtons[physical_button] = false;
        Keyboard.release(keyboardButton);
        return false;
      } else {
        activeKeyboardButtons[physical_button] = true;
        Keyboard.press(keyboardButton);
        return true;
      }
    } else {
      // But if we're just reading this and not changing anything...
      // Return the state of the wheel if that's what we're here for

      // Otherwise just say the state of the asked button
      return activeKeyboardButtons[physical_button];
    }
  }

  // If nothing happened, just say false
  return false;
}

/*
mode1.printInvertingButton("F13", 0);
mode1.printInvertingButton("F14", 1);
mode1.printInvertingButton("F15", 2);

mode1.printInvertingButton("F16", 3);
mode1.printInvertingButton("F17", 4);
mode1.printInvertingButton("F18", 5);

mode1.printInvertingButton("F19", 6);
mode1.printInvertingButton("F20", 7);
mode1.printInvertingButton("F21", 8);

mode1.printInvertingButton("AAA", A);
mode1.printInvertingButton("BBB", B);
mode1.printInvertingButton("LLL", L);
mode1.printInvertingButton("RRR", R);
*/