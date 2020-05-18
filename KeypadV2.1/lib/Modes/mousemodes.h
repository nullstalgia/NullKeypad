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

bool basicMouseMove(int *_x_velocity, MouseConfig *mouseConfig, bool left_pressed, bool right_pressed, uint8_t physical_button, bool being_released,
                            bool only_reading_value, bool ignore_toggle_check = false);

class MouseMode : public Mode {
 public:
  MouseMode(SSD1306AsciiAvrI2c *Display,
            KeypadButtons *Buttons, uint8_t keyCount = NUM_ALL_BUTTONS,
            uint8_t grid_width = 6, uint8_t grid_height = 8)
      : Mode(Display, Buttons, {}, keyCount, grid_width,
             grid_height) {}
  virtual void modeSetup();
  virtual void modeLoop();
  virtual void modeMenu();
  virtual void modeIsPressed();
  virtual void modeWasPressed();
  virtual void modeWasReleased();

  bool mouseMoving(uint8_t physical_button, bool being_released, bool only_reading_value, bool ignore_toggle_check = false);
  bool borderMouse(uint8_t physical_button, bool being_released, bool only_reading_value, bool ignore_toggle_check = false);

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

