#include "oledAsciiMenu.h"

void oledAsciiMenu::setupMenu(const char **menuItems, uint8_t count) {
  _count = count;
  _menuItems = menuItems;
  currentSelection = 0;
  menuScreen = 0;
  markerPos = 0;
  menuStartAt = 0;
  _previousStartMenuAt = 1;
  //_count = sizeof(menuItems) / sizeof(char *);
}

void oledAsciiMenu::showMenu(bool force_border_redraw) {
  if (_previousStartMenuAt != menuStartAt || force_border_redraw) {
    _oled->clear();
#if SHOW_BORDER
    printBorder();
#endif
  }
  for (int i = menuStartAt; i < (menuStartAt + MAX_MENU_ITEMS); i++) {
    int markerY = (i - menuStartAt) + MENU_ITEM_Y_OFFSET;

    if (i >= _count) {
      continue;
    }
    if (i == currentSelection) {
      /*
      for (uint8_t i = 0; i < 19; i++)
      {
          _oled->print((char)219);
          _oled->setCol
      }
      */
      _oled->setCursor(MENU_ITEM_X_OFFSET, markerY);
      _oled->print((char)219);
      _oled->setCursor(MENU_ITEM_X_OFFSET + 1, markerY);
      _oled->setInvertMode(true);
      _oled->print(_menuItems[i]);
      _oled->setInvertMode(false);
    } else {
      _oled->setCursor(MENU_ITEM_X_OFFSET, markerY);
      _oled->print(" ");
      _oled->setCursor(MENU_ITEM_X_OFFSET + 1, markerY);
      // _oled->clear(0,1,markerY,markerY);
      // _oled->setCursor(0, markerY);
      _oled->print(_menuItems[i]);
    }
  }
#if SHOW_PROGRESS
  char menuProgress[7];
  sprintf(menuProgress, "%d/%d", currentSelection + 1, _count);
  uint8_t XPosition =
      122;  // Base is 120 for a 6x8 font. 6 is the important number
  uint8_t YPosition = 0;
  XPosition -= strlen(menuProgress) * 5;
#if SHOW_BORDER
  XPosition -= 5;
  YPosition++;
#endif
  _oled->setCursor(XPosition, YPosition);
  _oled->print(menuProgress);
#endif
}

void oledAsciiMenu::moveDown() {
  _previousStartMenuAt = menuStartAt;
  if (currentSelection < _count - 1) {
    currentSelection++;

    if (currentSelection == MAX_MENU_ITEMS + menuStartAt) {
      menuStartAt++;
    }
  } else {
    if (_count > MAX_MENU_ITEMS) {
      // Don't bother refreshing screen if there's not extra items
      _previousStartMenuAt = 1;
    }
    menuStartAt = 0;
    currentSelection = 0;
  }
}

void oledAsciiMenu::moveUp() {
  _previousStartMenuAt = menuStartAt;
  if (currentSelection > 0) {
    currentSelection--;

    if (currentSelection < menuStartAt) {
      menuStartAt--;
    }
  } else {
    if (_count > MAX_MENU_ITEMS) {
      _previousStartMenuAt = 0;
      menuStartAt = _count - MAX_MENU_ITEMS;
    }

    currentSelection = _count - 1;
  }
}

void oledAsciiMenu::printHorizontalBarAtCursor() {
  for (uint8_t i = 0; i < 23; i++) {
    _oled->print((char)205);
  }
}

void oledAsciiMenu::printBorder() {
  _oled->setLetterSpacing(0);

  _oled->setCursor(0, 0);
  _oled->print((char)201);
  printHorizontalBarAtCursor();
  _oled->print((char)187);
  _oled->setCursor(0, 7);
  _oled->print((char)200);
  printHorizontalBarAtCursor();
  _oled->print((char)188);
  for (uint8_t i = 1; i < 7; i++) {
    _oled->setCursor(0, i);
    _oled->print((char)186);
    _oled->setCursor(128 - 8, i);
    _oled->print((char)186);
  }
  _oled->setLetterSpacing(1);
}

// TODO: Find a way to change the scrolling behavior so that it doesn't trigger
// a reflash when moving back up
