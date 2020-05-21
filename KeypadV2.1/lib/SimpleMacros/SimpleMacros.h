#include <Arduino.h>
#include <HID-Project.h>

#include "KeypadButtons.h"
#include "genericmode.h"

class SimpleMacros : public Mode {
 public:
  SimpleMacros(SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons,
               uint8_t grid_width = 8, uint8_t grid_height = 8)
      : Mode(Display, Buttons, {}, NUM_ALL_BUTTONS, grid_width, grid_height) {}
  virtual void modeSetup();
  virtual void modeLoop();
  virtual void modeMenu();
  // virtual void modeIsPressed();
  virtual void modeWasPressed();
  // virtual void modeWasReleased();

 private:
  bool _kbWiggle;
  bool _mWiggle;
  uint8_t macroPage;
};