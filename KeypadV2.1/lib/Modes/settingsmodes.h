#pragma once
#include <Arduino.h>
#include <avr/wdt.h>

#include "mousemodes.h"
#include "rgb.h"
#define WDTO_250MS 4

void printWrappingLineProgmem(SSD1306AsciiAvrI2c *_Display,
                              const char *signMessage PROGMEM);
void printWrappingLine(SSD1306AsciiAvrI2c *_Display, const char *line);

void printButtonPrompt(SSD1306AsciiAvrI2c *_Display);

typedef enum {
  rgbMainMenu = -1,
  rgbMenuBrightness,
  rgbMenuMode,
  rgbMenuOnPush,
  rgbMenuSpeed,
  rgbMenuInfoTest
} rgbMenuNumbers;

class RGBSettingsMode : public Mode {
 public:
  RGBSettingsMode(RGBController *rgb, oledAsciiMenu *menu,
                  SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons)
      : Mode(Display, Buttons, {}),
        _rgb(rgb),
        _menu(menu),
        _rgbMenuItems{"Back",        "Brightness",        "Lighting Mode",
                      "Push Action", "Speed (reset req)", "Test + Info"},
        _rgbSpeedBrightnessMenuItems{"0", "1", "2", "3", "4", "5",
                                     "6", "7", "8", "9", "10"},
        _rgbModeMenuItems{"None",
                          "White",
                          "Red",
                          "Green",
                          "Blue",
                          "Rainbow",
                          "Rainb. Ribbon",
                          "Rainb. Long Ribbon",
                          "Rainb. Reverse",
                          "Rainb. Rib. Rev",
                          "Rainb. LongRib. Rev"},
        _rgbOnPushMenuItems{"None", "Usr > White", "Usr > Black", "White > Usr",
                            "Black > Usr"} {}
  virtual void modeSetup();
  virtual void modeBackToMain();
  virtual bool modeLoop();

 private:
  RGBController *_rgb;
  oledAsciiMenu *_menu;
  int _rgbsubmenu;
  const char *_rgbMenuItems[6];
  const char *_rgbSpeedBrightnessMenuItems[11];
  const char *_rgbModeMenuItems[11];
  const char *_rgbOnPushMenuItems[5];
  int _previousMenuHoverSelection;
  uint8_t _goBackDownAmount;
};

class MouseSettingsMode : public Mode {
 public:
  MouseSettingsMode(oledAsciiMenu *menu, SSD1306AsciiAvrI2c *Display,
                    KeypadButtons *Buttons)
      : Mode(Display, Buttons, {}),
        _menu(menu),
        _menuItems{"Back", "Toggle Buttons", "Toggle Movement", "Scroll Wheel",
                   "Border Mouse"},
        _menuBools{"OFF", "ON "}  // Has to be in FALSE, TRUE order
  {}
  virtual void modeSetup();
  virtual void modeBackToMain();
  virtual bool modeLoop();
  void showOption(uint8_t option);
  void showOptionLoop(uint8_t option);

 private:
  oledAsciiMenu *_menu;
  int _mousesubmenu;
  const char *_menuItems[5];
  const char *_menuBools[2];
  int _previousMenuHoverSelection;
  MouseConfig *_mouseConfig;
  uint8_t _goBackDownAmount;
};

class KeyboardSettingsMode : public Mode {
 public:
  KeyboardSettingsMode(oledAsciiMenu *menu, SSD1306AsciiAvrI2c *Display,
                       KeypadButtons *Buttons)
      : Mode(Display, Buttons, {}),
        _menu(menu),
        _menuItems{"Back", "Toggle Buttons", "WASD Mouse", "Enable F22-F24"},
        _menuBools{"OFF", "ON "}  // Has to be in FALSE, TRUE order
  {}
  virtual void modeSetup();
  virtual void modeBackToMain();
  virtual bool modeLoop();
  void showOption(uint8_t option);
  void showOptionLoop(uint8_t option);

 private:
  oledAsciiMenu *_menu;
  int _keyboardsubmenu;
  const char *_menuItems[4];
  const char *_menuBools[2];
  int _previousMenuHoverSelection;
  KeyboardConfig *_keyboardConfig;
  uint8_t _goBackDownAmount;
};

typedef enum {
  sleepMainMenu = -1,
  sleepMenuUSBSleep,
  sleepMenuSleepTimer,
  sleepMenuSleepers,
  sleepMenuHelp
} sleepMenuNumbers;

class SleepSettingsMode : public Mode {
 public:
  SleepSettingsMode(oledAsciiMenu *menu, SSD1306AsciiAvrI2c *Display,
                    KeypadButtons *Buttons)
      : Mode(Display, Buttons, {}),
        _menu(menu),
        _sleepMenuItems{"Back", "USB Sleep", "Sleep Timer", "Sleepers", "Help"},
        _sleepTimerMenuItems{"Clear Timer", "15m", "30m", "1h",  "2h", "3h",
                             "4h",          "6h",  "8h",  "10h", "12h"},
        _menuBools{"OFF", "ON "},  // Has to be in FALSE, TRUE order
        _sleeperMenuItems{"RGB", "OLED", "Both"} {}
  virtual void modeSetup();
  virtual void modeBackToMain();
  virtual bool modeLoop();
  void showOption(uint8_t option);
  void showOptionLoop(uint8_t option);

 private:
  oledAsciiMenu *_menu;
  SleepConfig *_sleepConfig;
  int _sleepsubmenu;
  const char *_sleepMenuItems[6];
  const char *_sleepTimerMenuItems[11];
  const char *_sleeperMenuItems[3];
  const char *_menuBools[2];
  int _previousMenuHoverSelection;
  uint8_t _goBackDownAmount;
};

class SettingsMode : public Mode {
 public:
  SettingsMode(RGBController *rgb, oledAsciiMenu *menu,
               SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons)
      : Mode(Display, Buttons, {}),
        _rgb(rgb),
        _menu(menu),
        _menuItems{"Back", "Keyboard Settings", "Mouse ", "RGB "} {}
  virtual void modeSetup();
  virtual void modeLoop();

 private:
  RGBController *_rgb;
  oledAsciiMenu *_menu;
  int _submenu;
  const char *_menuItems[4];
  MouseSettingsMode *_mouseMode;
  RGBSettingsMode *_rgbMode;
  KeyboardSettingsMode *_keyboardMode;
  SleepSettingsMode *_sleepMode;
};