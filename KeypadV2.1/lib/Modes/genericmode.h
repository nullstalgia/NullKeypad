#pragma once
#include <Arduino.h>
#include "KeypadButtons.h"
//#include "lcdgfx.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include "menu.h"

//void printFixed(SSD1306AsciiAvrI2c *Display, uint8_t x, uint8_t y_row, const char *line);

typedef enum {
  modeNumberMainMenu = -1,
  modeNumberF13,
  modeNumberSettings,
  modeNumberSettingsRGB,
  modeNumberSettingsMouse,
  modeNumberSettingsKeyboard,
  modeNumberMediaKeys,
  modeNumberArrowKeys,
  modeNumberCounter,
  modeNumberMacros,
  modeNumberMouse,
  modeNumberWASD
} modeNumbers;

class Mode {
 public:
  Mode(SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons, const char **KeyLabels,
       bool ablr_buttons = false, uint8_t grid_width = 8,
       uint8_t grid_height = 8)
      : _Display(Display),
        _Buttons(Buttons),
        _keylabels(KeyLabels),
        _ablr_buttons(ablr_buttons),
        _grid_width(grid_width),
        _grid_height(grid_height) {}
  void printInvertingButton(uint8_t x, uint8_t y, const char *text,
                            uint8_t button_index, bool force_on = false);
  void printInvertingButton(uint8_t x, uint8_t y, const char *text,
                            uint8_t button_index, uint8_t grid_width,
                            uint8_t grid_height, bool force_on = false);
  void printInvertingButton(const char *text, uint8_t button_index, bool force_on = false);
  virtual void modeMenu();
  /*
  virtual void modeSetup();
  virtual void modeMenu();
  virtual void modeLoop();
  virtual void modeIsPressed();
  virtual void modeWasPressed();
  virtual void modeWasReleased();
  */

 protected:
  SSD1306AsciiAvrI2c *_Display;
  KeypadButtons *_Buttons;
  const char **_keylabels;
  const bool _ablr_buttons;
  uint8_t _grid_width;
  uint8_t _grid_height;
  
};