#include "menu.h"

static MenuKey menuButton;

static char menuProgess[10];

static int menuCount;

// -1 == Nothing chosen
// Otherwise, index of item.
int workMenu(SAppMenu *menu, MenuKey button,
             DisplaySSD1306_128x64_I2C *display) {
  uint8_t currentSelection = display->menuSelection(menu);
  sprintf(menuProgess, "%d/%d", currentSelection + 1, menuCount);
  uint8_t XPosition = 120; // Base is 120 for a 6x8 font. 6 is the important number
  XPosition -= strlen(menuProgess)*6;
  display->printFixed(XPosition, 8, menuProgess, STYLE_NORMAL);
  if (button != menuButton) {
    menuButton = button;
    switch (button) {
      case MenuKey::BT_UP:
        display->menuUp(menu);
        display->updateMenuSmooth(menu);
        break;
      case MenuKey::BT_DOWN:
        display->menuDown(menu);
        display->updateMenuSmooth(menu);
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

void setupMenu(SAppMenu *menu, DisplaySSD1306_128x64_I2C *display,
               const char *items[], uint8_t count, MenuKey currentButton, uint8_t autoDownAmount) {
  menuCount = count;
  /* Initialize main menu state */
  display->createMenu(menu, items, count);
  /* show menu on the display */
  // delay(100);
  display->showMenuSmooth(menu);
  for (uint8_t i = 0; i < autoDownAmount; i++)
  {
    display->menuDown(menu);
  }

  if(autoDownAmount > 0){
    display->updateMenuSmooth(menu);
  }

  // display.menuDown(&menu);
  // display->updateMenuSmooth(menu);
  menuButton = currentButton;
}