#include <Arduino.h>
#include <HID-Project.h>

#include "KeypadButtons.h"
#include "genericmode.h"

class SimpleMacros : public Mode {
 public:
  SimpleMacros(SSD1306AsciiAvrI2c *Display, KeypadButtons *Buttons,
               uint8_t grid_width = 8, uint8_t grid_height = 8)
      : Mode(Display, Buttons, {}, NUM_KEYPAD_BUTTONS, grid_width, grid_height) {}
  virtual void modeSetup();
  virtual void modeLoop();
  virtual void modeMenu();
  // virtual void modeIsPressed();
  virtual void modeWasPressed();
  //void printMacroButton(const char *text, uint8_t button_index,
                            //bool force_on = false);
  // virtual void modeWasReleased();

 private:
  bool _kbWiggle;
  bool _mWiggle;
  int _macroPage;
  int _previousPage;
  uint8_t _numPages;
};