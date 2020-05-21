#pragma once
#include <Arduino.h>

#include "KeypadButtons.h"
//#include "lcdgfx.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include "oledAsciiMenu.h"

int workMenu(oledAsciiMenu *menu, MenuKey button, SSD1306AsciiAvrI2c *display);

void setupMenu(oledAsciiMenu *menu, SSD1306AsciiAvrI2c *display,
               const char *items[], uint8_t count, MenuKey currentButton,
               uint8_t autoDownAmount = 0, bool flipItemOrder = false);
