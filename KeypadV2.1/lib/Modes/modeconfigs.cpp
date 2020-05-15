#include "modeconfigs.h"

void MouseConfig::init() {
  min_speed = 0;
  max_speed = 25;
  loadMouseEEPROM();
}

void MouseConfig::loadMouseEEPROM() {
  // EEPROM Address list in "KeypadSettings.h"
  MouseConfigStruct mouseConfig;

  EEPROM.get(EEPROM_OFFSET_MOUSE, mouseConfig);

  // In case it was a new device or an invalid config.
  if (mouseConfig.test_for_new != EEPROM_TEST_VAL) {
    toggle_buttons = false;
    // toggle_movement = false;
    mouse_wheel = false;
    border_mouse = false;
    saveMouseEEPROM();
  } else {
    // Proper config.
    toggle_buttons = mouseConfig.toggle_buttons;
    // toggle_movement = mouseConfig.toggle_movement;
    mouse_wheel = mouseConfig.mouse_wheel;
    border_mouse = mouseConfig.border_mouse;
  }
}

void MouseConfig::saveMouseEEPROM() {
  MouseConfigStruct mouseConfig;
  mouseConfig.toggle_buttons = toggle_buttons;
  // mouseConfig.toggle_movement = toggle_movement;
  mouseConfig.mouse_wheel = mouse_wheel;
  mouseConfig.border_mouse = border_mouse;
  mouseConfig.test_for_new = EEPROM_TEST_VAL;

  EEPROM.put(EEPROM_OFFSET_MOUSE, mouseConfig);
}

void MouseConfig::setOption(uint8_t option, bool new_setting) {
  switch (option) {
    case mouseToggleButtons:
      toggle_buttons = new_setting;
      break;
    // case mouseToggleMovement:
    // toggle_movement = new_setting;
    // break;
    case mouseMouseWheel:
      mouse_wheel = new_setting;
      break;
    case mouseBorderMouse:
      border_mouse = new_setting;
      break;

    default:
      break;
  }
}

bool MouseConfig::getOption(uint8_t option) {
  switch (option) {
    case mouseToggleButtons:
      return toggle_buttons;
      break;
    // case mouseToggleMovement:
    // return toggle_movement;
    // break;
    case mouseMouseWheel:
      return mouse_wheel;
      break;
    case mouseBorderMouse:
      return border_mouse;
      break;

    default:
      break;
  }
  return false;
}

////////////////////////

void KeyboardConfig::init() {
  min_speed = 0;
  max_speed = 25;
  loadKeyboardEEPROM();
}

void KeyboardConfig::loadKeyboardEEPROM() {
  // EEPROM Address list in "KeypadSettings.h"
  KeyboardConfigStruct keyboardConfig;

  EEPROM.get(EEPROM_OFFSET_KEYBOARD, keyboardConfig);

  // In case it was a new device or an invalid config.
  if (keyboardConfig.test_for_new != EEPROM_TEST_VAL) {
    toggle_buttons = false;
    wasd_mouse = false;
    saveKeyboardEEPROM();
  } else {
    // Proper config.
    toggle_buttons = keyboardConfig.toggle_buttons;
    wasd_mouse = keyboardConfig.wasd_mouse;
  }
}

void KeyboardConfig::saveKeyboardEEPROM() {
  KeyboardConfigStruct keyboardConfig;
  keyboardConfig.toggle_buttons = toggle_buttons;
  keyboardConfig.wasd_mouse = wasd_mouse;
  keyboardConfig.test_for_new = EEPROM_TEST_VAL;

  EEPROM.put(EEPROM_OFFSET_KEYBOARD, keyboardConfig);
}

void KeyboardConfig::setOption(uint8_t option, bool new_setting) {
  switch (option) {
    case keyboardToggleButtons:
      toggle_buttons = new_setting;
      break;
    case keyboardWASDMouse:
      wasd_mouse = new_setting;
      break;

    default:
      break;
  }
}

bool KeyboardConfig::getOption(uint8_t option) {
  switch (option) {
    case keyboardToggleButtons:
      return toggle_buttons;
      break;
    case keyboardWASDMouse:
      return wasd_mouse;
      break;

    default:
      break;
  }
  return false;
}