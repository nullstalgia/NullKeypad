#include "keyboardmodes.h"

/*
void KeyboardMode::modeIsPressed() {
  uint8_t keyCount;
  if (_ablr_buttons) {
    keyCount = NUM_ALL_BUTTONS;
  } else {
    keyCount = NUM_KEYPAD_BUTTONS;
  }

  for (uint8_t i = 0; i < keyCount; i++) {
    if(_Buttons->wasPressed[i]){
      Keyboard.press(_keyboardbuttons[i]);
    }
  }
}
*/


const char *F13toF21Labels[] = {"F13", "F14", "F15",  //
                                "F16", "F17", "F18",  //
                                "F19", "F20", "F21"};
const KeyboardKeycode F13toF21Buttons[] = {KEY_F13, KEY_F14, KEY_F15,  //
                                           KEY_F16, KEY_F17, KEY_F18,  //
                                           KEY_F19, KEY_F20, KEY_F21};

const char pause[] = {(char)32,(char)16, (char)186, '\0'};
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





bool activeButtons[13] = {false, false, false, false, false, false, false,
                          false, false, false, false, false, false};

void KeyboardMode::modeSetup() {
  _Display->clear();
  _keyboardConfig = new KeyboardConfig();
  _keyboardConfig->init();
  delay(500);
  //_Buttons->runThroughButtons();
  Keyboard.begin();
}

void KeyboardMode::modeWasPressed() {
  uint8_t keyCount;
  if (_ablr_buttons) {
    keyCount = NUM_ALL_BUTTONS;
  } else {
    keyCount = NUM_KEYPAD_BUTTONS;
  }

  for (uint8_t i = 0; i < keyCount; i++) {
    if (_Buttons->wasPressed[i]) {
      // Keyboard.press(_keyboardbuttons[i]);
      keyboardAction(_keyboardbuttons, _keyboardConfig, i, NOT_RELEASED,
                     USING_BUTTON);
    }
  }
}

void KeyboardMode::modeWasReleased() {
  uint8_t keyCount;
  if (_ablr_buttons) {
    keyCount = NUM_ALL_BUTTONS;
  } else {
    keyCount = NUM_KEYPAD_BUTTONS;
  }

  for (uint8_t i = 0; i < keyCount; i++) {
    if (_Buttons->wasReleased[i]) {
      // Keyboard.release(_keyboardbuttons[i]);
      keyboardAction(_keyboardbuttons, _keyboardConfig, i, RELEASED,
                     USING_BUTTON);
    }
  }
}

void KeyboardMode::modeMenu() {
  uint8_t keyCount;
  if (_ablr_buttons) {
    keyCount = NUM_ALL_BUTTONS;
  } else {
    keyCount = NUM_KEYPAD_BUTTONS;
  }

  for (uint8_t i = 0; i < keyCount; i++) {
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
  modeWasPressed();
  modeWasReleased();

  modeMenu();
}

void ConsumerMode::modeSetup() {
  _Display->clear();
  _keyboardConfig = new KeyboardConfig();
  _keyboardConfig->init();
  delay(500);
  //_Buttons->runThroughButtons();
  Consumer.begin();
}

void ConsumerMode::modeWasPressed() {
  uint8_t keyCount;
  if (_ablr_buttons) {
    keyCount = NUM_ALL_BUTTONS;
  } else {
    keyCount = NUM_KEYPAD_BUTTONS;
  }

  for (uint8_t i = 0; i < keyCount; i++) {
    if (_Buttons->wasPressed[i]) {
      // Keyboard.release(_keyboardbuttons[i]);
      keyboardAction(_consumerbuttons, _keyboardConfig, i, NOT_RELEASED,
                     USING_BUTTON);
    }
  }
}

void ConsumerMode::modeWasReleased() {
  uint8_t keyCount;
  if (_ablr_buttons) {
    keyCount = NUM_ALL_BUTTONS;
  } else {
    keyCount = NUM_KEYPAD_BUTTONS;
  }

  for (uint8_t i = 0; i < keyCount; i++) {
    if (_Buttons->wasReleased[i]) {
      // Keyboard.release(_keyboardbuttons[i]);
      keyboardAction(_consumerbuttons, _keyboardConfig, i, RELEASED,
                     USING_BUTTON);
    }
  }
}

void ConsumerMode::modeMenu() {
  uint8_t keyCount;
  if (_ablr_buttons) {
    keyCount = NUM_ALL_BUTTONS;
  } else {
    keyCount = NUM_KEYPAD_BUTTONS;
  }

  for (uint8_t i = 0; i < keyCount; i++) {
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
  modeWasPressed();
  modeWasReleased();

  modeMenu();
}

bool keyboardAction(const KeyboardKeycode *KeyboardButtons,
                    KeyboardConfig *kbConfig, uint8_t physical_button,
                    bool being_released, bool only_reading_value) {
  uint8_t mouseButton = 254;
  KeyboardKeycode keyboardButton = KEY_ERROR_UNDEFINED;

  if (kbConfig->wasd_mouse == false) {
    keyboardButton = KeyboardButtons[physical_button];
  } else {
    return false;
  }

  if (mouseButton == 254 && keyboardButton == KEY_ERROR_UNDEFINED) {
    return false;
  }

  bool mouse_enabled = kbConfig->wasd_mouse;
  bool toggle_buttons = kbConfig->toggle_buttons;

  // If we're not just ignoring everything to get the current state
  if (!only_reading_value) {
    // Basic toggling. Turns on when being pressed, and will not toggle it off
    // when the button is being released
    if (toggle_buttons && !being_released) {
      if (activeButtons[physical_button]) {
        activeButtons[physical_button] = false;
        Keyboard.release(keyboardButton);
        return false;
      } else {
        activeButtons[physical_button] = true;
        Keyboard.press(keyboardButton);
        return true;
      }
    } else if (toggle_buttons && being_released) {
      // activeButtons[physical_button] = false;
      return false;
    }

    // And just a proper button making a mouse button turn on or off
    if (being_released) {
      activeButtons[physical_button] = false;
      Keyboard.release(keyboardButton);
      return false;
    } else {
      activeButtons[physical_button] = true;
      Keyboard.press(keyboardButton);
      return true;
    }
  } else {
    // But if we're just reading this and not changing anything...
    // Return the state of the wheel if that's what we're here for

    // Otherwise just say the state of the asked button
    return activeButtons[physical_button];
  }

  // If nothing happened, just say false
  return false;
}

bool keyboardAction(const ConsumerKeycode *KeyboardButtons,
                    KeyboardConfig *kbConfig, uint8_t physical_button,
                    bool being_released, bool only_reading_value) {
  ConsumerKeycode consumerButton = CONSUMER_BROWSER_BACK;

  if (kbConfig->wasd_mouse == false) {
    consumerButton = KeyboardButtons[physical_button];
  } else {
    return false;
  }

  if (consumerButton == CONSUMER_BROWSER_BACK) {
    return false;
  }

  bool toggle_buttons = kbConfig->toggle_buttons;

  // If we're not just ignoring everything to get the current state
  if (!only_reading_value) {
    // Basic toggling. Turns on when being pressed, and will not toggle it off
    // when the button is being released
    if (toggle_buttons && !being_released) {
      if (activeButtons[physical_button]) {
        activeButtons[physical_button] = false;
        Consumer.release(consumerButton);
        return false;
      } else {
        activeButtons[physical_button] = true;
        Consumer.press(consumerButton);
        return true;
      }
    } else if (toggle_buttons && being_released) {
      // activeButtons[physical_button] = false;
      return false;
    }

    // And just a proper button making a mouse button turn on or off
    if (being_released) {
      activeButtons[physical_button] = false;
      Consumer.release(consumerButton);
      return false;
    } else {
      activeButtons[physical_button] = true;
      Consumer.press(consumerButton);
      return true;
    }
  } else {
    // But if we're just reading this and not changing anything...
    // Return the state of the wheel if that's what we're here for

    // Otherwise just say the state of the asked button
    return activeButtons[physical_button];
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