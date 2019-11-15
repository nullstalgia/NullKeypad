#include "HID-Project.h"
#include "ssd1306.h"
#include "nano_gfx.h"
#include <EasyButton.h>
#include <FastLED.h>
#include <EEPROM.h>
#include <MemoryFree.h>

#define RGB_PIN     9
#define LED_TYPE    NEOPIXEL
#define NUM_LEDS    9

byte RGB_BRIGHTNESS = 50;
byte RGB_MODE = 1;
byte RGB_ON_PUSH = 2;

uint8_t gHue = 0;

SAppMenu menu;

EasyButton button0(16, 35, true, true);
EasyButton button1(14, 35, true, true);
EasyButton button2(15, 35, true, true);
EasyButton button3(10, 35, true, true);
EasyButton button4(8, 35, true, true);
EasyButton button5(7, 35, true, true);
EasyButton button6(6, 35, true, true);
EasyButton button7(5, 35, true, true);
EasyButton button8(4, 35, true, true);

// Labels for menu options
const char *mainMenuItems[] =
{
  "F13-21 Keys",
  "RGB Controls",
  "Arrow Keys",
  "Counter",
  "Media Keys",
  "Macro (wip)",
  "Mouse"
};

const char *rgbMenuItems[] =
{
  "Brightness",
  "Mode",
  "Push Action",
  "Speed (reset req)",
  "Test + Info",
};

const char *chooseMenuItems[] =
{
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "10"
};

// How many items in current menu
int item_count = 0;
// Which one is chosen
int chosen_in_menu = 0;


// Our struct for EEPROM storage of RGB configs.
struct RGBConfig {
  byte brightness;
  byte mode;
  byte on_push;
  byte speed;
};


// Modes! Oh boy
// Mode 0: Main Menu
// Mode 1: F13-21 Keys (keypad with the "hidden" F13, F14, F15... keys.
// Mode 2: RGB Controls (save RGB preferences in EEPROM)
// Mode 3: Arrow Keys (Arrow keys and Tab, Enter, etc)
// Mode 4: Simple counter for keeping track of whatever. Saves to EEPROM.
int mode = 0;

// Sub menus, mostly.
int sub_mode = 0;

char menu_prog[5];

byte RGB_SPEED = 1;


CRGB leds[NUM_LEDS];



int pins[9] = {16, 14, 15, 10, 8, 7, 6, 5, 4};
bool isPressed[10];
bool wasPressed[10];
bool wasReleased[10];


const char F1[] PROGMEM = "F13";
const char F2[] PROGMEM = "F14";
const char F3[] PROGMEM = "F15";
const char F4[] PROGMEM = "F16";
const char F5[] PROGMEM = "F17";
const char F6[] PROGMEM = "F18";
const char F7[] PROGMEM = "F19";
const char F8[] PROGMEM = "F20";
const char F9[] PROGMEM = "F21";

const char *const fkeys[] PROGMEM = {F1, F2, F3, F4, F5, F6, F7, F8, F9};

KeyboardKeycode fkey_code[9] =
{ KEY_F13, KEY_F14, KEY_F15,
  KEY_F16, KEY_F17, KEY_F18,
  KEY_F19, KEY_F20, KEY_F21
};



const char Ar1[] PROGMEM = "TAB";
const char Ar2[] PROGMEM = "ESC";
const char Ar3[] PROGMEM = "END";
const char Ar4[] PROGMEM = "LALT";
const char Ar5[] PROGMEM = " ^";
const char Ar6[] PROGMEM = "RET";
const char Ar7[] PROGMEM = "<-";
const char Ar8[] PROGMEM = " V";
const char Ar9[] PROGMEM = "->";

const char *const akeys[] PROGMEM = {Ar1, Ar2, Ar3, Ar4, Ar5, Ar6, Ar7, Ar8, Ar9};

KeyboardKeycode akey_code[9] =
{ KEY_TAB, KEY_ESC, KEY_END,
  KEY_LEFT_ALT, KEY_UP_ARROW, KEY_RETURN,
  KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW
};



const char Me1[] PROGMEM = "V+";
const char Me2[] PROGMEM = "Mute";
const char Me3[] PROGMEM = "V-";
const char Me4[] PROGMEM = "Prev";
const char Me5[] PROGMEM = ">||";
const char Me6[] PROGMEM = "Next";
const char Me7[] PROGMEM = "Rew";
const char Me8[] PROGMEM = "Stop";
const char Me9[] PROGMEM = "FF";

const char *const mediakeys[] PROGMEM = {Me1, Me2, Me3, Me4, Me5, Me6, Me7, Me8, Me9};

ConsumerKeycode mediakey_code[9] =
{ MEDIA_VOLUME_UP, MEDIA_VOLUME_MUTE, MEDIA_VOLUME_DOWN,
  MEDIA_PREVIOUS, MEDIA_PLAY_PAUSE, MEDIA_NEXT,
  MEDIA_REWIND, MEDIA_STOP, MEDIA_FAST_FORWARD
};


const char C1[] PROGMEM = "";
const char C2[] PROGMEM = "";
const char C3[] PROGMEM = "";
const char C4[] PROGMEM = "^";
const char C5[] PROGMEM = "^";
const char C6[] PROGMEM = "^";
const char C7[] PROGMEM = "V";
const char C8[] PROGMEM = "V";
const char C9[] PROGMEM = "V";

const char *const counter_keys[] PROGMEM = {C1, C2, C3, C4, C5, C6, C7, C8, C9};

const char Ma1[] PROGMEM = "Compl.";
const char Ma2[] PROGMEM = "2";
const char Ma3[] PROGMEM = "3";
const char Ma4[] PROGMEM = "Upload";
const char Ma5[] PROGMEM = "5";
const char Ma6[] PROGMEM = "6";
const char Ma7[] PROGMEM = "PrgUp";
const char Ma8[] PROGMEM = "8";
const char Ma9[] PROGMEM = "9";

const char *const macro_names[] PROGMEM = {Ma1, Ma2, Ma3, Ma4, Ma5, Ma6, Ma7, Ma8, Ma9};


const char M1[] PROGMEM = "M1";
const char M2[] PROGMEM = "M3";
const char M3[] PROGMEM = "M2";
const char M4[] PROGMEM = "Sp-";
const char M5[] PROGMEM = "U";
const char M6[] PROGMEM = "Sp+";
const char M7[] PROGMEM = "L";
const char M8[] PROGMEM = "D";
const char M9[] PROGMEM = "R";

const char *const mouse_keys[] PROGMEM = {M1, M2, M3, M4, M5, M6, M7, M8, M9};

const static char mouse_buttons[] = {MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT};

char button_buffer[7];

bool redraw = true;

unsigned long mode_start_millis = 0;

int mode_start_delay = 250;

unsigned long current_millis = 0;

unsigned long rgb_speed_millis = 0;

unsigned long eeprom_save_delay_millis = 0;

bool mode_extension_bool = false;

int reset_reset = -1;

int counters[3] = {0, 0, 0};

char counter_buffer[17];

bool freeMemSet = false;

byte modifier_factor = 10;

void setup() {
  // put your setup code here, to run once:
  delay(1000);

  //Serial.begin(115200);

  // Init pushed array

  for (int i = 0; i < 10; i++) {
    isPressed[i] = false;
    wasPressed[i] = false;
    wasReleased[i] = false;
  }

  // EEPROM Addresses:
  // 1  - RGBConfig (4 bytes long)
  // 12 - Counter X
  // 14 - Counter Y
  // 16 - Counter Z
  RGBConfig rgbConfig;

  EEPROM.get(1, rgbConfig);

  RGB_BRIGHTNESS = rgbConfig.brightness;
  RGB_MODE = rgbConfig.mode;
  RGB_ON_PUSH = rgbConfig.on_push;
  RGB_SPEED = rgbConfig.speed;

  EEPROM.get(12, counters[0]);
  EEPROM.get(14, counters[1]);
  EEPROM.get(16, counters[2]);

  // In case it was a new/invalid config.

  if (RGB_BRIGHTNESS == 255) {
    RGB_BRIGHTNESS = 0;
  }

  if (RGB_MODE == 255) {
    RGB_MODE = 0;
  }

  if (RGB_ON_PUSH == 255) {
    RGB_ON_PUSH = 0;
  }

  if (RGB_SPEED == 255) {
    RGB_SPEED = 20;
  }

  FastLED.addLeds<LED_TYPE, RGB_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(RGB_BRIGHTNESS);


  /* Select the font to use with menu and all font functions */
  ssd1306_setFixedFont(ssd1306xled_font6x8);

  ssd1306_128x64_i2c_init();

  modeChangeSetup(mode);

  // Getting the button array ready and then started (pullups and the like)
  basicButt(false);

  if (isPressed[8]) {
    freeMemSet = true;
  }
}
uint8_t can_buffer[96 * 60 / 8];
NanoCanvas canvas(96, 60, can_buffer);
void loop() {
  basicButt(true);
  current_millis = millis();
  for (int i = 0; i < 9; i++) {
    //buttons[i]->read();
    //pressed[i] = buttons[i]->wasPressed();
  }

  // put your main code here, to run repeatedly:
  //delay(1000);
  //ssd1306_clearScreen( );


  // LED Stuff to *always* run

  // Cycling the rainbow effect
  /*
    if((current_millis - rgb_speed_millis) >= RGB_SPEED){
    gHue++;
    rgb_speed_millis = rgb_speed_millis;
    }*/
  EVERY_N_MILLISECONDS(RGB_SPEED) {
    gHue++;
  };

  switch (RGB_MODE)
  {
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
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(gHue, 255, 255), CHSV(gHue + 10, 255, 255));
      break;

    case 6:
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(-gHue, 255, 255), CHSV(-gHue + 10, 255, 255));
      break;

    case 7:
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(gHue, 255, 255), CHSV(gHue + 70, 255, 255));
      break;

    case 8:
      fill_gradient_RGB(leds, NUM_LEDS, CHSV(-gHue, 255, 255), CHSV(-gHue + 70, 255, 255));
      break;

    default:
      break;
  }

  if (RGB_ON_PUSH != 0) {
    for (int i = 0; i < NUM_LEDS; i++) {

      if (RGB_ON_PUSH <= 2) {
        if (isPressed[i]) {
          if (RGB_ON_PUSH == 1) {
            leds[i] = CRGB::White;
          } else { // RGB_ON_PUSH == 2
            leds[i] = CRGB::Black;
          }
        }
      } else {
        if (RGB_ON_PUSH == 3) {
          if (!isPressed[i]) {
            leds[i] = CRGB::Black;
          }
        } else { // RGB_ON_PUSH == 4
          if (!isPressed[i]) {
            leds[i] = CRGB::White;
          }
        }
      }


      switch (RGB_ON_PUSH) {
        case 0:
          break;
      }
    }
  }

  FastLED.show();

  if (mode == 0) {
    // Main Menu
    if (workMenu(true)) {
      modeChangeSetup(chosen_in_menu);
    }
  } else {
    if ((current_millis - mode_start_millis) > mode_start_delay) {
      // Other modes go here!

      // F13-21
      if (mode == 1) {



        for (int i = 0; i < 9; i++) {
          if (wasPressed[i]) {
            redraw = true;
            Keyboard.press(fkey_code[i]);
          }
          if (wasReleased[i]) {
            redraw = true;
            Keyboard.release(fkey_code[i]);
          }
        }
        if (redraw) {

          canvas.clear();

          //canvas.drawHLine(0, 15,  ssd1306_displayWidth() - 1);
          //canvas.drawHLine(0, 35,  ssd1306_displayWidth() - 1);
          //canvas.drawHLine(0, 60,  ssd1306_displayWidth() - 1);

          int i = 0;
          int budge = 3;
          for (int y = 0; y < 3; y ++) {
            for (int x = 0; x < 3; x ++) {
              int xpos = 35 * x + 1;
              int ypos = 20 * y + 1;
              if (!isPressed[i]) {
                strcpy_P(button_buffer, (char *)pgm_read_word(&(fkeys[i])));
                //button_buffer
                canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL );
              } else {
                strcpy_P(button_buffer, (char *)pgm_read_word(&(fkeys[i])));
                //button_buffer
                canvas.printFixed(xpos + budge, ypos + budge, button_buffer, STYLE_BOLD );
              }

              i++;
            }
          }

          //canvas.printFixed(20, 1, " DEMO ", STYLE_BOLD );
          canvas.blt(20, 1);
        }
        redraw = false;
      }
      else
        // End Mode 1

        // RGB Config
        if (mode == 2) {
          if (sub_mode == 0) {
            // RGB Main Menu
            if (workMenu(true)) {
              // If chose button mode
              if (chosen_in_menu == 3) {
                item_count = 5;
              } else {
                item_count = sizeof(chooseMenuItems) / sizeof(char *);
              }
              showMenu(chooseMenuItems);
              sub_mode = chosen_in_menu;

              if (chosen_in_menu == 5) {
                //uint8_t can_buffer[128 * 64 / 8];
                //NanoCanvas canvas(128, 64, can_buffer);
                char bright[4];
                char rgbmode[2];
                char rgbpush[2];
                char rgbspeed[4];
                sprintf(bright, "%d", RGB_BRIGHTNESS);
                sprintf(rgbmode, "%d", RGB_MODE);
                sprintf(rgbpush, "%d", RGB_ON_PUSH);
                sprintf(rgbspeed, "%d", RGB_SPEED);

                canvas.printFixed(0, 0, "Brightness:", STYLE_NORMAL );
                canvas.printFixed(0, 8, "Mode:", STYLE_NORMAL );
                canvas.printFixed(0, 16, "Push Action:", STYLE_NORMAL );
                canvas.printFixed(0, 24, "RGB Speed:", STYLE_NORMAL );

                canvas.printFixed(80, 0, bright, STYLE_NORMAL );
                canvas.printFixed(80, 8, rgbmode, STYLE_NORMAL );
                canvas.printFixed(80, 16, rgbpush, STYLE_NORMAL );
                canvas.printFixed(80, 24, rgbspeed, STYLE_NORMAL );
                ssd1306_clearScreen();
                canvas.blt(0, 0);
              }
            }
          }
          else
            // RGB Brightness Control
            if (sub_mode == 1) {
              if (workMenu(false)) {
                saveRGBEEPROM();
                item_count = sizeof(rgbMenuItems) / sizeof(char *);
                showMenu(rgbMenuItems);
                sub_mode = 0;
              }
              RGB_BRIGHTNESS = 25 * (chosen_in_menu);
              FastLED.setBrightness(RGB_BRIGHTNESS);
            }
            else
              // RGB Mode Control
              if (sub_mode == 2) {
                if (workMenu(false)) {
                  saveRGBEEPROM();
                  item_count = sizeof(rgbMenuItems) / sizeof(char *);
                  showMenu(rgbMenuItems);
                  sub_mode = 0;
                }
                RGB_MODE = chosen_in_menu;
              }
              else
                // Controlling if we should do something when the user pushes a button.
                if (sub_mode == 3) {
                  if (workMenu(false)) {
                    saveRGBEEPROM();
                    item_count = sizeof(rgbMenuItems) / sizeof(char *);
                    showMenu(rgbMenuItems);
                    sub_mode = 0;
                  }
                  RGB_ON_PUSH = chosen_in_menu;
                }
                else if (sub_mode == 4) {
                  // RGB Rainbow Speed
                  if (workMenu(false)) {
                    saveRGBEEPROM();
                    item_count = sizeof(rgbMenuItems) / sizeof(char *);
                    showMenu(rgbMenuItems);
                    sub_mode = 0;
                  }
                  RGB_SPEED = 10 * (chosen_in_menu + 1);
                }
                else
                  // Testing the users mode && giving info
                  if (sub_mode == 5) {
                  }
          //ssd1306_printFixed(100,  16, EEPROM.length(), STYLE_NORMAL);
        }
      // End Mode 2

      // Mode 3: Arrow keys
        else if (mode == 3) {

          //uint8_t can_buffer[128 * 64 / 8];
          //NanoCanvas canvas(128, 64, can_buffer);

          for (int i = 0; i < 9; i++) {
            if (wasPressed[i]) {
              redraw = true;
              Keyboard.press(akey_code[i]);
            }
            if (wasReleased[i]) {
              redraw = true;
              Keyboard.release(akey_code[i]);
            }
          }
          if (redraw) {

            canvas.clear();

            //canvas.drawHLine(0, 15,  ssd1306_displayWidth() - 1);
            //canvas.drawHLine(0, 35,  ssd1306_displayWidth() - 1);
            //canvas.drawHLine(0, 60,  ssd1306_displayWidth() - 1);

            int i = 0;
            int budge = 3;
            for (int y = 0; y < 3; y ++) {
              for (int x = 0; x < 3; x ++) {
                int xpos = 35 * x + 1;
                int ypos = 20 * y + 1;
                if (!isPressed[i]) {
                  strcpy_P(button_buffer, (char *)pgm_read_word(&(akeys[i])));
                  //button_buffer
                  canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL );
                } else {
                  strcpy_P(button_buffer, (char *)pgm_read_word(&(akeys[i])));
                  //button_buffer
                  canvas.printFixed(xpos + budge, ypos + budge, button_buffer, STYLE_BOLD );
                }

                i++;
              }
            }

            //canvas.printFixed(20, 1, " DEMO ", STYLE_BOLD );
            canvas.blt(20, 1);
          }
          redraw = false;
        }
      // End Mode 3
      // Mode 4: Counter
        else if (mode == 4) {

          for (int i = 0; i < 9; i++) {
            if (wasPressed[i]) {
              redraw = true;
              if (i < 3) {
                if (reset_reset == -1) {
                  reset_reset = i;
                } else if (reset_reset == i) {
                  counters[i] = 0;
                  reset_reset = -1;
                } else {
                  reset_reset = -1;
                }
              } else {
                if (reset_reset != -1) {
                  reset_reset = -1;
                }
              }



              if (i == 3) {
                counterButton(0, true);
              }
              if (i == 4) {
                counterButton(1, true);
              }
              if (i == 5) {
                counterButton(2, true);
              }

              if (i == 6) {
                counterButton(0, false);
              }
              if (i == 7) {
                counterButton(1, false);
              }
              if (i == 8) {
                counterButton(2, false);
              }
            }
            if (wasReleased[i]) {
              redraw = true;
              mode_extension_bool = true;
              eeprom_save_delay_millis = current_millis;
            }

          }
          if (redraw) {

            canvas.clear();

            int i = 0;
            int budge = 2;
            if (mode_extension_bool) {
              canvas.drawHLine(0, 0, 96);
            }
            for (int y = 0; y < 3; y ++) {
              for (int x = 0; x < 3; x ++) {
                int xpos = 35 * x + 1;
                int ypos = 20 * y + 1;
                if (i < 3) {
                  //sprintf(menu_prog, "%d", counters[i]);
                  if (sub_mode == 1) {
                    sprintf(counter_buffer, "%X", counters[i]);
                  } else if (sub_mode == 2) {
                    itoa(counters[i], counter_buffer, 2);
                  } else {
                    itoa(counters[i], counter_buffer, 10);
                  }

                  //itoa(counters[i], counter_buffer, 16);

                  if ( sub_mode != 2 ) {
                    if (reset_reset == i) {
                      canvas.printFixed(xpos + budge, ypos, counter_buffer, STYLE_BOLD );
                    } else {
                      canvas.printFixed(xpos , ypos + budge, counter_buffer, STYLE_NORMAL );
                    }
                  } else {
                    if (reset_reset == i) {
                      canvas.printFixed(0 + budge, 2 + ( i * 9) + 2, counter_buffer, STYLE_BOLD );
                    } else {
                      canvas.printFixed(0 , 2 + ( i * 9) + 2 + budge, counter_buffer, STYLE_NORMAL );
                    }
                  }





                  //itoa(counters[i], counter_buffer, 10);
                  //canvas.printFixed(0 , 0, sub_mode, STYLE_BOLD );

                } else {
                  ypos = 25 + ( y * 7) + 1;
                  if (!isPressed[i]) {
                    strcpy_P(button_buffer, (char *)pgm_read_word(&(counter_keys[i])));
                    //button_buffer
                    canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL );
                  } else {
                    strcpy_P(button_buffer, (char *)pgm_read_word(&(counter_keys[i])));
                    //button_buffer
                    canvas.printFixed(xpos + budge, ypos + budge, button_buffer, STYLE_BOLD );
                  }
                }


                i++;
              }
            }

            //canvas.printFixed(20, 1, " DEMO ", STYLE_BOLD );
            canvas.blt(20, 1);
          }
          redraw = false;

          if ((current_millis - eeprom_save_delay_millis) > 7500 && mode_extension_bool) {
            EEPROM.put(12, counters[0]);
            EEPROM.put(14, counters[1]);
            EEPROM.put(16, counters[2]);
            mode_extension_bool = false;
            redraw = true;
          }
        }
      // End Mode 4
      // Mode 5: Media keys
        else if (mode == 5) {

          for (int i = 0; i < 9; i++) {
            if (wasPressed[i]) {
              redraw = true;
              Consumer.press(mediakey_code[i]);
            }
            if (wasReleased[i]) {
              redraw = true;
              Consumer.release(mediakey_code[i]);
            }
          }
          if (redraw) {

            canvas.clear();

            int i = 0;
            int budge = 3;
            for (int y = 0; y < 3; y ++) {
              for (int x = 0; x < 3; x ++) {
                int xpos = 35 * x + 1;
                int ypos = 20 * y + 1;
                if (!isPressed[i]) {
                  strcpy_P(button_buffer, (char *)pgm_read_word(&(mediakeys[i])));
                  //button_buffer
                  canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL );
                } else {
                  strcpy_P(button_buffer, (char *)pgm_read_word(&(mediakeys[i])));
                  canvas.printFixed(xpos + budge, ypos + budge, button_buffer, STYLE_BOLD );
                }

                i++;
              }
            }
            canvas.blt(20, 1);
          }
          redraw = false;
        }
      // End Mode 5
      // Mode 6: Macros (not custom, just stuff I use)
        else if (mode == 6) {
          for (int i = 0; i < 9; i++) {
            if (wasPressed[i]) {
              redraw = true;

              // Arduino Compile
              if (i == 0) {
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press('r');
                Keyboard.releaseAll();
              }

              // Regular Arduino Upload
              if (i == 3) {
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press('u');
                Keyboard.releaseAll();
              }

              // Arduino Upload with Programmer
              if (i == 6) {
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_LEFT_SHIFT);
                Keyboard.press('u');
                Keyboard.releaseAll();
              }
            }
            if (wasReleased[i]) {
              redraw = true;
              //Keyboard.release(akey_code[i]);
            }
          }
          if (redraw) {

            canvas.clear();

            //canvas.drawHLine(0, 15,  ssd1306_displayWidth() - 1);
            //canvas.drawHLine(0, 35,  ssd1306_displayWidth() - 1);
            //canvas.drawHLine(0, 60,  ssd1306_displayWidth() - 1);

            int i = 0;
            int budge = 1;
            for (int y = 0; y < 3; y ++) {
              for (int x = 0; x < 3; x ++) {
                int xpos = 40 * x + 1;
                int ypos = 20 * y + 1;
                if (!isPressed[i]) {
                  strcpy_P(button_buffer, (char *)pgm_read_word(&(macro_names[i])));
                  //button_buffer
                  canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL );
                } else {
                  strcpy_P(button_buffer, (char *)pgm_read_word(&(macro_names[i])));
                  canvas.printFixed(xpos + budge, ypos + budge, button_buffer, STYLE_BOLD );
                }

                i++;
              }
            }

            //canvas.printFixed(20, 1, " DEMO ", STYLE_BOLD );
            canvas.blt(20, 1);
          }
          redraw = false;
        }
      // End Mode 6
      // Mode 7: Mouse
        else if (mode == 7) {
          counters[0] = 0;
          counters[1] = 0;
          for (int i = 0; i < 9; i++) {
            if (wasPressed[i]) {
              redraw = true;
              if (i < 3) {
                if (counters[2] == 0) {
                  Mouse.press(mouse_buttons[i]);
                } else {
                  if (Mouse.isPressed(mouse_buttons[i])) {
                    Mouse.release(mouse_buttons[i]);
                  } else {
                    Mouse.press(mouse_buttons[i]);
                  }
                }
              }

              if (i == 3) {
                sub_mode--;
              } else if (i == 5) {
                sub_mode++;
              }

              if (sub_mode < 1) {
                sub_mode = 1;
              } else if (sub_mode > 10) {
                sub_mode = 10;
              }

              //continue;
            }
            if (wasReleased[i]) {
              redraw = true;
              if (counters[2] == 0) {
                if (i < 3) {
                  Mouse.release(mouse_buttons[i]);
                }
              }
              //Consumer.release(mediakey_code[i]);
            }
            if (isPressed[i]) {
              if (i == 4) {
                counters[1] -= sub_mode;
              } else if (i == 6) {
                counters[0] -= sub_mode;
              } else if (i == 7) {
                counters[1] += sub_mode;
              } else if (i == 8) {
                counters[0] += sub_mode;
              }
            }
          }
          Mouse.move(counters[0], counters[1]);
          if (redraw) {

            canvas.clear();

            int i = 0;
            int budge = 3;
            for (int y = 0; y < 3; y ++) {
              for (int x = 0; x < 3; x ++) {
                int xpos = 35 * x + 1;
                int ypos = 20 * y + 1;
                if ( i < 3 ) {

                  if (!Mouse.isPressed(mouse_buttons[i])) {
                    strcpy_P(button_buffer, (char *)pgm_read_word(&(mouse_keys[i])));
                    //char button_buffer[7];
                    canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL );
                  } else {
                    strcpy_P(button_buffer, (char *)pgm_read_word(&(mouse_keys[i])));
                    canvas.printFixed(xpos + budge, ypos + budge, button_buffer, STYLE_BOLD );
                  }

                } else {
                  if (!isPressed[i]) {
                    strcpy_P(button_buffer, (char *)pgm_read_word(&(mouse_keys[i])));
                    //char button_buffer[7];
                    canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL );
                  } else {
                    strcpy_P(button_buffer, (char *)pgm_read_word(&(mouse_keys[i])));
                    canvas.printFixed(xpos + budge, ypos + budge, button_buffer, STYLE_BOLD );
                  }
                }

                i++;
              }
            }
            canvas.blt(20, 1);
          }
          redraw = false;
        }

      // End Mode 7

    }
  }

  //FastLED.show();
}

// Redo menu with a new char* array
/*
  Usage:
  item_count = sizeof(given_array) / sizeof(char *);
  showMenu(given_array);
*/
void showMenu(const char *given_array[]) {
  ssd1306_clearScreen();
  delay(mode_start_delay);
  ssd1306_createMenu( &menu, given_array, item_count);
  ssd1306_showMenu( &menu );
}

bool workMenu(bool show_prog) {
  chosen_in_menu = ssd1306_menuSelection(&menu);
  if (show_prog) {
    chosen_in_menu++;
    if (freeMemSet) {
      sprintf(menu_prog, "%d/%d", freeMemory(), item_count);
    } else {
      sprintf(menu_prog, "%d/%d", chosen_in_menu, item_count);
    }

    ssd1306_printFixed(99,  8, menu_prog, STYLE_NORMAL);
  }
  //delay(500);

  if (wasPressed[1]) {
    ssd1306_menuUp(&menu);
  } else if (wasPressed[7]) {
    ssd1306_menuDown(&menu);
  } else if (wasPressed[4] || wasPressed[5]) {
    if (mode == 0) {
      mode = chosen_in_menu;
    }
    mode_start_millis = current_millis;
    //canvas.clear();
    ssd1306_clearScreen();
    redraw = true;
    return true;
  }
  ssd1306_updateMenu(&menu);
  return false;
}

void saveRGBEEPROM() {
  RGBConfig rgbConfig;
  rgbConfig.brightness = RGB_BRIGHTNESS;
  rgbConfig.mode = RGB_MODE;
  rgbConfig.on_push = RGB_ON_PUSH;
  rgbConfig.speed = RGB_SPEED;

  EEPROM.put(1, rgbConfig);
}

void modeChangeSetup(int new_mode) {
  // Special things that need to be done just once
  // Like their own setup()
  switch (new_mode)
  {
    case 0:
      // Need to set global item_count so the "progress" is accurate and the amount of items is accurate
      item_count = sizeof(mainMenuItems) / sizeof(char *);
      showMenu(mainMenuItems);
      break;

    case 1:
    case 3:
    case 6:
      delay(mode_start_delay * 3);
      Keyboard.begin();
      break;

    case 5:
      delay(mode_start_delay * 3);
      Consumer.begin();
      break;

    case 2:
      item_count = sizeof(rgbMenuItems) / sizeof(char *);
      showMenu(rgbMenuItems);
      break;

    case 7:
      counters[2] = 0;
      delay(mode_start_delay * 3);
      Mouse.begin();
      sub_mode = 1;
      if (isPressed[6]) {
        counters[2] = 1;
      } else if (isPressed[8]) {
        counters[2] = 2;
      }
      break;


    case 4:
      ssd1306_setFixedFont(ssd1306xled_font5x7);
      if (isPressed[6] || isPressed[8]) {
        sub_mode = 1;
      }
      break;

    default:
      break;
  }
}

void counterButton(int counter_index, bool addition) {

  if (isPressed[counter_index]) {
    if (addition) {
      counters[counter_index] += modifier_factor;
    } else {
      counters[counter_index] -= modifier_factor;
    }

  } else {
    if (addition) {
      counters[counter_index]++;
    } else {
      counters[counter_index]--;
    }
  }

}


void basicButt(bool read_b) {
  if (read_b) {
    button0.read();
    isPressed[0] = button0.isPressed();
    wasPressed[0] = button0.wasPressed();
    wasReleased[0] = button0.wasReleased();
    button1.read();
    isPressed[1] = button1.isPressed();
    wasPressed[1] = button1.wasPressed();
    wasReleased[1] = button1.wasReleased();
    button2.read();
    isPressed[2] = button2.isPressed();
    wasPressed[2] = button2.wasPressed();
    wasReleased[2] = button2.wasReleased();
    button3.read();
    isPressed[3] = button3.isPressed();
    wasPressed[3] = button3.wasPressed();
    wasReleased[3] = button3.wasReleased();
    button4.read();
    isPressed[4] = button4.isPressed();
    wasPressed[4] = button4.wasPressed();
    wasReleased[4] = button4.wasReleased();
    button5.read();
    isPressed[5] = button5.isPressed();
    wasPressed[5] = button5.wasPressed();
    wasReleased[5] = button5.wasReleased();
    button6.read();
    isPressed[6] = button6.isPressed();
    wasPressed[6] = button6.wasPressed();
    wasReleased[6] = button6.wasReleased();
    button7.read();
    isPressed[7] = button7.isPressed();
    wasPressed[7] = button7.wasPressed();
    wasReleased[7] = button7.wasReleased();
    button8.read();
    isPressed[8] = button8.isPressed();
    wasPressed[8] = button8.wasPressed();
    wasReleased[8] = button8.wasReleased();
  } else {
    button0.begin();
    button1.begin();
    button2.begin();
    button3.begin();
    button4.begin();
    button5.begin();
    button6.begin();
    button7.begin();
    button8.begin();
    basicButt(true);
  }
}
