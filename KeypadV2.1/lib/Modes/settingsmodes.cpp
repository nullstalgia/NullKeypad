#include "settingsmodes.h"

const char _menuDescriptionToggleMouseButton[] PROGMEM =
    {  // Weird spacing is due to line-wrapping in both VSCode and in the UI
        "When you press a     mouse btn., it will  stay held until you  press "
        "it again."};
const char _menuDescriptionToggleMovement[] PROGMEM = {
    "Mouse will stay moving until button pressed again."};
const char _menuDescriptionWheel[] PROGMEM = {
    "When holding M3, Up  and Down will act as scroll wheel "
    "instead.(Toggleable)"};
const char _menuDescriptionBorder[] PROGMEM = {
    "Replaces layout with the sides/corner buttons being directional(L + R to "
    "quickswap)"};

const char _menuDescriptionToggleKBButton[] PROGMEM = {
    "When you press a button, it will stay held until it is pressed "
    "again. Try B + < or >"};  // Toggle Buttons
const char _menuDescriptionWASDMouse[] PROGMEM = {
    "When in WASD mode, replaces A and D with Mouse Movements(L + R to "
    "quickswap)"};
const char _menuDescriptionF24[] PROGMEM = {
    "Enable extra Function keys in F13+ mode"};

const char _rgbInfoTest[] PROGMEM = {
    "Brightness:\nLighting Mode:\nPush Action:\nRGB Speed:"};
const char _rgbInfoSpeed[] PROGMEM = {
    "A higher \"Speed value\" means a slower moving rainbow"};

const char _buttonPrompt[] PROGMEM = {
    "A/Middle Row: Toggle\nB/Bottom Row: Back"};

const char _menuDescriptionUSBSleep[] PROGMEM = {
    "If the keypad detects the computer turning off/sleeping "
    "it will trigger sleep if enabled."};

const char _menuDescriptionSleepHelp[] PROGMEM = {
    "If the keypad detects the computer turning off/sleeping "
    "it will trigger sleep if enabled."};

void SettingsMode::modeSetup() {
  _Display->clear();
  _submenu = modeNumberSettings;
  uint8_t itemCount = sizeof(_menuItems) / sizeof(char *);
  MenuKey menuButton = _Buttons->getMenuButton();
  setupMenu(_menu, _Display, _menuItems, itemCount, menuButton);
}

void SettingsMode::modeLoop() {
  //_Display->clear();

  MenuKey menuButton;
  int changeMenu;

  if (_submenu == modeNumberSettings) {
    menuButton = _Buttons->getMenuButton();
    changeMenu = workMenu(_menu, menuButton, _Display);
  }

  // Settings Main Menu
  if (_submenu == modeNumberSettings) {
    if (changeMenu != -1) {
      // If they choose an option
      if (changeMenu == 0) {
        wdt_enable(WDTO_15MS);
        while (true)
          ;
      } else if (changeMenu == 1) {
        _submenu = modeNumberSettingsKeyboard;
        _keyboardMode = new KeyboardSettingsMode(_menu, _Display, _Buttons);
        _keyboardMode->modeSetup();
      } else if (changeMenu == 2) {
        _submenu = modeNumberSettingsMouse;
        _mouseMode = new MouseSettingsMode(_menu, _Display, _Buttons);
        _mouseMode->modeSetup();
      } else if (changeMenu == 3) {
        _submenu = modeNumberSettingsRGB;
        _rgbMode = new RGBSettingsMode(_rgb, _menu, _Display, _Buttons);
        _rgbMode->modeSetup();
      } else if (changeMenu == 4) {
        _submenu = modeNumberSettingsSleep;
        _sleepMode = new SleepSettingsMode(_menu, _Display, _Buttons);
        _sleepMode->modeSetup();
      }
    }
  } else if (_submenu == modeNumberSettingsMouse) {
    if (_mouseMode->modeLoop()) {
      delete _mouseMode;
      modeSetup();
    }
  } else if (_submenu == modeNumberSettingsRGB) {
    if (_rgbMode->modeLoop()) {
      delete _rgbMode;
      modeSetup();
    }
  } else if (_submenu == modeNumberSettingsKeyboard) {
    if (_keyboardMode->modeLoop()) {
      delete _keyboardMode;
      modeSetup();
    }
  } else if (_submenu == modeNumberSettingsSleep) {
    if (_sleepMode->modeLoop()) {
      delete _sleepMode;
      modeSetup();
    }
  }
}

void MouseSettingsMode::modeSetup() {
  _mouseConfig = new MouseConfig();
  _mouseConfig->init();
  _goBackDownAmount = 1;
  modeBackToMain();
}

void MouseSettingsMode::modeBackToMain() {
  _Display->setInvertMode(false);
  _Display->clear();
  _mousesubmenu = mouseMainMenu;
  uint8_t itemCount = sizeof(_menuItems) / sizeof(char *);
  MenuKey menuButton = _Buttons->getMenuButton();
  setupMenu(_menu, _Display, _menuItems, itemCount, menuButton,
            _goBackDownAmount);
}

bool MouseSettingsMode::modeLoop() {
  MenuKey menuButton = _Buttons->getMenuButton();
  int changeMenu;
  if (_mousesubmenu == mouseMainMenu) {
    changeMenu = workMenu(_menu, menuButton, _Display);
  }

  // Mouse Settings Main Menu
  if (_mousesubmenu == mouseMainMenu) {
    if (changeMenu != -1) {
      if (changeMenu == 0) {
        return true;
      }
      changeMenu--;
      // If they choose an option
      _mousesubmenu = changeMenu;
      _goBackDownAmount = changeMenu + 1;
      showOption(changeMenu);
    }
  } else {
    showOptionLoop(_mousesubmenu);
  }
  return false;
}

void MouseSettingsMode::showOption(uint8_t option) {
  _Display->clear();
  _Display->setCursor(0, 0);
  switch (option) {
    case mouseToggleButtons:
      printWrappingLineProgmem(_Display, _menuDescriptionToggleMouseButton);
      break;
    case mouseToggleMovement:
      printWrappingLineProgmem(_Display, _menuDescriptionToggleMovement);
      break;
    case mouseMouseWheel:
      printWrappingLineProgmem(_Display, _menuDescriptionWheel);
      break;
    case mouseBorderMouse:
      printWrappingLineProgmem(_Display, _menuDescriptionBorder);
      break;

    default:
      break;
  }
  printButtonPrompt(_Display);
  _Display->setInvertMode(true);
}

void MouseSettingsMode::showOptionLoop(uint8_t option) {
  bool optionBool = _mouseConfig->getOption(option);
  MenuKey menuButton = _Buttons->getToggleButton();
  if (menuButton == MenuKey::BT_DOWN) {
    _mouseConfig->saveMouseEEPROM();
    modeBackToMain();
    return;
  } else if (menuButton == MenuKey::BT_SELECT) {
    optionBool = !optionBool;
    _mouseConfig->setOption(option, optionBool);
  }
  _Display->setCursor(46, 4);
  _Display->set2X();
  _Display->print(_menuBools[optionBool]);
  _Display->set1X();
}

void KeyboardSettingsMode::modeSetup() {
  _keyboardConfig = new KeyboardConfig();
  _keyboardConfig->init();
  _goBackDownAmount = 1;
  modeBackToMain();
}

void KeyboardSettingsMode::modeBackToMain() {
  _Display->setInvertMode(false);
  _Display->clear();
  _keyboardsubmenu = keyboardMainMenu;
  uint8_t itemCount = sizeof(_menuItems) / sizeof(char *);
  MenuKey menuButton = _Buttons->getMenuButton();
  setupMenu(_menu, _Display, _menuItems, itemCount, menuButton,
            _goBackDownAmount);
}

bool KeyboardSettingsMode::modeLoop() {
  MenuKey menuButton = _Buttons->getMenuButton();
  int changeMenu;
  if (_keyboardsubmenu == keyboardMainMenu) {
    changeMenu = workMenu(_menu, menuButton, _Display);
  }

  // Keyboard Settings Main Menu
  if (_keyboardsubmenu == keyboardMainMenu) {
    if (changeMenu != -1) {
      if (changeMenu == 0) {
        return true;
      }
      changeMenu--;
      // If they choose an option
      _keyboardsubmenu = changeMenu;
      _goBackDownAmount = changeMenu + 1;
      showOption(changeMenu);
    }
  } else {
    showOptionLoop(_keyboardsubmenu);
  }
  return false;
}

void KeyboardSettingsMode::showOption(uint8_t option) {
  _Display->clear();
  _Display->setCursor(0, 0);
  switch (option) {
    case keyboardToggleButtons:
      printWrappingLineProgmem(_Display, _menuDescriptionToggleKBButton);
      break;
    case keyboardWASDMouse:
      printWrappingLineProgmem(_Display, _menuDescriptionWASDMouse);
      break;
    case keyboardF24:
      printWrappingLineProgmem(_Display, _menuDescriptionF24);
      break;
    default:
      break;
  }
  printButtonPrompt(_Display);
  _Display->setInvertMode(true);
}

void KeyboardSettingsMode::showOptionLoop(uint8_t option) {
  bool optionBool = _keyboardConfig->getOption(option);
  MenuKey menuButton = _Buttons->getToggleButton();
  if (menuButton == MenuKey::BT_DOWN) {
    _keyboardConfig->saveKeyboardEEPROM();
    modeBackToMain();
    return;
  } else if (menuButton == MenuKey::BT_SELECT) {
    optionBool = !optionBool;
    _keyboardConfig->setOption(option, optionBool);
  }
  _Display->setCursor(46, 4);
  _Display->set2X();
  _Display->print(_menuBools[optionBool]);
  _Display->set1X();
}

void RGBSettingsMode::modeSetup() {
  _goBackDownAmount = 1;
  modeBackToMain();
}

void RGBSettingsMode::modeBackToMain() {
  _Display->clear();
  _rgbsubmenu = rgbMainMenu;
  uint8_t itemCount = sizeof(_rgbMenuItems) / sizeof(char *);
  MenuKey menuButton = _Buttons->getMenuButton();
  setupMenu(_menu, _Display, _rgbMenuItems, itemCount, menuButton,
            _goBackDownAmount);
}

/*
typedef enum {
  rgbMainMenu = -1,
  rgbMenuBrightness,
  rgbMenuMode,
  rgbMenuOnPush,
  rgbMenuSpeed,
  rgbMenuInfoTest
} rgbMenuNumbers;
*/

bool RGBSettingsMode::modeLoop() {
  MenuKey menuButton;
  int changeMenu;
  if (_rgbsubmenu != rgbMenuInfoTest) {
    menuButton = _Buttons->getMenuButton();
    changeMenu = workMenu(_menu, menuButton, _Display);
  }
  // RGB Main Menu
  if (_rgbsubmenu == rgbMainMenu) {
    if (changeMenu != -1) {
      if (changeMenu == 0) {
        return true;
      }
      changeMenu--;
      // If they choose an option
      _rgbsubmenu = changeMenu;
      _goBackDownAmount = changeMenu + 1;
      _previousMenuHoverSelection = -1;
      if (changeMenu == rgbMenuBrightness) {
        uint8_t itemCount =
            sizeof(_rgbSpeedBrightnessMenuItems) / sizeof(char *);
        setupMenu(_menu, _Display, _rgbSpeedBrightnessMenuItems, itemCount,
                  menuButton);
        //_Display->printFixed(0,0,"ff");
      } else if (changeMenu == rgbMenuMode) {
        uint8_t itemCount = sizeof(_rgbModeMenuItems) / sizeof(char *);
        setupMenu(_menu, _Display, _rgbModeMenuItems, itemCount, menuButton);
        //_Display->printFixed(0,0,"ff");
      } else if (changeMenu == rgbMenuOnPush) {
        uint8_t itemCount = sizeof(_rgbOnPushMenuItems) / sizeof(char *);
        setupMenu(_menu, _Display, _rgbOnPushMenuItems, itemCount, menuButton);
      } else if (changeMenu == rgbMenuSpeed) {
        uint8_t itemCount =
            sizeof(_rgbSpeedBrightnessMenuItems) / sizeof(char *);
        setupMenu(_menu, _Display, _rgbSpeedBrightnessMenuItems, itemCount,
                  menuButton, 0, true);
      } else if (changeMenu == rgbMenuInfoTest) {
        _Display->clear();
        char bright[4];
        char rgbmode[2];
        char rgbpush[2];
        char rgbspeed[4];
        sprintf(bright, "%d", _rgb->brightness);
        sprintf(rgbmode, "%d", _rgb->mode);
        sprintf(rgbpush, "%d", _rgb->on_push);
        sprintf(rgbspeed, "%d", _rgb->speed);
        _Display->setCursor(0, 0);
        printWrappingLineProgmem(_Display, _rgbInfoTest);
        _Display->setCursor(0, 5);
        printWrappingLineProgmem(_Display, _rgbInfoSpeed);

        _Display->setCursor(80, 0);
        _Display->print(bright);
        _Display->setCursor(80, 1);
        _Display->print(rgbmode);
        _Display->setCursor(80, 2);
        _Display->print(rgbpush);
        _Display->setCursor(80, 3);
        _Display->print(rgbspeed);
      }
    }
  } else if (_rgbsubmenu == rgbMenuBrightness) {
    uint8_t menuHoverSelection = _menu->currentSelection;
    if (menuHoverSelection != _previousMenuHoverSelection) {
      _rgb->brightness = menuHoverSelection * 10;
      _rgb->setBrightness(_rgb->brightness);
    }
    _previousMenuHoverSelection = menuHoverSelection;
    if (changeMenu != -1) {
      _rgb->saveRGBEEPROM();
      modeBackToMain();
    }
  } else if (_rgbsubmenu == rgbMenuMode) {
    uint8_t menuHoverSelection = _menu->currentSelection;
    if (menuHoverSelection != _previousMenuHoverSelection) {
      _rgb->mode = menuHoverSelection;
    }
    _previousMenuHoverSelection = menuHoverSelection;
    if (changeMenu != -1) {
      _rgb->saveRGBEEPROM();
      modeBackToMain();
    }
  } else if (_rgbsubmenu == rgbMenuOnPush) {
    uint8_t menuHoverSelection = _menu->currentSelection;
    if (menuHoverSelection != _previousMenuHoverSelection) {
      _rgb->on_push = menuHoverSelection;
    }
    _previousMenuHoverSelection = menuHoverSelection;
    if (changeMenu != -1) {
      _rgb->saveRGBEEPROM();
      modeBackToMain();
    }
  } else if (_rgbsubmenu == rgbMenuSpeed) {
    uint8_t menuHoverSelection = _menu->currentSelection;
    if (menuHoverSelection != _previousMenuHoverSelection) {
      _rgb->speed = (menuHoverSelection + 1) * 10;
    }
    _previousMenuHoverSelection = menuHoverSelection;
    if (changeMenu != -1) {
      _rgb->saveRGBEEPROM();
      modeBackToMain();
    }
  } else if (_rgbsubmenu == rgbMenuInfoTest) {
    // :)
  }
  return false;
}
////////////////////////////

void SleepSettingsMode::modeSetup() {
  _goBackDownAmount = 1;
  modeBackToMain();
}

void SleepSettingsMode::modeBackToMain() {
  _Display->clear();
  _sleepsubmenu = sleepMainMenu;
  uint8_t itemCount = sizeof(_sleepMenuItems) / sizeof(char *);
  MenuKey menuButton = _Buttons->getMenuButton();
  setupMenu(_menu, _Display, _sleepMenuItems, itemCount, menuButton,
            _goBackDownAmount);
}

/*
typedef enum {
  sleepMainMenu = -1,
  sleepMenuUSBSleep,
  sleepMenuSleepTimer,
  sleepMenuSleepers,
  sleepMenuHelp
} sleepMenuNumbers;
*/

bool SleepSettingsMode::modeLoop() {
  MenuKey menuButton;
  int changeMenu;
  menuButton = _Buttons->getMenuButton();
  changeMenu = workMenu(_menu, menuButton, _Display);
  // Sleep Main Menu
  if (_sleepsubmenu == sleepMainMenu) {
    if (changeMenu != -1) {
      if (changeMenu == 0) {
        return true;
      }
      changeMenu--;
      // If they choose an option
      _sleepsubmenu = changeMenu;
      _goBackDownAmount = changeMenu + 1;
      _previousMenuHoverSelection = -1;
      if (changeMenu == sleepMenuUSBSleep) {
        showOption(sleepMenuUSBSleep);
        //_Display->printFixed(0,0,"ff");
      } else if (changeMenu == sleepMenuSleepTimer) {
        uint8_t itemCount = sizeof(_sleepTimerMenuItems) / sizeof(char *);
        setupMenu(_menu, _Display, _sleepTimerMenuItems, itemCount, menuButton);
        //_Display->printFixed(0,0,"ff");
      } else if (changeMenu == sleepMenuSleepers) {
        uint8_t itemCount = sizeof(_sleeperMenuItems) / sizeof(char *);
        setupMenu(_menu, _Display, _sleeperMenuItems, itemCount, menuButton);
        //_Display->printFixed(0,0,"ff");
      } else if (changeMenu == sleepMenuSleepers) {
        showOption(sleepMenuUSBSleep);
      }
    }
  } else if (_sleepsubmenu == sleepMenuUSBSleep) {
    showOptionLoop(sleepMenuUSBSleep);
  } else if (_sleepsubmenu == sleepMenuSleepTimer) {
    uint8_t menuHoverSelection = _menu->currentSelection;
    if (menuHoverSelection != _previousMenuHoverSelection) {
      uint16_t sleepTimer = 0;
      switch (menuHoverSelection) {
        case 0:
          sleepTimer = (15 * 60);
          break;

        case 1:
          sleepTimer = (30 * 60);
          break;

        case 2:
          sleepTimer = (1 * 3600);
          break;

        case 3:
          sleepTimer = (2 * 3600);
          break;

        case 4:
          sleepTimer = (3 * 3600);
          break;

        case 5:
          sleepTimer = (4 * 3600);
          break;

        case 6:
          sleepTimer = (6 * 3600);
          break;

        case 7:
          sleepTimer = (8 * 3600);
          break;

        case 8:
          sleepTimer = (10 * 3600);
          break;

        case 9:
          sleepTimer = (12 * 3600);
          break;

        default:
          break;
      }
      _sleepConfig->sleepTimer = sleepTimer;
    }
    _previousMenuHoverSelection = menuHoverSelection;
    if (changeMenu != -1) {
      _sleepConfig->saveSleepEEPROM();
      modeBackToMain();
    }
  } else if (_sleepsubmenu == sleepMenuSleepers) {
    uint8_t menuHoverSelection = _menu->currentSelection;
    if (menuHoverSelection != _previousMenuHoverSelection) {
      switch (menuHoverSelection)
      {
      case 0:
        _sleepConfig->sleepOLED = false;
        _sleepConfig->sleepRGB = true;
        break;

      case 1:
        _sleepConfig->sleepOLED = true;
        _sleepConfig->sleepRGB = false;
        break;

      case 2:
        _sleepConfig->sleepOLED = true;
        _sleepConfig->sleepRGB = true;
        break;
      
      default:
        break;
      }
    }
    _previousMenuHoverSelection = menuHoverSelection;
    if (changeMenu != -1) {
      _sleepConfig->saveSleepEEPROM();
      modeBackToMain();
    }
  } else if (_sleepsubmenu == sleepMenuHelp) {
    showOption(sleepMenuHelp);
  }
  return false;
}

void SleepSettingsMode::showOption(uint8_t option) {
  _Display->clear();
  _Display->setCursor(0, 0);
  switch (option) {
    case sleepMenuUSBSleep:
      printWrappingLineProgmem(_Display, _menuDescriptionUSBSleep);
      break;
    case sleepMenuHelp:
      printWrappingLineProgmem(_Display, _menuDescriptionSleepHelp);
      break;
    default:
      break;
  }
  printButtonPrompt(_Display);
  _Display->setInvertMode(true);
}

void SleepSettingsMode::showOptionLoop(uint8_t option) {
  bool optionBool = _sleepConfig->getOption(option);
  MenuKey menuButton = _Buttons->getToggleButton();
  if (menuButton == MenuKey::BT_DOWN) {
    _sleepConfig->saveSleepEEPROM();
    modeBackToMain();
    return;
  }

  if(option == sleepMenuUSBSleep){
    if (menuButton == MenuKey::BT_SELECT) {
    optionBool = !optionBool;
    _sleepConfig->setOption(option, optionBool);
  }
  _Display->setCursor(46, 4);
  _Display->set2X();
  _Display->print(_menuBools[optionBool]);
  _Display->set1X();
  }
}
/*
void SleepSettingsMode::showOptionLoop(uint8_t option) {
  bool optionBool = _keyboardConfig->getOption(option);
  MenuKey menuButton = _Buttons->getToggleButton();
  if (menuButton == MenuKey::BT_DOWN) {
    _keyboardConfig->saveKeyboardEEPROM();
    modeBackToMain();
    return;
  } else if (menuButton == MenuKey::BT_SELECT) {
    optionBool = !optionBool;
    _keyboardConfig->setOption(option, optionBool);
  }
  _Display->setCursor(46, 4);
  _Display->set2X();
  _Display->print(_menuBools[optionBool]);
  _Display->set1X();
}*/
///////////////

void printWrappingLineProgmem(SSD1306AsciiAvrI2c *_Display,
                              const char *signMessage PROGMEM) {
  // uint8_t initColumn = _Display->col();
  uint8_t currentLineLength = 0;
  // read back a char
  char myChar;
  for (byte k = 0; k < strlen_P(signMessage); k++) {
    myChar = pgm_read_byte_near(signMessage + k);
    // Serial.print(myChar);
    if (myChar == '\n') {
      currentLineLength = 0;
      _Display->setCursor(0, _Display->row() + 1);
    } else {
      _Display->print(myChar);
      currentLineLength++;
      if (currentLineLength > 20) {
        currentLineLength = 0;
        _Display->setCursor(0, _Display->row() + 1);
      }
    }
  }
}

void printWrappingLine(SSD1306AsciiAvrI2c *_Display, const char *line) {
  // uint8_t initColumn = _Display->col();
  uint8_t currentLineLength = 0;
  for (uint8_t i = 0; i < strlen(line); i++) {
    if (line[i] == '\n') {
      currentLineLength = 0;
      _Display->setCursor(0, _Display->row() + 1);
    } else {
      _Display->print(line[i]);
      currentLineLength++;
      if (currentLineLength > 20) {
        currentLineLength = 0;
        _Display->setCursor(0, _Display->row() + 1);
      }
    }
  }
}

void printButtonPrompt(SSD1306AsciiAvrI2c *_Display) {
  _Display->setCursor(0, 6);
  printWrappingLineProgmem(_Display, _buttonPrompt);
}