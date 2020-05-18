#include "mousemodes.h"

bool activeMouseButtons[NUM_ALL_BUTTONS] = {false, false, false, false, false,
                                            false, false, false, false, false,
                                            false, false, false};

void clearactiveMouseButtons() {
  for (uint8_t i = 0; i < NUM_ALL_BUTTONS; i++) {
    activeMouseButtons[i] = false;
  }
}

const char *MouseLabels[] = {"M1", up,   "M2", left, down,  right, "M4",
                             "MW", "M5", "M1", "M2", "Sp-", "Sp+"};

const char *BorderMouseLabels[] = {"  ", up,   "  ", left, "M1",  right, "  ",
                                   down, "  ", "M1", "M2", "Sp-", "Sp+"};

void MouseMode::modeSetup() {
  _speed = 1;
  _x_velocity = 0;
  _y_velocity = 0;
  _wheel_velocity = 0;

  _mouse3_active = false;

  _hotswapBorder = false;

  _mouseConfig = new MouseConfig;
  _mouseConfig->init();
  _Display->clear();
  Mouse.begin();
}

void MouseMode::modeMenu() {
  char buffer[12];
  _Display->setInvertMode(false);
  sprintf(buffer, "Speed: %02d", _speed);
  _Display->setCursor(45, 1);
  _Display->print(buffer);

  uint8_t keyCount = NUM_ALL_BUTTONS;

  for (uint8_t i = 0; i < _keyCount; i++) {
    if (_mouseConfig->border_mouse == false) {
      if (i < ablrL) {
        bool buttonValue = (mouseButton(i, NOT_RELEASED, ONLY_READING) ||
                            mouseMoving(i, NOT_RELEASED, ONLY_READING));
        printInvertingButton(MouseLabels[i], i, buttonValue);
      } else {
        printInvertingButton(MouseLabels[i], i);
      }
    } else {
      if (i < ablrL) {
        bool buttonValue = (mouseButton(i, NOT_RELEASED, ONLY_READING) ||
                            borderMouse(i, NOT_RELEASED, ONLY_READING));
        printInvertingButton(BorderMouseLabels[i], i, buttonValue);
      } else {
        printInvertingButton(BorderMouseLabels[i], i);
      }
    }
  }
  /*
  sprintf(buffer, "X:%02d", _x_velocity);
  _Display->printFixed(0, 16, buffer);
  sprintf(buffer, "Y:%02d", _y_velocity);
  _Display->printFixed(0, 24, buffer);
  sprintf(buffer, "W:%02d", _wheel_velocity);
  _Display->printFixed(0, 32, buffer);
  */
}

void MouseMode::modeWasReleased() {
  for (uint8_t i = 0; i < NUM_ALL_BUTTONS; i++) {
    if (_Buttons->wasReleased[i]) {
      mouseButton(i, RELEASED, USING_BUTTON);
      if (_mouseConfig->border_mouse) {
        borderMouse(i, RELEASED, USING_BUTTON);
      } else {
        mouseMoving(i, RELEASED, USING_BUTTON);
      }
      _redraw_menu = true;
    }
  }
}

void MouseMode::modeWasPressed() {
  // Speed change
  if (_Buttons->wasPressed[ablrL]) {
    _speed -= 1;
    if (_speed < _mouseConfig->min_speed) {
      //_Display->printFixed(0,0,"smol");
      _speed = _mouseConfig->min_speed;
    }
  }

  if (_Buttons->wasPressed[ablrR]) {
    _speed += 1;
    if (_speed > _mouseConfig->max_speed) {
      //_Display->printFixed(0,0,"big");
      _speed = _mouseConfig->max_speed;
    }
  }

  for (uint8_t i = 0; i < NUM_ALL_BUTTONS; i++) {
    if (_Buttons->wasPressed[i]) {
      mouseButton(i, NOT_RELEASED, USING_BUTTON);
      if (_mouseConfig->border_mouse) {
        borderMouse(i, NOT_RELEASED, USING_BUTTON);
      } else {
        mouseMoving(i, NOT_RELEASED, USING_BUTTON);
      }
      _redraw_menu = true;
    }
  }
}

void MouseMode::modeIsPressed() {
  bool current_buttons =
      (_Buttons->isPressed[ablrL] && _Buttons->isPressed[ablrR]);
  if (current_buttons != _hotswapBorder) {
    if (current_buttons) {
      _mouseConfig->border_mouse = !_mouseConfig->border_mouse;
      clearactiveMouseButtons();
      _x_velocity = 0;
      _y_velocity = 0;
      _wheel_velocity = 0;
    }
  }
  _hotswapBorder = current_buttons;
}

void MouseMode::modeLoop() {
  _redraw_menu = false;
  //_x_velocity = 0;
  //_y_velocity = 0;
  //_wheel_velocity = 0;
  modeWasReleased();
  modeWasPressed();
  modeIsPressed();
  if (_redraw_menu) modeMenu();

  if (_x_velocity != 0 || _y_velocity != 0 || _wheel_velocity != 0) {
    Mouse.move(_x_velocity * _speed, _y_velocity * _speed);
    if (_wheel_velocity != 0) {
      Mouse.move(0, 0, _wheel_velocity);

      // Prevents it from going too fast
      if (_speed != 0) delay(20 / _speed);
    }
  }
}
// Ran every time a mouse key is pressed

bool MouseMode::mouseButton(uint8_t physical_button, bool being_released,
                            bool only_reading_value) {
  uint8_t mouseButton = 254;

  if (_mouseConfig->border_mouse == false) {
    if (physical_button == 0 || physical_button == ablrA) {
      mouseButton = MOUSE_LEFT;
    } else if (physical_button == 2 || physical_button == ablrB) {
      mouseButton = MOUSE_RIGHT;
    } else if (physical_button == 6) {
      mouseButton = MOUSE_PREV;
    } else if (physical_button == 8) {
      mouseButton = MOUSE_NEXT;
    } else if (physical_button == 7) {
      mouseButton = MOUSE_MIDDLE;
    }
  } else {
    if (physical_button == 4 || physical_button == ablrA) {
      mouseButton = MOUSE_LEFT;
    } else if (physical_button == ablrB) {
      mouseButton = MOUSE_RIGHT;
    }
  }

  if (mouseButton == 254) {
    return false;
  }

  bool mouse_wheel_enabled = _mouseConfig->mouse_wheel;
  bool toggle_buttons = _mouseConfig->toggle_buttons;

  // If we're not just ignoring everything to get the current state
  if (!only_reading_value) {
    // We check to see if we're dealing with the special Middle Mouse key
    if (mouseButton == MOUSE_MIDDLE) {
      // If the wheel is enabled at all
      if (mouse_wheel_enabled) {
        // We gotta see if we're going to change it
        // If toggle is enabled and we're not in the wasReleased
        if (toggle_buttons && !being_released) {
          // Just toggle it and move on.
          _mouse3_active = !_mouse3_active;
          return _mouse3_active;
        } else if (toggle_buttons && being_released) {
          // If we're in the release and we're toggling, ignore this as to not
          // turn it back on accidentally.
          return false;
        } else {
          // But if the wheel is enabled but we're not toggling
          // Disactivate if releasing, activate if not releasing
          if (being_released) {
            _mouse3_active = false;
          } else {
            _mouse3_active = true;
          }
          return _mouse3_active;
        }
      }
    }
    // Done with middle mouse for now

    // Basic toggling. Turns on when being pressed, and will not toggle it off
    // when the button is being released
    if (toggle_buttons && !being_released) {
      if (Mouse.isPressed(mouseButton)) {
        Mouse.release(mouseButton);
        return false;
      } else {
        Mouse.press(mouseButton);
        return true;
      }
    } else if (toggle_buttons && being_released) {
      return false;
    }

    // And just a proper button making a mouse button turn on or off
    if (being_released) {
      Mouse.release(mouseButton);
      return false;
    } else {
      Mouse.press(mouseButton);
      return true;
    }
  } else {
    // But if we're just reading this and not changing anything...
    // Return the state of the wheel if that's what we're here for
    if (mouseButton == MOUSE_MIDDLE) {
      if (mouse_wheel_enabled) {
        return _mouse3_active;
      }
    }

    // Otherwise just say the state of the asked button
    return Mouse.isPressed(mouseButton);
  }

  // If nothing happened, just say false
  return false;
}

/*
// Quick thing to get the requested button in case it's not a 0 1 2 layout
char keyToMouseButton(int key) {
  if (key == M2) {
    return MOUSE_RIGHT;
  } else if (key == M3) {
    return MOUSE_MIDDLE;
  } else if (key == M4) {
    return MOUSE_NEXT;
  } else if (key == M5) {
    return MOUSE_PREV;
  }
  return MOUSE_LEFT;
}
*/

bool MouseMode::mouseMoving(uint8_t physical_button, bool being_released,
                            bool only_reading_value, bool ignore_toggle_check) {
  // Counter 0 is X
  // Counter 1 is Y

  if (!(physical_button == wasdUP || physical_button == wasdDOWN ||
        physical_button == wasdLEFT || physical_button == wasdRIGHT)) {
    return false;
  }

  if (_mouseConfig->toggle_movement == false && only_reading_value) {
    return false;
  } else if (_mouseConfig->toggle_movement && only_reading_value) {
    return activeMouseButtons[physical_button];
  }

  if (ignore_toggle_check == false) {
    if (_mouseConfig->toggle_movement) {
      if (being_released) {
        return false;
      } else {
        if (activeMouseButtons[physical_button]) {
          activeMouseButtons[physical_button] = false;
          mouseMoving(physical_button, RELEASED, USING_BUTTON, true);
          return false;
        } else {
          activeMouseButtons[physical_button] = true;
          mouseMoving(physical_button, NOT_RELEASED, USING_BUTTON, true);
          return true;
        }
      }
    }
  }

  // Just used to make it a little nicer on the eyes
  // If we weren't called in the wasReleased
  if (!being_released) {
    // And if we're going up or down
    if (physical_button == wasdUP || physical_button == wasdDOWN) {
      if (_mouseConfig->mouse_wheel &&
          mouseButton(7, NOT_RELEASED, ONLY_READING)) {
        _y_velocity = 0;
        if (_wheel_velocity == 0) {
          if (physical_button == wasdUP) {
            _wheel_velocity += 1;
            return false;
          } else if (physical_button == wasdDOWN) {
            _wheel_velocity -= 1;
            return false;
          }
        } else {
          _wheel_velocity = 0;
          mouseMoving(physical_button, NOT_RELEASED, USING_BUTTON);
        }
      } else {
        // AND if it wasn't already moving...
        if (_y_velocity == 0) {
          // ...just set the movement
          if (physical_button == wasdUP) {
            _y_velocity -= 1;
            return false;
          } else if (physical_button == wasdDOWN) {
            _y_velocity += 1;
            return false;
          }
        } else {
          // But if we were already moving
          // Make it think we weren't, and rerun the function to change the
          // direction instantly Even if the old button is still being held
          _y_velocity = 0;
          mouseMoving(physical_button, NOT_RELEASED, USING_BUTTON);
        }
      }
    } else if (physical_button == wasdLEFT || physical_button == wasdRIGHT) {
      if (_x_velocity == 0) {
        if (physical_button == wasdLEFT) {
          _x_velocity -= 1;
          return false;
        } else if (physical_button == wasdRIGHT) {
          _x_velocity += 1;
        }
      } else {
        _x_velocity = 0;
        mouseMoving(physical_button, NOT_RELEASED, USING_BUTTON);
      }
    }
  } else {
    // But if a key was released, we don't want to just stop the mouse moving
    // instantly So we check to see which direction we *should* be going based
    // on what still being held And rerun the function to keep the direction
    if (physical_button == wasdUP && _Buttons->isPressed[wasdDOWN]) {
      _y_velocity = 0;
      _wheel_velocity = 0;
      mouseMoving(wasdDOWN, NOT_RELEASED, USING_BUTTON);
      return false;
    }
    if (physical_button == wasdDOWN && _Buttons->isPressed[wasdUP]) {
      _y_velocity = 0;
      _wheel_velocity = 0;
      mouseMoving(wasdUP, NOT_RELEASED, USING_BUTTON);
      return false;
    }
    if (physical_button == wasdLEFT && _Buttons->isPressed[wasdRIGHT]) {
      _x_velocity = 0;
      mouseMoving(wasdRIGHT, NOT_RELEASED, USING_BUTTON);
      return false;
    }
    if (physical_button == wasdRIGHT && _Buttons->isPressed[wasdLEFT]) {
      _x_velocity = 0;
      mouseMoving(wasdLEFT, NOT_RELEASED, USING_BUTTON);
      return false;
    }
    if (physical_button == wasdUP || physical_button == wasdDOWN) {
      _y_velocity = 0;
      _wheel_velocity = 0;
      return false;
    }
    if (physical_button == wasdLEFT || physical_button == wasdRIGHT) {
      _x_velocity = 0;
      return false;
    }
  }
  return false;
}

bool MouseMode::borderMouse(uint8_t physical_button, bool being_released,
                            bool only_reading_value, bool ignore_toggle_check) {
  // If it's the middle key (Mouse 1), or the side keys, then ignore this and
  // move on
  if (physical_button == 4 || physical_button >= ablrA) {
    return false;
  }

  if (_mouseConfig->toggle_movement == false && only_reading_value) {
    return false;
  } else if (_mouseConfig->toggle_movement && only_reading_value) {
    return activeMouseButtons[physical_button];
  }

  if (ignore_toggle_check == false) {
    if (_mouseConfig->toggle_movement) {
      if (being_released) {
        return false;
      } else {
        if (activeMouseButtons[physical_button]) {
          activeMouseButtons[physical_button] = false;
          borderMouse(physical_button, RELEASED, USING_BUTTON, true);
          return false;
        } else {
          activeMouseButtons[physical_button] = true;
          borderMouse(physical_button, NOT_RELEASED, USING_BUTTON, true);
          return true;
        }
      }
    }
  }

  if (!being_released) {
    // If we're not already moving the mouse
    if (true) {
      // Keys 1 through 3 (Top edge)
      if (physical_button < 3) {
        _y_velocity -= 1;
      }
      // Keys 6 through 8 (Bottom edge)
      if (physical_button > 5) {
        _y_velocity += 1;
      }
      // Keys 0, 3, 6 (Left edge)
      if (physical_button % 3 == 0) {
        _x_velocity -= 1;
      }
      // Keys 2, 5, 8 (Right edge)
      if ((physical_button + 1) % 3 == 0) {
        _x_velocity += 1;
      }
    } else {
      // !! Now impossible state. !!
      // If mouse is moving, stop it and redo this as to not re-add speed
      _x_velocity = 0;
      _y_velocity = 0;
      borderMouse(physical_button, NOT_RELEASED, USING_BUTTON);
    }
  } else {
    // Reversing the above action if a button is let go
    // Keys 1 through 3 (Top edge)
    if (physical_button < 3) {
      _y_velocity += 1;
    }
    // Keys 6 through 8 (Bottom edge)
    if (physical_button > 5) {
      _y_velocity -= 1;
    }
    // Keys 0, 3, 6 (Left edge)
    if (physical_button % 3 == 0) {
      _x_velocity += 1;
    }
    // Keys 2, 5, 8 (Right edge)
    if ((physical_button + 1) % 3 == 0) {
      _x_velocity -= 1;
    }

    // Leftovers.
    /*
    // Checking to see which one is being pushed and then pushing it
    // TODO: Maybe try this on mouseMoving?
    for (int i = 0; i < 9; i++) {
      if (i == 4) {
        continue;
      }
      if (isPressed[i]) {
        borderMouse(i, false);
        break;
      }
    }
     */
  }
  return false;
}

bool basicMouseMove(int *_x_velocity, MouseConfig *mouseConfig,
                    bool left_pressed, bool right_pressed,
                    uint8_t physical_button, bool being_released,
                    bool only_reading_value, bool ignore_toggle_check) {
  // Counter 0 is X
  // Counter 1 is Y

  if (!(physical_button == wasdLEFT || physical_button == wasdRIGHT ||
        physical_button == 6 || physical_button == 8)) {
    return false;
  }

  // bool toggle_movement = mouseConfig->toggle_movement;
  bool toggle_movement = false;

  if (toggle_movement == false && only_reading_value) {
    return false;
  } else if (toggle_movement && only_reading_value) {
    return activeMouseButtons[physical_button];
  }

  if (physical_button == 6 || physical_button == 8) {
    uint8_t mouseButton;
    if (physical_button == 6) {
      mouseButton = MOUSE_RIGHT;
    } else {
      mouseButton = MOUSE_LEFT;
    }
    bool toggle_mouse_buttons = mouseConfig->toggle_buttons;
    if (!only_reading_value) {
      if (toggle_mouse_buttons && !being_released) {
        if (Mouse.isPressed(mouseButton)) {
          Mouse.release(mouseButton);
          return false;
        } else {
          Mouse.press(mouseButton);
          return true;
        }
      } else if (toggle_mouse_buttons && being_released) {
        return false;
      }
      if (being_released) {
        Mouse.release(mouseButton);
        return false;
      } else {
        Mouse.press(mouseButton);
        return true;
      }
    } else {
      return Mouse.isPressed(mouseButton);
    }
    return false;
  }

  if (ignore_toggle_check == false) {
    if (toggle_movement) {
      if (being_released) {
        return false;
      } else {
        if (activeMouseButtons[physical_button]) {
          activeMouseButtons[physical_button] = false;
          basicMouseMove(_x_velocity, mouseConfig, left_pressed, right_pressed,
                         physical_button, RELEASED, USING_BUTTON, true);
          return false;
        } else {
          activeMouseButtons[physical_button] = true;
          basicMouseMove(_x_velocity, mouseConfig, left_pressed, right_pressed,
                         physical_button, NOT_RELEASED, USING_BUTTON, true);
          return true;
        }
      }
    }
  }

  // Just used to make it a little nicer on the eyes
  // If we weren't called in the wasReleased
  if (!being_released) {
    // And if we're going up or down
    if (physical_button == wasdLEFT || physical_button == wasdRIGHT) {
      if (*_x_velocity == 0) {
        if (physical_button == wasdLEFT) {
          *_x_velocity -= 1;
          return false;
        } else if (physical_button == wasdRIGHT) {
          *_x_velocity += 1;
        }
      } else {
        *_x_velocity = 0;
        basicMouseMove(_x_velocity, mouseConfig, left_pressed, right_pressed,
                       physical_button, NOT_RELEASED, USING_BUTTON);
      }
    }
  } else {
    // But if a key was released, we don't want to just stop the mouse moving
    // instantly So we check to see which direction we *should* be going based
    // on what still being held And rerun the function to keep the direction
    if (physical_button == wasdLEFT && right_pressed) {
      *_x_velocity = 0;
      basicMouseMove(_x_velocity, mouseConfig, left_pressed, right_pressed,
                     wasdRIGHT, NOT_RELEASED, USING_BUTTON);
      return false;
    }
    if (physical_button == wasdRIGHT && left_pressed) {
      *_x_velocity = 0;
      basicMouseMove(_x_velocity, mouseConfig, left_pressed, right_pressed,
                     wasdLEFT, NOT_RELEASED, USING_BUTTON);
      return false;
    }
    if (physical_button == wasdLEFT || physical_button == wasdRIGHT) {
      *_x_velocity = 0;
      return false;
    }
  }
  return false;
}