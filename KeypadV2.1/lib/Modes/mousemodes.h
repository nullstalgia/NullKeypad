#pragma once
#include <Arduino.h>
#include <HID-Project.h>  // https://github.com/NicoHood/HID
#include "genericmode.h"
#include "modeconfigs.h"

extern const char *MouseLabels[];
extern const char *BorderMouseLabels[];


typedef enum {
  mouse1,
  mouse2,
  mouse3,
  mousewheel = mouse3,
  mouse4,
  mouse5
} mouseButtonIndex;

class MouseMode : public Mode {
 public:
  MouseMode(DisplaySSD1306_128x64_I2C *Display,
            KeypadButtons *Buttons, bool ablr_buttons = false,
            uint8_t grid_width = 6, uint8_t grid_height = 8)
      : Mode(Display, Buttons, {}, ablr_buttons, grid_width,
             grid_height) {}
  virtual void modeSetup();
  virtual void modeLoop();
  virtual void modeMenu();
  virtual void modeIsPressed();
  virtual void modeWasPressed();
  virtual void modeWasReleased();

  void mouseMoving(uint8_t physical_button, bool being_released);
  void borderMouse(uint8_t physical_button, bool being_released);

  bool mouseButton(uint8_t physical_button, bool being_released, bool only_reading_value);

 private:
  MouseConfig *_mouseConfig;
  int _speed;
  signed char _x_velocity;
  signed char _y_velocity;
  signed char _wheel_velocity;
  bool _mouse3_active;
  bool _hotswapBorder;
};

