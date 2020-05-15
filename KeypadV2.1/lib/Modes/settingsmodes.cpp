#include "settingsmodes.h"

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
        _submenu = modeNumberSettingsMouse;
        _mouseMode = new MouseSettingsMode(_menu, _Display, _Buttons);
        _mouseMode->modeSetup();
      } else if (changeMenu == 1) {
        _submenu = modeNumberSettingsKeyboard;
        //_keyboardMode = new KeyboardSettingsMode(_menu, _Display, _Buttons);
        //_keyboardMode->modeSetup();
      } else if (changeMenu == 2) {
        _submenu = modeNumberSettingsRGB;
        _rgbMode = new RGBSettingsMode(_rgb, _menu, _Display, _Buttons);
        _rgbMode->modeSetup();
      }
    }
  } else if (_submenu == modeNumberSettingsMouse) {
    _mouseMode->modeLoop();
  } else if (_submenu == modeNumberSettingsRGB) {
    _rgbMode->modeLoop();
  } else if (_submenu == modeNumberSettingsKeyboard) {
    //_keyboardMode->modeLoop();
  }
}

void MouseSettingsMode::modeSetup() {
  _mouseConfig = new MouseConfig();
  _mouseConfig->init();
  _goBackDownAmount = 0;
  modeBackToMain();
}

void MouseSettingsMode::modeBackToMain() {
  _Display->positiveMode();
  _Display->clear();
  _mousesubmenu = mouseMainMenu;
  uint8_t itemCount = sizeof(_menuItems) / sizeof(char *);
  MenuKey menuButton = _Buttons->getMenuButton();
  setupMenu(_menu, _Display, _menuItems, itemCount, menuButton,
            _goBackDownAmount);
}

void MouseSettingsMode::modeLoop() {
  MenuKey menuButton = _Buttons->getMenuButton();
  int changeMenu;
  if (_mousesubmenu == mouseMainMenu) {
    changeMenu = workMenu(_menu, menuButton, _Display);
  }

  // Mouse Settings Main Menu
  if (_mousesubmenu == mouseMainMenu) {
    if (changeMenu != -1) {
      // If they choose an option
      _mousesubmenu = changeMenu;
      _goBackDownAmount = changeMenu;
      showOption(changeMenu);
    }
  } else {
    showOptionLoop(_mousesubmenu);
  }
}

void MouseSettingsMode::showOption(uint8_t option) {
  _Display->clear();
  _Display->printFixed(0, 0, _menuDescriptions[option]);
  _Display->printFixed(0, 48, "A/Bottom Row: Toggle");
  _Display->printFixed(0, 56, "B/Middle Row: Back");
  _Display->negativeMode();
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
  _Display->printFixedN(46, 32, _menuBools[optionBool], STYLE_NORMAL, 1);
}


void KeyboardSettingsMode::modeSetup() {
  _keyboardConfig = new KeyboardConfig();
  _keyboardConfig->init();
  _goBackDownAmount = 0;
  modeBackToMain();
}

void KeyboardSettingsMode::modeBackToMain() {
  _Display->positiveMode();
  _Display->clear();
  _keyboardsubmenu = keyboardMainMenu;
  uint8_t itemCount = sizeof(_menuItems) / sizeof(char *);
  MenuKey menuButton = _Buttons->getMenuButton();
  setupMenu(_menu, _Display, _menuItems, itemCount, menuButton,
            _goBackDownAmount);
}

void KeyboardSettingsMode::modeLoop() {
  MenuKey menuButton = _Buttons->getMenuButton();
  int changeMenu;
  if (_keyboardsubmenu == keyboardMainMenu) {
    changeMenu = workMenu(_menu, menuButton, _Display);
  }

  // Keyboard Settings Main Menu
  if (_keyboardsubmenu == keyboardMainMenu) {
    if (changeMenu != -1) {
      // If they choose an option
      _keyboardsubmenu = changeMenu;
      _goBackDownAmount = changeMenu;
      showOption(changeMenu);
    }
  } else {
    showOptionLoop(_keyboardsubmenu);
  }
}

void KeyboardSettingsMode::showOption(uint8_t option) {
  _Display->clear();
  _Display->printFixed(0, 0, _menuDescriptions[option]);
  _Display->printFixed(0, 48, "A/Bottom Row: Toggle");
  _Display->printFixed(0, 56, "B/Middle Row: Back");
  _Display->negativeMode();
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
  _Display->printFixedN(46, 32, _menuBools[optionBool], STYLE_NORMAL, 1);
}


void RGBSettingsMode::modeSetup() {
  _goBackDownAmount = 0;
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

void RGBSettingsMode::modeLoop() {
  MenuKey menuButton;
  int changeMenu;
  if (_rgbsubmenu != rgbMenuInfoTest) {
    menuButton = _Buttons->getMenuButton();
    changeMenu = workMenu(_menu, menuButton, _Display);
  }
  // RGB Main Menu
  if (_rgbsubmenu == rgbMainMenu) {
    if (changeMenu != -1) {
      // If they choose an option
      _rgbsubmenu = changeMenu;
      _goBackDownAmount = changeMenu;
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
                  menuButton);
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

        _Display->printFixed(0, 0, "Brightness:", STYLE_NORMAL);
        _Display->printFixed(0, 8, "Lighting Mode:", STYLE_NORMAL);
        _Display->printFixed(0, 16, "Push Action:", STYLE_NORMAL);
        _Display->printFixed(0, 24, "RGB Speed:", STYLE_NORMAL);

        _Display->printFixed(80, 0, bright, STYLE_NORMAL);
        _Display->printFixed(80, 8, rgbmode, STYLE_NORMAL);
        _Display->printFixed(80, 16, rgbpush, STYLE_NORMAL);
        _Display->printFixed(80, 24, rgbspeed, STYLE_NORMAL);
      }
    }
  } else if (_rgbsubmenu == rgbMenuBrightness) {
    uint8_t menuHoverSelection = _Display->menuSelection(_menu);
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
    uint8_t menuHoverSelection = _Display->menuSelection(_menu);
    if (menuHoverSelection != _previousMenuHoverSelection) {
      _rgb->mode = menuHoverSelection;
    }
    _previousMenuHoverSelection = menuHoverSelection;
    if (changeMenu != -1) {
      _rgb->saveRGBEEPROM();
      modeBackToMain();
    }
  } else if (_rgbsubmenu == rgbMenuOnPush) {
    uint8_t menuHoverSelection = _Display->menuSelection(_menu);
    if (menuHoverSelection != _previousMenuHoverSelection) {
      _rgb->on_push = menuHoverSelection;
    }
    _previousMenuHoverSelection = menuHoverSelection;
    if (changeMenu != -1) {
      _rgb->saveRGBEEPROM();
      modeBackToMain();
    }
  } else if (_rgbsubmenu == rgbMenuSpeed) {
    uint8_t menuHoverSelection = _Display->menuSelection(_menu);
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
}