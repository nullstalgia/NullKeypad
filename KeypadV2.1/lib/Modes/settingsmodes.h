#pragma once
#include <Arduino.h>
#include "mousemodes.h"
#include "rgb.h"

typedef enum {
  rgbMainMenu = -1,
  rgbMenuBrightness,
  rgbMenuMode,
  rgbMenuOnPush,
  rgbMenuSpeed,
  rgbMenuInfoTest
} rgbMenuNumbers;

void printWrappingLine(SSD1306AsciiAvrI2c *_Display, const char *line);

void printButtonPrompt(SSD1306AsciiAvrI2c *_Display);

class RGBSettingsMode : public Mode {
 public:
  RGBSettingsMode(RGBController *rgb, oledAsciiMenu *menu,
                  SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons)
      : Mode(Display, Buttons, {}),
        _rgb(rgb),
        _menu(menu),
        _rgbMenuItems{"Brightness", "Lighting Mode", "Push Action",
                      "Speed (reset req)", "Test + Info"},
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
                          "Rainbow Reverse",
                          "Rainb. Ribbon Rev",
                          "Rainb. LongRib. Rev"},
        _rgbOnPushMenuItems{"None", "Normal > White", "Normal > Black",
                            "White > Normal", "Black > Normal"} {}
  virtual void modeSetup();
  virtual void modeBackToMain();
  virtual void modeLoop();

 private:
  RGBController *_rgb;
  oledAsciiMenu *_menu;
  int _rgbsubmenu;
  const char *_rgbMenuItems[5];
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
        _menuItems{"Toggle Buttons", "Scroll Wheel",
                   "Border Mouse"},
        _menuDescriptionToggle{ // Weird spacing is due to line-wrapping in both VSCode and in the UI
            "When you press a     mouse btn., it will  stay held until you  press it again."},
        _menuDescriptionWheel{"When holding M3, Up  and Down will act as scroll wheel instead.(Toggleable)"},
        _menuDescriptionBorder{"Replaces layout with the sides/corner buttons being directional(L + R to quickswap)"},
            // ,"Mouse will stay moving until button pressed again." // Toggle Movement Deprecated
        _menuBools{"OFF", "ON "}  // Has to be in FALSE, TRUE order
  {}
  virtual void modeSetup();
  virtual void modeBackToMain();
  virtual void modeLoop();
  void showOption(uint8_t option);
  void showOptionLoop(uint8_t option);

 private:
  oledAsciiMenu *_menu;
  int _mousesubmenu;
  const char *_menuItems[3];
  const char *_menuDescriptionToggle PROGMEM;
  const char *_menuDescriptionWheel PROGMEM;
  const char *_menuDescriptionBorder PROGMEM;
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
        _menuItems{"Toggle Buttons", "WASD Mouse"},
        _menuDescriptionToggle{"When you press a button, it will stay held until it is pressed again."},  // Toggle Buttons
            _menuDescriptionWASDMouse{"When in WASD mode, replaces A and D with Mouse Movements(L + R to quickswap)"},
        _menuBools{"OFF", "ON "}  // Has to be in FALSE, TRUE order
  {}
  virtual void modeSetup();
  virtual void modeBackToMain();
  virtual void modeLoop();
  void showOption(uint8_t option);
  void showOptionLoop(uint8_t option);

 private:
  oledAsciiMenu *_menu;
  int _keyboardsubmenu;
  const char *_menuItems[2];
  const char *_menuDescriptionToggle PROGMEM;
  const char *_menuDescriptionWASDMouse PROGMEM;
  const char *_menuBools[2];
  int _previousMenuHoverSelection;
  KeyboardConfig *_keyboardConfig;
  uint8_t _goBackDownAmount;
};

class SettingsMode : public Mode {
 public:
  SettingsMode(RGBController *rgb, oledAsciiMenu *menu,
               SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons)
      : Mode(Display, Buttons, {}),
        _rgb(rgb),
        _menu(menu),
        _menuItems{"Mouse Settings", "Keyboard Settings", "RGB Settings"} {}
  virtual void modeSetup();
  virtual void modeLoop();

 private:
  RGBController *_rgb;
  oledAsciiMenu *_menu;
  int _submenu;
  const char *_menuItems[3];
  MouseSettingsMode *_mouseMode;
  RGBSettingsMode *_rgbMode;
  KeyboardSettingsMode *_keyboardMode;
};