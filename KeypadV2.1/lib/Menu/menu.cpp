#include "menu.h"

static MenuKey menuButton;

// -1 == Nothing chosen
// Otherwise, index of item.
int workMenu(oledAsciiMenu *menu, MenuKey button, SSD1306AsciiAvrI2c *display) {
  uint8_t currentSelection = menu->currentSelection;
  if (button != menuButton) {
    menuButton = button;
    switch (button) {
      case MenuKey::BT_UP:
        menu->moveUp();
        menu->showMenu();
        break;
      case MenuKey::BT_DOWN:
        menu->moveDown();
        menu->showMenu();
        break;
      case MenuKey::BT_SELECT:
        return currentSelection;
        break;
      default:
        break;
    }
  }
  return -1;
}

void setupMenu(oledAsciiMenu *menu, SSD1306AsciiAvrI2c *display,
               const char *items[], uint8_t count, MenuKey currentButton,
               uint8_t autoDownAmount) {
  /* Initialize main menu state */
  menu->setupMenu(items, count);
  /* show menu on the display */
  // delay(100);
  for (uint8_t i = 0; i < autoDownAmount; i++) {
    menu->moveDown();
  }

  menu->showMenu(true);

  // display.menuDown(&menu);
  // display->updateMenuSmooth(menu);
  menuButton = currentButton;
}