#include <Arduino.h>
//#include <HID-Project.h>  // https://github.com/NicoHood/HID
//#include <FastLED.h>      // https://github.com/FastLED/FastLED
#include "KeypadButtons.h"
#include "MemoryFree.h"
//#include "lcdgfx.h"
#include "Adafruit5x7_slim.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#include "SimpleMacros.h"
#include "USBAPI.h"
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
const char* mItemMacros = "Simple Macros";

const char* order[7] = {mItemF13,  mItemArrow,  mItemMouse,   mItemMedia,
                        mItemWASD, mItemMacros, mItemSettings};

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

SimpleMacros* macros;

bool usbDisconnected = false;
bool beenConfigured = false;
uint8_t prevUDF = 0;
uint8_t sameUDFCount = 0;

void setup() {
  display.begin(&Adafruit128x64, I2C_ADDRESS);
  display.setFont(Adafruit5x7_slim);
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
      if (kbConfig.f24) {
        f13tof21mode = new KeyboardMode(F13toF21Labels, F13toF21Buttons,
                                        &display, &buttons, 12);
      } else {
        f13tof21mode = new KeyboardMode(F13toF21Labels, F13toF21Buttons,
                                        &display, &buttons, 9);
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
    } else if (order[changeMode] == mItemMacros) {
      currentMode = modeNumberMacros;
      macros = new SimpleMacros(&display, &buttons, 12);
      macros->modeSetup();
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
  } else if (currentMode == modeNumberMacros) {
    macros->modeLoop();
  }

  // Versions without auto RGB shut-off have these last bits removed/commented out.

  if (beenConfigured && usbDisconnected) {
    if (UDFNUML != prevUDF) {
      usbDisconnected = false;
      sameUDFCount = 0;
      rgb.setBrightness(rgb.brightness);
    }
  }
  if (beenConfigured) {
    if (prevUDF == UDFNUML) {
      sameUDFCount++;
      if (sameUDFCount == UDFTIMEOUT_STRIKES) {
        usbDisconnected = true;
        rgb.setBrightness(0);
      }
    }
  } else if (!beenConfigured) {
    beenConfigured = USBDevice.configured();
  }
  /*
    if (beenConfigured) {
      if (prevUDF == UDFNUML) {
        if (!usbDisconnected) {
          sameUDFCount++;
          if (sameUDFCount == UDFTIMEOUT_STRIKES) usbDisconnected = true;
        }
      } else {
        usbDisconnected = false;
        sameUDFCount = 0;
      }

    } else {

    }*/

  prevUDF = UDFNUML;
}
