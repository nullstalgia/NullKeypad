#include <Arduino.h>
//#include <HID-Project.h>  // https://github.com/NicoHood/HID
//#include <FastLED.h>      // https://github.com/FastLED/FastLED
#include "KeypadButtons.h"
#include "MemoryFree.h"
//#include "lcdgfx.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include "keyboardmodes.h"
#include "menu.h"
#include "mousemodes.h"
#include "oledAsciiMenu.h"
#include "settingsmodes.h"

/* This variable will hold menu state, processed by SSD1306 API functions */

const char* mItemF13 = "F13-21 Keys";
const char* mItemArrow = "Arrow Keys";
const char* mItemWASD = "WASD";
const char* mItemSettings = "Settings";
const char* mItemMedia = "Media Keys";
const char* mItemMouse = "Mouse";

const char* order[] = {mItemWASD,  mItemArrow, mItemMouse,
                       mItemMedia, mItemF13,   mItemSettings};

int currentMode = modeNumberMainMenu;

KeypadButtons buttons;
RGBController rgb;

#define I2C_ADDRESS 0x3C
SSD1306AsciiAvrI2c display;

oledAsciiMenu menu(&display);

KeyboardMode* f13tof21mode;

KeyboardMode* arrowkeysmode;

WASDMode* wasdmode;

SettingsMode* settingsmode;

ConsumerMode* mediakeymode;
MouseMode* mousemode;

void setup() {
  display.begin(&Adafruit128x64, I2C_ADDRESS);
  display.setFont(Adafruit5x7);
  display.clear();



  rgb.init();
  buttons.init();
  uint8_t itemCount = sizeof(order) / sizeof(char*);
  setupMenu(&menu, &display, order, itemCount, buttons.getMenuButton());

  // button = MenuKey::BT_NONE;
}

void loop() {
  buttons.runThroughButtons();
  rgb.loopRGB(buttons.isPressed);

  if (currentMode == modeNumberMainMenu) {
    int changeMode = workMenu(&menu, buttons.getMenuButton(), &display);
    if (changeMode == -1) {
      // currentMode = changeMode;
    } else if (order[changeMode] == mItemSettings) {
      currentMode = modeNumberSettings;
      settingsmode = new SettingsMode(&rgb, &menu, &display, &buttons);
      settingsmode->modeSetup();
    } else if (order[changeMode] == mItemF13) {
      currentMode = modeNumberF13;
      KeyboardConfig kbConfig;
      kbConfig.init();
      if (kbConfig.f24)
      {
        f13tof21mode = new KeyboardMode(F13toF21Labels, F13toF21Buttons, &display,
                                      &buttons, 12);
      } else {
        f13tof21mode = new KeyboardMode(F13toF21Labels, F13toF21Buttons, &display,
                                      &buttons, 9);
      }
      f13tof21mode->modeSetup();
    } else if (order[changeMode] == mItemMedia) {
      currentMode = modeNumberMediaKeys;
      mediakeymode = new ConsumerMode(MediaKeyLabels, MediaKeyButtons, &display,
                                      &buttons, NUM_KEYPAD_BUTTONS, 8);
      mediakeymode->modeSetup();
    } else if (order[changeMode] == mItemMouse) {
      currentMode = modeNumberMouse;
      mousemode = new MouseMode(&display, &buttons);
      mousemode->modeSetup();
    } else if (order[changeMode] == mItemArrow) {
      currentMode = modeNumberArrowKeys;
      arrowkeysmode = new KeyboardMode(ArrowKeyLabels, ArrowKeyButtons,
                                       &display, &buttons, NUM_KEYPAD_BUTTONS);
      arrowkeysmode->modeSetup();
    } else if (order[changeMode] == mItemWASD) {
      currentMode = modeNumberWASD;
      wasdmode = new WASDMode(&display, &buttons);
      wasdmode->modeSetup();
    }
  } else if (currentMode == modeNumberF13) {
    f13tof21mode->modeLoop();
  } else if (currentMode == modeNumberSettings) {
    settingsmode->modeLoop();
  } else if (currentMode == modeNumberMediaKeys) {
    mediakeymode->modeLoop();
  } else if (currentMode == modeNumberMouse) {
    mousemode->modeLoop();
  } else if (currentMode == modeNumberArrowKeys) {
    arrowkeysmode->modeLoop();
  } else if (currentMode == modeNumberWASD) {
    wasdmode->modeLoop();
  }
}
