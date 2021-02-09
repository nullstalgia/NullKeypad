#pragma once
#include <Arduino.h>
#include <EEPROM.h>

#include "KeypadSettings.h"

typedef enum {
  mouseMainMenu = -1,
  mouseToggleButtons,
  mouseToggleMovement,
  mouseMouseWheel,
  mouseBorderMouse
} mouseOptionNumbers;

// The struct for EEPROM storage of Mouse configs.
struct MouseConfigStruct {
  bool toggle_buttons;
  bool toggle_movement;
  bool mouse_wheel;
  bool border_mouse;
  byte test_for_new;
};

class MouseConfig {
 public:
  // These control the global RGB and will be written to in setup()
  // when the config is loaded from the EEPROM.
  int min_speed;
  int max_speed;
  bool toggle_buttons;
  bool toggle_movement;
  bool mouse_wheel;
  bool border_mouse;

  void init();
  void loadMouseEEPROM();
  void saveMouseEEPROM();
  void setOption(uint8_t option, bool new_setting);
  bool getOption(uint8_t option);
};

////////////

typedef enum {
  keyboardMainMenu = -1,
  keyboardToggleButtons,
  keyboardWASDMouse,
  keyboardF24
} keyboardOptionNumbers;

// The struct for EEPROM storage of Keyboard configs.
struct KeyboardConfigStruct {
  bool toggle_buttons;
  bool wasd_mouse;
  bool f24;
  byte test_for_new;
};

class KeyboardConfig {
 public:
  int min_speed;
  int max_speed;
  bool toggle_buttons;
  bool wasd_mouse;
  bool f24;

  void init();
  void loadKeyboardEEPROM();
  void saveKeyboardEEPROM();

  void setOption(uint8_t option, bool new_setting);
  bool getOption(uint8_t option);
};

////////////

typedef enum {
  sleepOptionOLED,
  sleepOptionRGB,
  sleepOptionUSB,
  sleepOptionTimer
} sleepOptionNumbers;

// The struct for EEPROM storage of Sleep configs.
struct SleepConfigStruct {
  bool sleepOLED;
  bool sleepRGB;
  bool sleepViaUSB;

  // In seconds
  uint16_t timeIdleForSleep;

  byte test_for_new;
};

class SleepConfig {
 public:
  bool sleepEnabled;
  bool sleepOLED;
  bool sleepRGB;
  bool sleepViaUSB;
  // 0 is no timer.
  // Otherwise, is in seconds
  uint16_t sleepTimer;
  

  void init();
  void loadSleepEEPROM();
  void saveSleepEEPROM();

  void setOption(uint8_t option, uint16_t new_setting);
  uint16_t getOption(uint8_t option);
};