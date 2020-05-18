#pragma once
#include <Arduino.h>
#include <HID-Project.h>  // https://github.com/NicoHood/HID

#include "KeypadButtons.h"
#include "genericmode.h"
//#include "lcdgfx.h"

#include "menu.h"
#include "modeconfigs.h"
#include "mousemodes.h"



bool keyboardAction(const KeyboardKeycode *KeyboardButtons,
                    KeyboardConfig *kbConfig, uint8_t physical_button,
                    bool being_released, bool only_reading_value);
bool keyboardAction(const ConsumerKeycode *KeyboardButtons,
                    KeyboardConfig *kbConfig, uint8_t physical_button,
                    bool being_released, bool only_reading_value);
bool WASDAction(const KeyboardKeycode *KeyboardButtons,
                KeyboardConfig *kbConfig, MouseConfig *mouseConfig, uint8_t physical_button,
                bool being_released, bool only_reading_value,
                bool mouse_enabled);

extern const char *F13toF21Labels[];
extern const KeyboardKeycode F13toF21Buttons[];

extern const char *MediaKeyLabels[];
extern const ConsumerKeycode MediaKeyButtons[];

extern const char *ArrowKeyLabels[];
extern const KeyboardKeycode ArrowKeyButtons[];

class KeyboardMode : public Mode {
 public:
  KeyboardMode(const char **KeyLabels, const KeyboardKeycode *KeyboardButtons,
               SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons,
               uint8_t keyCount = NUM_KEYPAD_BUTTONS, uint8_t grid_width = 8,
               uint8_t grid_height = 8)
      : Mode(Display, Buttons, KeyLabels, keyCount, grid_width,
             grid_height),
        _keyboardbuttons(KeyboardButtons) {}
  virtual void modeSetup();
  virtual void modeLoop();
  virtual void modeMenu();
  // virtual void modeIsPressed();
  virtual void modeWasPressed();
  virtual void modeWasReleased();

 private:
  const KeyboardKeycode *_keyboardbuttons;
  KeyboardConfig *_keyboardConfig;
};

class ConsumerMode : public Mode {
 public:
  ConsumerMode(const char **KeyLabels, const ConsumerKeycode *ConsumerButtons,
               SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons,
               uint8_t keyCount = NUM_KEYPAD_BUTTONS, uint8_t grid_width = 8,
               uint8_t grid_height = 8)
      : Mode(Display, Buttons, KeyLabels, keyCount, grid_width,
             grid_height),
        _consumerbuttons(ConsumerButtons) {}
  virtual void modeSetup();
  virtual void modeLoop();
  virtual void modeMenu();
  // virtual void modeIsPressed();
  virtual void modeWasPressed();
  virtual void modeWasReleased();

 private:
  const ConsumerKeycode *_consumerbuttons;
  KeyboardConfig *_keyboardConfig;
};

class WASDMode : public Mode {
 public:
  WASDMode(SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons, uint8_t grid_width = 8,
           uint8_t grid_height = 8)
      : Mode(Display, Buttons, {}, NUM_ALL_BUTTONS, grid_width, grid_height) {}
  virtual void modeSetup();
  virtual void modeLoop();
  virtual void modeMenu();
  // virtual void modeIsPressed();
  virtual void modeWasPressed();
  virtual void modeWasReleased();

  bool WASDAction(const KeyboardKeycode *KeyboardButtons, uint8_t physical_button,
                bool being_released, bool only_reading_value);

 private:
  KeyboardConfig *_keyboardConfig;
  MouseConfig *_mouseConfig;
  bool _mouse_enabled;
  int _mouse_speed;
  int _x_velocity;
};