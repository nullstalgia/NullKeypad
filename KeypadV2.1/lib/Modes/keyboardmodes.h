#pragma once
#include <Arduino.h>
#include <HID-Project.h>  // https://github.com/NicoHood/HID
#include "genericmode.h"
#include "KeypadButtons.h"
#include "lcdgfx.h"
#include "menu.h"
#include "modeconfigs.h"

extern bool activeButtons[13];

bool keyboardButton(const KeyboardKeycode *KeyboardButtons, KeyboardConfig *kbConfig, uint8_t physical_button, bool being_released, bool only_reading_value, bool uses_mouse = false);
bool keyboardButton(const ConsumerKeycode *KeyboardButtons, KeyboardConfig *kbConfig, uint8_t physical_button, bool being_released, bool only_reading_value);

extern const char *F13toF21Labels[];
extern const KeyboardKeycode F13toF21Buttons[];

extern const char *MediaKeyLabels[];
extern const ConsumerKeycode MediaKeyButtons[];

class KeyboardMode : public Mode {
 public:
  KeyboardMode(const char **KeyLabels, const KeyboardKeycode *KeyboardButtons,
               DisplaySSD1306_128x64_I2C *Display, KeypadButtons *Buttons,
               bool ablr_buttons = false, uint8_t grid_width = 8,
               uint8_t grid_height = 8)
      : Mode(Display, Buttons, KeyLabels, ablr_buttons, grid_width, grid_height),
        _keyboardbuttons(KeyboardButtons) {}
  virtual void modeSetup();
  virtual void modeLoop();
  // virtual void modeIsPressed();
  virtual void modeWasPressed();
  virtual void modeWasReleased();
 private:
  const KeyboardKeycode *_keyboardbuttons;
};

class ConsumerMode : public Mode {
 public:
  ConsumerMode(const char **KeyLabels, const ConsumerKeycode *ConsumerButtons,
               DisplaySSD1306_128x64_I2C *Display, KeypadButtons *Buttons,
               bool ablr_buttons = false, uint8_t grid_width = 8,
               uint8_t grid_height = 8)
      : Mode(Display, Buttons, KeyLabels, ablr_buttons, grid_width, grid_height),
        _consumerbuttons(ConsumerButtons) {}
  virtual void modeSetup();
  virtual void modeLoop();
  // virtual void modeIsPressed();
  virtual void modeWasPressed();
  virtual void modeWasReleased();
 private:
  const ConsumerKeycode *_consumerbuttons;
};