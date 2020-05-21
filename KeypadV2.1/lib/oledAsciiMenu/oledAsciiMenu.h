#pragma once
#include <Arduino.h>

#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"

#define MAX_MENU_ITEMS 6

#define SHOW_PROGRESS 1

#define SHOW_BORDER 1

#if SHOW_BORDER
#define MENU_ITEM_X_OFFSET 8
#define MENU_ITEM_Y_OFFSET 1
#else
#define MENU_ITEM_X_OFFSET 0
#define MENU_ITEM_Y_OFFSET 0
#endif
class oledAsciiMenu {
 private:
  uint8_t _count;
  int menuStartAt;
  int _previousStartMenuAt;
  SSD1306AsciiAvrI2c *_oled;
  void printHorizontalBarAtCursor();
  const char **_menuItems;

 public:
  oledAsciiMenu(SSD1306AsciiAvrI2c *oled) : _oled(oled) {}
  void moveUp();
  void moveDown();
  void showMenu(bool force_border_redraw = false);
  void setupMenu(const char **menuItems, uint8_t count, bool flipItemOrder = false);
  void printBorder();
  int currentSelection;
  int menuScreen;
  int markerPos;
  private:
  bool _flipItemOrder;
};