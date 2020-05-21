#include "rgb.h"

void RGBController::setBrightness(uint8_t brightness) {
  FastLED.setBrightness(brightness);
}

void RGBController::init() {
  FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
  gHue = 0;
  loadRGBEEPROM();
  setBrightness(brightness);
  // delay(100);
}

void RGBController::loadRGBEEPROM() {
  // EEPROM Address list in "KeypadSettings.h"
  RGBConfig rgbConfig;

  EEPROM.get(EEPROM_OFFSET_RGB, rgbConfig);

  brightness = rgbConfig.brightness;
  mode = rgbConfig.mode;
  on_push = rgbConfig.on_push;
  speed = rgbConfig.speed;

  /*
  // Good for debugging
  brightness = 10;
  mode = 2;
  on_push = 1;
  speed = 20;
  */

  // In case it was a new device or an invalid config.
  if (brightness == 255 || mode == 255 || on_push == 255 || speed == 255) {
    brightness = 0;
    mode = 0;
    on_push = 0;
    speed = 20;
  }
}

void RGBController::saveRGBEEPROM() {
  RGBConfig rgbConfig;
  rgbConfig.brightness = brightness;
  rgbConfig.mode = mode;
  rgbConfig.on_push = on_push;
  rgbConfig.speed = speed;

  EEPROM.put(1, rgbConfig);
}

void RGBController::loopRGB(bool isPressed[NUM_ALL_BUTTONS]) {
  // Every X milliseconds, it adds to gHue, which in turn changes which part of
  // the rainbow is shown on what LEDs. Essentially, it slowly shifts the
  // rainbow for a rolling effect.
  EVERY_N_MILLISECONDS(speed) { gHue++; };

  switch (mode) {
    case 0:
      fill_solid(leds, NUM_LEDS, CRGB::Black);
      break;

    case 1:
      fill_solid(leds, NUM_LEDS, CRGB::White);
      break;

    case 2:
      fill_solid(leds, NUM_LEDS, CRGB::Red);
      break;

    case 3:
      fill_solid(leds, NUM_LEDS, CRGB::Green);
      break;

    case 4:
      fill_solid(leds, NUM_LEDS, CRGB::Blue);
      break;

    case 5:
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(gHue, 255, 255),
                        CHSV(gHue + 10, 255, 255));
      break;

    case 6:
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(gHue, 255, 255),
                        CHSV(gHue + 70, 255, 255));
      break;

    case 7:
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(gHue, 255, 255),
                        CHSV(gHue + 120, 255, 255));
      break;

    case 8:
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(-gHue, 255, 255),
                        CHSV(-gHue + 10, 255, 255));
      break;

    case 9:
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(-gHue, 255, 255),
                        CHSV(-gHue + 70, 255, 255));
      break;

    case 10:
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(-gHue, 255, 255),
                        CHSV(-gHue + 120, 255, 255));
      break;

    default:
      break;
  }

  // If the on_push option is enabled
  if (on_push != 0) {
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
      // First 2 modes normally show the selected color
      // And change to white or black when pressed
      if (on_push <= 2) {
        if (isPressed[i]) {
          if (on_push == 1) {
            leds[i] = CRGB::White;
          } else {  // on_push == 2
            leds[i] = CRGB::Black;
          }
        }
      } else {
        // Last 2 normally are white or black
        // And show selected color when pressed
        if (on_push == 3) {
          if (!isPressed[i]) {
            leds[i] = CRGB::White;
          }
        } else {  // on_push == 4
          if (!isPressed[i]) {
            leds[i] = CRGB::Black;
          }
        }
      }
    }
  }

  // Done with RGB stuff, send it over to the LEDs.
  // FastLED.setBrightness(0);
  FastLED.show();
}
