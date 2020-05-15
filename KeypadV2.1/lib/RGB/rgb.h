#pragma once
#include <Arduino.h>
#include <EEPROM.h>
#include <FastLED.h>
#include "genericmode.h"
#include "KeypadButtons.h"
#include "lcdgfx.h"
#include "menu.h"
#include "KeypadSettings.h"

#define NUM_LEDS 9
#define DATA_PIN 9
#define LED_TYPE NEOPIXEL

// The struct for EEPROM storage of RGB configs.
struct RGBConfig {
  byte brightness;
  byte mode;
  byte on_push;
  byte speed;
};

class RGBController {
 public:
  // These control the global RGB and will be written to in setup()
  // when the config is loaded from the EEPROM.
  byte brightness;
  byte mode;
  byte on_push;
  byte speed;

  // This is used for the rolling rainbow effect
  uint8_t gHue;

  // Array holding all LED states. Written to the strip on FastLED.show()
  CRGB leds[NUM_LEDS];

  void init();
  void loadRGBEEPROM();
  void saveRGBEEPROM();
  void loopRGB(bool isPressed[]);
  void setBrightness(uint8_t brightness);
};