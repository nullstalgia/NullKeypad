#include "SimpleMacros.h"

/*
const char *MacrosPage1[] = {"KB Wake", "F14", "F15",  //
                             "M Wake",  "F17", "F18",  //
                             "F19",     "F20", "F21",  //
                             "",        "",    "<-",  "->"};
*/

void SimpleMacros::modeSetup() {
  _Display->clear();
  Keyboard.begin();
  Mouse.begin();
  _kbWiggle = false;
  _mWiggle = false;
  _M1Spam = false;
  _M2Spam = false;
  _M3Spam = false;
  _macroPage = 0;
  _previousPage = _macroPage;
  _numPages = 3;
  _redraw_menu = false;
  delay(1000);
}

void SimpleMacros::modeLoop() {
  modeWasPressed();
  // modeWasReleased();

  for (uint8_t i = 0; i < NUM_ALL_BUTTONS; i++) {
    if (_Buttons->wasPressed[i] || _Buttons->wasReleased[i]) {
      _redraw_menu = true;
    }
  }
  if (_redraw_menu) modeMenu();
  _previousPage = _macroPage;

  if (_kbWiggle) {
    Keyboard.press(KEY_LEFT_ARROW);
    Keyboard.releaseAll();
    Keyboard.press(KEY_RIGHT_ARROW);
    Keyboard.releaseAll();
  }
  if (_mWiggle) {
    // Mouse.move(5,0);
    Mouse.move(0, -5);
    // Mouse.move(-5,0);
    Mouse.move(0, 5);
  }
  if (_M1Spam) {
    Mouse.press(MOUSE_LEFT);
    delay(1);
    Mouse.release(MOUSE_LEFT);
    delay(1);
  }
  if (_M2Spam) {
    Mouse.press(MOUSE_RIGHT);
    delay(1);
    Mouse.release(MOUSE_RIGHT);
    delay(1);
  }
  if (_M3Spam) {
    Mouse.press(MOUSE_MIDDLE);
    delay(1);
    Mouse.release(MOUSE_MIDDLE);
    delay(1);
  }
}

void SimpleMacros::modeWasPressed() {
  // Speed change
  if (_Buttons->wasPressed[ablrL]) {
    _Display->clear();
    _macroPage -= 1;
    if (_macroPage < 0) {
      //_Display->printFixed(0,0,"smol");
      _macroPage = _numPages - 1;
    }
  }

  if (_Buttons->wasPressed[ablrR]) {
    _Display->clear();
    _macroPage += 1;
    if (_macroPage > _numPages - 1) {
      //_Display->printFixed(0,0,"big");
      _macroPage = 0;
    }
  }

  //////////////////

  if (_macroPage == 0) {
    if (_Buttons->wasPressed[2]) {
      _kbWiggle = !_kbWiggle;
    }
    if (_Buttons->wasPressed[5]) {
      _mWiggle = !_mWiggle;
    }
    if (_Buttons->wasPressed[0]) {
      // Keyboard.print("(っಠ‿ಠ)っ"); didn't work
      _M1Spam = !_M1Spam;
    }
    if (_Buttons->wasPressed[3]) {
      _M2Spam = !_M2Spam;
    }
    if (_Buttons->wasPressed[6]) {
      _M3Spam = !_M3Spam;
    }
  } else if (_macroPage == 1) {     // Arduino
    if (_Buttons->wasPressed[1]) {  // Prog
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press('u');
      Keyboard.releaseAll();
    } else if (_Buttons->wasPressed[4]) {  // Up
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('u');
      Keyboard.releaseAll();
    } else if (_Buttons->wasPressed[7]) {  // Build
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('r');
      Keyboard.releaseAll();
    } else if (_Buttons->wasPressed[5]) {  // Flash Bootloader
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('t');
      Keyboard.releaseAll();
      Keyboard.press(KEY_UP_ARROW);
      Keyboard.releaseAll();
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
    } else if (_Buttons->wasPressed[6]) {  // Build and export binary (hex, actually)
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('s');
      Keyboard.releaseAll();
    }
  } else if (_macroPage == 2) {     // Platform IO
    if (_Buttons->wasPressed[1]) {  // Prog
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press('p');
      Keyboard.releaseAll();
      delay(350);
      Keyboard.println("tasks");
      delay(350);
      Keyboard.println("programmer");
    } else if (_Buttons->wasPressed[4]) {  // Up
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('u');
      Keyboard.releaseAll();
    } else if (_Buttons->wasPressed[6]) {  // Open Serial Terminal
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('s');
      Keyboard.releaseAll();
    } else if (_Buttons->wasPressed[7]) {  // Build
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press('b');
      Keyboard.releaseAll();
    }
  }
}

void SimpleMacros::modeMenu() {
  // printInvertingButton(new_x, new_y, text, button_index, force_on);  for
  // (uint8_t i = 0; i < _keyCount; i++) {
  //    printMacroButton(MacrosPage1[i], i);
  //}
  if (_macroPage == 0) {
    printInvertingButton(6, 3, "KB Wake", 2, _kbWiggle);
    printInvertingButton(6, 5, "M Wake", 5, _mWiggle);
    printInvertingButton(0, 3, "M1 Spam", 0, _M1Spam);
    printInvertingButton(0, 5, "M2 Spam", 3, _M2Spam);
    printInvertingButton(0, 7, "M3 Spam", 6, _M3Spam);
  } else if (_macroPage == 1 || _macroPage == 2) {
    printInvertingButton(2, 3, "Upload w/ Prog", 1);
    printInvertingButton(4, 5, "Upload", 4);
    printInvertingButton(4, 7, "Build", 7);
    if (_macroPage == 1) {
      printInvertingButton(8, 5, "Bootl", 5);
      printInvertingButton(1, 7, "Bin", 6);
    } else {
      printInvertingButton(1, 7, "Term", 6);
    }
    _Display->setInvertMode(false);
    _Display->setCursor(105, 0);
    if (_macroPage == 1) {
      _Display->print("Ard");
    } else {
      _Display->print("PIO");
    }
  }
  //

  printInvertingButton(0, 0, "<--", 11);
  printInvertingButton(2, 0, "-->", 12);
  char buffer[9];
  _Display->setInvertMode(false);
  // sprintf(buffer, "Page: %02d", _macroPage+1);
  sprintf(buffer, "Page: %d", _macroPage + 1);
  _Display->setCursor(50, 0);
  _Display->print(buffer);
}

/*
void SimpleMacros::printMacroButton(const char *text, uint8_t button_index,
                                    bool force_on) {
  uint8_t new_x, new_y;

  if (button_index < 9) {
    // Assuming 3x3 grid without ABLR buttons

    // Getting X
    new_x = button_index % 3;

    new_x *= 4;

    // new_x += 3;

    // Getting Y
    new_y = button_index / 3;

    new_y *= 3;

    new_y++;

    for (uint8_t i = (button_index / 3); i < 2; i++) {
      new_y++;
    }

  } else {
    return;
  }

  printInvertingButton(new_x, new_y, text, button_index, force_on);
}*/