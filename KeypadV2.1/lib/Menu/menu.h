#pragma once
#include <Arduino.h>
#include "KeypadButtons.h"
#include "lcdgfx.h"

int workMenu(SAppMenu *menu, MenuKey button, DisplaySSD1306_128x64_I2C *display);

void setupMenu(SAppMenu *menu, DisplaySSD1306_128x64_I2C *display, const char *items[], uint8_t count, MenuKey currentButton, uint8_t autoDownAmount = 0);


