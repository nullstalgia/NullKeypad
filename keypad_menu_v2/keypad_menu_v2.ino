#include <Arduino.h>     // VS Code/Platform.io funtime.
#include "HID-Project.h" // https://github.com/NicoHood/HID
#include "nano_gfx.h"    // https://github.com/lexus2k/ssd1306/
#include "ssd1306.h"     // https://github.com/lexus2k/ssd1306/
#include <EEPROM.h>
#include <EasyButton.h> // https://github.com/evert-arias/EasyButton
#include <FastLED.h>    // https://github.com/FastLED/FastLED/
#include <MemoryFree.h> // https://github.com/maniacbug/MemoryFree
#include <Wire.h>       // For I2C Devices (OLED)

// Is RGB Enabled?
// TODO
#define RGB_ENABLED 1

// Pin for RGB LED control
#define RGB_PIN 9
#define LED_TYPE NEOPIXEL
#define NUM_LEDS 9
// Array which holds each LED's RGB value
CRGB leds[NUM_LEDS];

// These control the global RGB and will be overwritten in setup()
// when EEPROM is loaded.
byte RGB_BRIGHTNESS = 50;
byte RGB_MODE = 1;
byte RGB_ON_PUSH = 2;
byte RGB_SPEED = 1;

// This is used for the rolling rainbow effect
uint8_t gHue = 0;

// Menu for the OLED
SAppMenu menu;

// Where to usually draw the buffer onto the OLED.
#define drawX 25
#define drawY 1

// Initing all of the buttons and enabling pullups
EasyButton button0(16, 35, true, true);
EasyButton button1(14, 35, true, true);
EasyButton button2(15, 35, true, true);
EasyButton button3(10, 35, true, true);
EasyButton button4(8, 35, true, true);
EasyButton button5(7, 35, true, true);
EasyButton button6(6, 35, true, true);
EasyButton button7(5, 35, true, true);
EasyButton button8(4, 35, true, true);
// int pins[9] = {16, 14, 15, 10, 8, 7, 6, 5, 4};
bool isPressed[13];
bool wasPressed[13];
bool wasReleased[13];

// -- Side Buttons --
EasyButton buttonA(A3, 35, true, true);
EasyButton buttonB(A2, 35, true, true);
EasyButton buttonL(A1, 35, true, true);
EasyButton buttonR(A0, 35, true, true);

// So I can be like isPressed[A]
#define A 9
#define B 10
#define L 11
#define R 12

// Labels for menu options
const char *mainMenuItems[] = {"F13-21 Keys", "RGB Settings", "Arrow Keys",
                               "Counter",     "Media Keys",   "Macro (wip)",
                               "Mouse",       "WASD"};

// Ditto for RGB Settings
const char *rgbMenuItems[] = {
    "Brightness", "Mode", "Push Action", "Speed (reset req)", "Test + Info",
};

// For RGB control, these are the possible "choices" given for brightness,
// speed, etc.
const char *chooseMenuItems[] = {"0", "1", "2", "3", "4", "5",
                                 "6", "7", "8", "9", "10"};

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
// Mode 2: RGB Settings (save RGB preferences in EEPROM)
// Mode 3: Arrow Keys (Arrow keys and Tab, Enter, etc)
// Mode 4: Simple counter for keeping track of whatever. Saves to EEPROM.
// Mode 5: Media Keys! Controls volume, playpause. And extra useless buttons
// Mode 6: Beginnings of Macros. Right now, it's just hardcoded Arduino Compile,
// Upload, and Upload with Prog. Mode 7: Mouse mode. WASD-style mouse with
// clicks and adjustable speed. Mode 8: Basic WASD with common game keys plus
// optional mouse mode replacing A and D
int mode = 0;

// Sub menus, mostly.
// Also used as a custom variable for functions that need that extra variable
// but don't want to use the extra RAM
int sub_mode = 0;

// Ditto
bool mode_extension_bool = false;

// Buffer for the X/Y on the top right of menus.
char menu_prog[5];

// The following is me squeezing as much RAM as I can out of this chip
// Instead of having arrays of char arrays, I have several arrays of PROGMEM'd
// char arrays that are decoded as needed. This is a bit slower it seems, but
// not so bad that it severely impacts the experience

// F13-21 Mode
const char F0[] PROGMEM = "F13";
const char F1[] PROGMEM = "F14";
const char F2[] PROGMEM = "F15";
const char F3[] PROGMEM = "F16";
const char F4[] PROGMEM = "F17";
const char F5[] PROGMEM = "F18";
const char F6[] PROGMEM = "F19";
const char F7[] PROGMEM = "F20";
const char F8[] PROGMEM = "F21";

const char *const fkeys[] PROGMEM = {F0, F1, F2, F3, F4, F5, F6, F7, F8};

KeyboardKeycode fkey_code[9] = {KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17,
                                KEY_F18, KEY_F19, KEY_F20, KEY_F21};

// Arrow Keys mode
const char Ar0[] PROGMEM = "TAB";
const char Ar1[] PROGMEM = "ESC";
const char Ar2[] PROGMEM = "CTL";
const char Ar3[] PROGMEM = "LALT";
const char Ar4[] PROGMEM = " ^";
const char Ar5[] PROGMEM = "RET";
const char Ar6[] PROGMEM = "<-";
const char Ar7[] PROGMEM = " V";
const char Ar8[] PROGMEM = "->";

const char *const akeys[] PROGMEM = {Ar0, Ar1, Ar2, Ar3, Ar4,
                                     Ar5, Ar6, Ar7, Ar8};

KeyboardKeycode akey_code[9] = {
    KEY_TAB,    KEY_ESC,        KEY_LEFT_CTRL,  KEY_LEFT_ALT,   KEY_UP_ARROW,
    KEY_RETURN, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW};

// Media Keys mode
const char Me0[] PROGMEM = "V+";
const char Me1[] PROGMEM = "Mute";
const char Me2[] PROGMEM = "V-";
const char Me3[] PROGMEM = "Prev";
const char Me4[] PROGMEM = ">||";
const char Me5[] PROGMEM = "Next";
const char Me6[] PROGMEM = "Rew";
const char Me7[] PROGMEM = "Stop";
const char Me8[] PROGMEM = "FF";

const char *const mediakeys[] PROGMEM = {Me0, Me1, Me2, Me3, Me4,
                                         Me5, Me6, Me7, Me8};

ConsumerKeycode mediakey_code[9] = {
    MEDIA_VOLUME_UP, MEDIA_VOLUME_MUTE, MEDIA_VOLUME_DOWN,
    MEDIA_PREVIOUS,  MEDIA_PLAY_PAUSE,  MEDIA_NEXT,
    MEDIA_REWIND,    MEDIA_STOP,        MEDIA_FAST_FORWARD};

// Counter mode
// TBH I probably don't even need this
// Nevermind! I do! It takes more mem to get rid of this than keep it.
const char C0[] PROGMEM = "";
const char C1[] PROGMEM = "";
const char C2[] PROGMEM = "";
const char C3[] PROGMEM = "^";
const char C4[] PROGMEM = "^";
const char C5[] PROGMEM = "^";
const char C6[] PROGMEM = "V";
const char C7[] PROGMEM = "V";
const char C8[] PROGMEM = "V";

const char *const counter_keys[] PROGMEM = {C0, C1, C2, C3, C4, C5, C6, C7, C8};

// How long to wait after they let go to save the numbers to EEPROM
#define EEPROM_DELAY_THRESHOLD 7500

// Macros mode
const char Ma0[] PROGMEM = "ArdC";
const char Ma1[] PROGMEM = "PioC";
const char Ma2[] PROGMEM = "Btl";
const char Ma3[] PROGMEM = "ArdU";
const char Ma4[] PROGMEM = "PioU";
const char Ma5[] PROGMEM = "Silk";
const char Ma6[] PROGMEM = "ArdPU";
const char Ma7[] PROGMEM = "PioPU";
const char Ma8[] PROGMEM = "LCSC";

const char *const macro_names[] PROGMEM = {Ma0, Ma1, Ma2, Ma3, Ma4,
                                           Ma5, Ma6, Ma7, Ma8};

// Mouse mode
const char Mo0[] PROGMEM = "M1";
// const char Mo1[] PROGMEM = "M3";
const char Mo1[] PROGMEM = "U";
const char Mo2[] PROGMEM = "M2";
const char Mo3[] PROGMEM = "L";
const char Mo4[] PROGMEM = "D";
const char Mo5[] PROGMEM = "R";
const char Mo6[] PROGMEM = "M4";
const char Mo7[] PROGMEM = "MW";
const char Mo8[] PROGMEM = "M5";

const char *const mouse_keys[] PROGMEM = {Mo0, Mo1, Mo2, Mo3, Mo4,
                                          Mo5, Mo6, Mo7, Mo8};

// Border Mouse mode
const char BMo0[] PROGMEM = "";
const char BMo1[] PROGMEM = " ^";
const char BMo2[] PROGMEM = "";
const char BMo3[] PROGMEM = "<-";
const char BMo4[] PROGMEM = "M1";
const char BMo5[] PROGMEM = "->";
const char BMo6[] PROGMEM = "";
const char BMo7[] PROGMEM = " V";
const char BMo8[] PROGMEM = "";

const char *const bmouse_keys[] PROGMEM = {BMo0, BMo1, BMo2, BMo3, BMo4,
                                           BMo5, BMo6, BMo7, BMo8};

const static char mouse_buttons[] = {MOUSE_LEFT, MOUSE_MIDDLE, MOUSE_RIGHT};

// Also mouse mode. Was helpful for moving around buttons without dealing with
// memhogging arrays
#define UP 1
#define DOWN 4
#define LEFT 3
#define RIGHT 5
#define M1 0
#define M2 2
#define M3 7
#define M4 6
#define M5 8
#define SpeedDown 11
#define SpeedUp 12

// Min and Max speeds
#define MinSpeed -5
#define MaxSpeed 25

// PC Game Mode
const char PCG0[] PROGMEM = "M1";
const char PCG1[] PROGMEM = "W";
const char PCG2[] PROGMEM = "M2";
const char PCG3[] PROGMEM = "A";
const char PCG4[] PROGMEM = "S";
const char PCG5[] PROGMEM = "D";
const char PCG6[] PROGMEM = "E";
const char PCG7[] PROGMEM = "F";
const char PCG8[] PROGMEM = "Spc";

const char *const pcgame_keys[] PROGMEM = {PCG0, PCG1, PCG2, PCG3, PCG4,
                                           PCG5, PCG6, PCG7, PCG8};

#define KeyE 6
#define KeyF 7
#define KeySpace 8
// TODO: Maybe gamepad mode?

// Buffer all of those PROGMEM'd buttons go into when I read them
char button_buffer[7];

// Modes use this to see if its time to redraw on the OLED
// Since if they draw all the time, it slows down the whole operation
bool redraw = true;

// Compares against mode_start_delay to see if its time to give the user control
// of the mode.
unsigned long mode_start_millis = 0;

unsigned long mode_start_delay = 250;

// Used instead of millis() that is refreshed on loop() start. Makes me feel
// better.
unsigned long current_millis = 0;

// Used to see if its time to cycle the RGB Effect (rainbows, etc)
unsigned long rgb_speed_millis = 0;

// Used to see if its been long enough after the user input a counter button
// that it's time to save to EEPROM
unsigned long eeprom_save_delay_millis = 0;

// I hate the name of this, but I couldn't think of anything better.
// Used for when a user double-presses a number in the counter to set it to 0
int reset_reset = -1;

// Counters that count in the counting counter app
int counters[3] = {0, 0, 0};

// When user is in binary counter mode, it uses this to store the binary number
// as a "string" to be easily printed to the OLED
char counter_buffer[17];

// When holding the top key in the counter, change the value by this when going
// up or down
#define modifier_factor 10

// If user holds button8 (9) on boot, it replaces the progress with the free
// memory remaining
bool freeMemSet = false;

// Need to declare functions before they are used for fun VS Code/Platform.io
// stuff. https://bit.ly/ino2cpp

void showMenu(const char *given_array[]);
bool workMenu(bool show_prog);
void saveRGBEEPROM();
void modeChangeSetup(int new_mode);
void counterButton(int counter_index, bool addition);
void mouseSpeed(int key);
void borderMouse(int key, bool release);
void mouseMoving(int key, bool release);
bool mouseButton(char button, bool m_release, bool mouse_wheel, bool toggle,
                 bool reading);
char keyToMouseButton(int key);
char keyToPCGameButton(int key);
void basicButt(bool read_b);

void setup() {
  // put your setup code here, to run once:
  delay(1000);

  // Serial.begin(115200);

  // Initializing button pushed array
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
// Canvas for most modes to draw on the OLED with.
uint8_t can_buffer[88 * 60 / 8];
NanoCanvas canvas(88, 60, can_buffer);
void loop() {
  // Read every button and put the values in the right arrays
  basicButt(true);
  current_millis = millis();
  for (int i = 0; i < 9; i++) {
    // buttons[i]->read();
    // pressed[i] = buttons[i]->wasPressed();
  }

  if (current_millis > 3000) {
    // strcpy(mainMenuItems[0], "hmm");
  }

  // LED Stuff to *always* run

  EVERY_N_MILLISECONDS(RGB_SPEED) { gHue++; };

  switch (RGB_MODE) {
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
  if (RGB_ON_PUSH != 0) {
    for (int i = 0; i < NUM_LEDS; i++) {
      // First 2 modes normally show the selected color
      // And change to white or black when pressed
      if (RGB_ON_PUSH <= 2) {
        if (isPressed[i]) {
          if (RGB_ON_PUSH == 1) {
            leds[i] = CRGB::White;
          } else { // RGB_ON_PUSH == 2
            leds[i] = CRGB::Black;
          }
        }
      } else {
        // Last 2 normally are white or black
        // And show selected color when pressed
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
    }
  }

  // Done with RGB stuff, send it over to the LEDs.
  FastLED.show();

  if (mode == 0) {
    // Main Menu
    // If option is chosen, run the setup then the next loop() will run the
    // mode's function.
    if (workMenu(true)) {
      modeChangeSetup(chosen_in_menu);
    }
  } else {
    if ((current_millis - mode_start_millis) > mode_start_delay) {
      // Other modes go here!

      // Mode 1: F13-21
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

          int i = 0;
          int budge = 3;
          for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
              int xpos = 30 * x + 1;
              int ypos = 20 * y + 1;
              if (!isPressed[i]) {
                strcpy_P(button_buffer, (char *)pgm_read_word(&(fkeys[i])));
                canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL);
              } else {
                strcpy_P(button_buffer, (char *)pgm_read_word(&(fkeys[i])));
                canvas.printFixed(xpos + budge, ypos + budge, button_buffer,
                                  STYLE_BOLD);
              }
              i++;
            }
          }
          canvas.blt(drawX, drawY);
        }
        redraw = false;
      }
      // End Mode 1
      // Mode 2: RGB Config
      else if (mode == 2) {
        // RGB Main Menu
        if (sub_mode == 0) {
          // If they choose an option
          if (workMenu(true)) {

            // If changing on push actions, limit options to 5
            if (chosen_in_menu == 3) {
              item_count = 5;
            } else {
              item_count = sizeof(chooseMenuItems) / sizeof(char *);
            }

            showMenu(chooseMenuItems);
            sub_mode = chosen_in_menu;

            // Test/info menu
            if (chosen_in_menu == 5) {
              char bright[4];
              char rgbmode[2];
              char rgbpush[2];
              char rgbspeed[4];
              sprintf(bright, "%d", RGB_BRIGHTNESS);
              sprintf(rgbmode, "%d", RGB_MODE);
              sprintf(rgbpush, "%d", RGB_ON_PUSH);
              sprintf(rgbspeed, "%d", RGB_SPEED);

              canvas.printFixed(0, 0, "Brightness:", STYLE_NORMAL);
              canvas.printFixed(0, 8, "Mode:", STYLE_NORMAL);
              canvas.printFixed(0, 16, "Push Action:", STYLE_NORMAL);
              canvas.printFixed(0, 24, "RGB Speed:", STYLE_NORMAL);

              canvas.printFixed(80, 0, bright, STYLE_NORMAL);
              canvas.printFixed(80, 8, rgbmode, STYLE_NORMAL);
              canvas.printFixed(80, 16, rgbpush, STYLE_NORMAL);
              canvas.printFixed(80, 24, rgbspeed, STYLE_NORMAL);
              ssd1306_clearScreen();
              canvas.blt(0, 0);
            }
          }
        }
        // RGB Brightness Menu
        else if (sub_mode == 1) {
          if (workMenu(false)) {
            saveRGBEEPROM();
            item_count = sizeof(rgbMenuItems) / sizeof(char *);
            showMenu(rgbMenuItems);
            sub_mode = 0;
          }
          RGB_BRIGHTNESS = 25 * (chosen_in_menu);
          FastLED.setBrightness(RGB_BRIGHTNESS);
        }

        // RGB Mode Menu (what pattern to show)
        else if (sub_mode == 2) {
          if (workMenu(false)) {
            saveRGBEEPROM();
            item_count = sizeof(rgbMenuItems) / sizeof(char *);
            showMenu(rgbMenuItems);
            sub_mode = 0;
          }
          RGB_MODE = chosen_in_menu;
        }

        // Controlling if we should do something when the user pushes a button.
        else if (sub_mode == 3) {
          if (workMenu(false)) {
            saveRGBEEPROM();
            item_count = sizeof(rgbMenuItems) / sizeof(char *);
            showMenu(rgbMenuItems);
            sub_mode = 0;
          }
          RGB_ON_PUSH = chosen_in_menu;
        } else if (sub_mode == 4) {
          // RGB Rainbow Speed
          if (workMenu(false)) {
            saveRGBEEPROM();
            item_count = sizeof(rgbMenuItems) / sizeof(char *);
            showMenu(rgbMenuItems);
            sub_mode = 0;
          }
          RGB_SPEED = 10 * (chosen_in_menu + 1);
        }

        // Testing the users mode && giving info
        else if (sub_mode == 5) {
        }
      }
      // End Mode 2

      // Mode 3: Arrow keys
      else if (mode == 3) {
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

          int i = 0;
          int budge = 3;
          for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
              int xpos = 30 * x + 1;
              int ypos = 20 * y + 1;
              if (!isPressed[i]) {
                strcpy_P(button_buffer, (char *)pgm_read_word(&(akeys[i])));
                canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL);
              } else {
                strcpy_P(button_buffer, (char *)pgm_read_word(&(akeys[i])));
                canvas.printFixed(xpos + budge, ypos + budge, button_buffer,
                                  STYLE_BOLD);
              }

              i++;
            }
          }
          canvas.blt(drawX, drawY);
        }
        redraw = false;
      }
      // End Mode 3
      // Mode 4: Counter
      else if (mode == 4) {

        for (int i = 0; i < 9; i++) {
          if (wasPressed[i]) {
            redraw = true;
            // If they pushed one of the "number" buttons (top row)
            if (i < 3) {
              // And if they haven't pushed one yet
              if (reset_reset == -1) {
                // Then set the "ready to reset" counter
                reset_reset = i;
              } else if (reset_reset == i) {
                // But if they have pushed one and they push the same one
                // Set the chosen counter to 0 and deselect it
                counters[i] = 0;
                reset_reset = -1;
              } else {
                // But if they had one chosen and they push a different top row,
                // deselect it
                reset_reset = -1;
              }
            } else {
              // But if they didn't hit a top row, set it to nothing if it
              // already wasn't
              if (reset_reset != -1) {
                reset_reset = -1;
              }
            }

            // Seperate func for those that take into account the modifiers

            switch (i) {
            case 3:
              counterButton(0, true);
              break;
            case 4:
              counterButton(1, true);
              break;
            case 5:
              counterButton(2, true);
              break;

            case 6:
              counterButton(0, false);
              break;
            case 7:
              counterButton(1, false);
              break;
            case 8:
              counterButton(2, false);
              break;
            }
          }

          if (wasReleased[i]) {
            redraw = true;
            // The extension bool is being used for seeing if we're going to
            // save to EEPROM So when the button is let go, we say "Hey, we
            // should save soon. Here's when they let go"
            mode_extension_bool = true;
            eeprom_save_delay_millis = current_millis;
          }
        }
        if (redraw) {
          canvas.clear();

          int i = 0;
          int budge = 2;
          // If we're gonna save soon, let the user know with a bar across the
          // top
          if (mode_extension_bool) {
            canvas.drawHLine(0, 0, 96);
          }

          for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
              int xpos = 30 * x + 1;
              int ypos = 20 * y + 1;

              // Drawing the numbers
              if (i < 3) {
                // If we're on the hex mode
                if (sub_mode == 1) {
                  // (s)printf already has a int > hex function, so I just use
                  // that
                  sprintf(counter_buffer, "%X", counters[i]);
                } else if (sub_mode == 2) {
                  // If we're in binary mode, put it into the buffer at base 2
                  itoa(counters[i], counter_buffer, 2);
                } else {
                  // Same function as binary but in base 10
                  itoa(counters[i], counter_buffer, 10);
                }

                // If we're not in the binary mode, put the numbers on the top
                // from left to right
                if (sub_mode != 2) {
                  if (reset_reset == i) {
                    canvas.printFixed(xpos + budge, ypos, counter_buffer,
                                      STYLE_BOLD);
                  } else {
                    canvas.printFixed(xpos, ypos + budge, counter_buffer,
                                      STYLE_NORMAL);
                  }
                } else {
                  // If we are, they are placed on the left side from top to
                  // bottom
                  if (reset_reset == i) {
                    canvas.printFixed(0 + budge, 2 + (i * 9) + 2,
                                      counter_buffer, STYLE_BOLD);
                  } else {
                    canvas.printFixed(0, 2 + (i * 9) + 2 + budge,
                                      counter_buffer, STYLE_NORMAL);
                  }
                }
              } else {
                // This forces the buttons to go lower to give room for the
                // binary mode, but I keep it on all the time for continuity.
                ypos = 25 + (y * 7) + 1;
                if (!isPressed[i]) {
                  strcpy_P(button_buffer,
                           (char *)pgm_read_word(&(counter_keys[i])));
                  canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL);
                } else {
                  strcpy_P(button_buffer,
                           (char *)pgm_read_word(&(counter_keys[i])));
                  canvas.printFixed(xpos + budge, ypos + budge, button_buffer,
                                    STYLE_BOLD);
                }
              }
              i++;
            }
          }
          canvas.blt(drawX, drawY);
        }

        redraw = false;
        // If it's ready to save and it's been long enough, put every number
        // into EEPROM
        if ((current_millis - eeprom_save_delay_millis) >
                EEPROM_DELAY_THRESHOLD &&
            mode_extension_bool) {
          EEPROM.put(12, counters[0]);
          EEPROM.put(14, counters[1]);
          EEPROM.put(16, counters[2]);
          // Then stop the saving
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
          for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
              int xpos = 30 * x + 1;
              int ypos = 20 * y + 1;
              if (!isPressed[i]) {
                strcpy_P(button_buffer, (char *)pgm_read_word(&(mediakeys[i])));
                canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL);
              } else {
                strcpy_P(button_buffer, (char *)pgm_read_word(&(mediakeys[i])));
                canvas.printFixed(xpos + budge, ypos + budge, button_buffer,
                                  STYLE_BOLD);
              }

              i++;
            }
          }
          canvas.blt(drawX, drawY);
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

            // PlatformIO Compile
            if (i == 1) {
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_LEFT_ALT);
                Keyboard.press('b');
                Keyboard.releaseAll();
            }

            // Regular PlatformIO Upload
            if (i == 4) {
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_LEFT_ALT);
                Keyboard.press('u');
                Keyboard.releaseAll();
            }

            // PlatformIO Upload with Programmer
            if (i == 7) {
                Keyboard.press(KEY_LEFT_CTRL);
                Keyboard.press(KEY_LEFT_SHIFT);
                Keyboard.press('p');
                Keyboard.releaseAll();
                delay(350);
                Keyboard.println("tasks");
                delay(350);
                Keyboard.println("programmer");
            }

            // Arduino Burn Bootloader
            if (i == 2) {
              Keyboard.press(KEY_LEFT_ALT);
              Keyboard.press('t');
              Keyboard.releaseAll();
              Keyboard.press(KEY_UP_ARROW);
              Keyboard.releaseAll();
              Keyboard.press(KEY_RETURN);
              Keyboard.releaseAll();
            }

            // KiCad choose FSilk in PCBNew
            if (i == 5) {
              Mouse.click();
              delay(100);
              Keyboard.press(KEY_PAGE_UP);
              Keyboard.releaseAll();
              Keyboard.press(KEY_PAGE_UP);
              Keyboard.releaseAll();
              for(int j = 0; j < 6; j++){
                  Keyboard.press(KEY_DOWN_ARROW);
                  Keyboard.releaseAll();
              }
              Keyboard.press(KEY_RETURN);
              Keyboard.releaseAll();
            }

            // KiCad LCSC Field adding
            if (i == 8) {
                Keyboard.print("e");
              for (int j = 0; j < 45; j++) {
                Mouse.move(-127, 127);
                //delay(20);
              }
              //delay(2000);
              //Mouse.move(-127, 127);
              for (int j = 0; j < 7; j++) {
                Mouse.move(127, -127);
                //delay(10);
              }
              Mouse.move(48, -48);
              delay(100);
              Mouse.click();
              delay(100);
              //Keyboard.print("hi");
              Keyboard.print("LCSC");
              delay(100);
              Keyboard.press(KEY_TAB);
              Keyboard.release(KEY_TAB);
              //delay(10);
              Keyboard.press(KEY_LEFT_CTRL);
              Keyboard.press('v');
              Keyboard.releaseAll();

              Keyboard.press(KEY_LEFT_ALT);
              Keyboard.press('o');
              Keyboard.releaseAll();
            }
          }
          if (wasReleased[i]) {
            redraw = true;
          }
        }
        if (redraw) {
          canvas.clear();
          int i = 0;
          int budge = 1;
          for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
              int xpos = 31 * x + 1;
              int ypos = 20 * y + 1;
              if (!isPressed[i]) {
                strcpy_P(button_buffer,
                         (char *)pgm_read_word(&(macro_names[i])));
                canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL);
              } else {
                strcpy_P(button_buffer,
                         (char *)pgm_read_word(&(macro_names[i])));
                canvas.printFixed(xpos + budge, ypos + budge, button_buffer,
                                  STYLE_BOLD);
              }

              i++;
            }
          }
          canvas.blt(drawX, drawY);
        }
        redraw = false;
      }
      // End Mode 6
      // Mode 7: Mouse
      else if (mode == 7) {
        // Bit map for counters[2]
        // 0 - Mouse Wheel Enabled
        // 1 - Toggle Enabled
        // 2 - Mouse Wheel Active

        // Read each bit of the counter to see the options
        bool mouse_wheel_enabled = bitRead(counters[2], 0);
        bool toggle = bitRead(counters[2], 1);
        bool border_mouse = bitRead(counters[2], 3);
        // This is set when M3 is held/toggled when mouse_wheel_enabled is also
        // true This means the wheel is active and should spin when up/down is
        // going
        bool mouse_wheel = bitRead(counters[2], 2);

        // Used for border mouse mode
        bool movingUpDown = false;
        bool movingLeftRight = false;

        if (!border_mouse) {

          for (int i = 0; i < 13; i++) {
            if (wasPressed[i]) {
              redraw = true;
              // If a mouse button is being pressed
              if (i == M1 || i == M2 || i == M3 || i == M4 || i == M5) {
                // Send it to the function
                // The arguments are:
                // Actual mouse button (e.g. MOUSE_LEFT)
                // in wasReleased
                // mouse_wheel_enabled
                // toggle enabled
                // only return if its pressed/active, and don't actually touch
                // the mouse
                mouseButton(keyToMouseButton(i), false, mouse_wheel_enabled,
                            toggle, false);
              }
              // Checking if we're changing speed
              // Arg. is keypad #
              if (i == SpeedUp || i == SpeedDown || i == L || i == R) {
                mouseSpeed(i);
              }

              // Checking if we're moving mouse
              // Args are keypad # and if in wasReleased
              mouseMoving(i, false);
            }

            if (wasReleased[i]) {
              redraw = true;
              if (i == M1 || i == M2 || i == M3 || i == M4 || i == M5) {
                mouseButton(keyToMouseButton(i), true, mouse_wheel_enabled,
                            toggle, false);
              }
              mouseMoving(i, true);
            }

            // Unused for now.
            if (isPressed[i]) {
            }

            // In case we messed up (and just in general), if theres no buttons
            // being held, don't move the mouse in the given axis
            if (!isPressed[LEFT] && !isPressed[RIGHT]) {
              counters[0] = 0;
            }

            if (!isPressed[UP] && !isPressed[DOWN]) {
              counters[1] = 0;
            }
          }

          // If the wheel is to be spun, ignore any X and Y movements and just
          // spin the wheel But only send mouse events if needed, otherwise it
          // messes with the whole PC (Especially on Windows)
          if ((mouse_wheel_enabled && mouse_wheel) &&
              (isPressed[UP] || isPressed[DOWN])) {
            Mouse.move(0, 0, -counters[1]);
            // Delay is because it was going too fast
            delay(50);
          } else if (isPressed[UP] || isPressed[DOWN] || isPressed[LEFT] ||
                     isPressed[RIGHT]) {
            Mouse.move(counters[0], counters[1]);
          }

          if (redraw) {
            canvas.clear();
            // Printing current speed
            sprintf(counter_buffer, "Speed: %d", sub_mode);
            canvas.printFixed(15, 0, counter_buffer, STYLE_NORMAL);
            int i = 0;
            int budge = 3;
            for (int y = 0; y < 3; y++) {
              for (int x = 0; x < 3; x++) {
                int xpos = 30 * x + 1;
                int ypos = 21 * y + 1;
                ypos = 10 + (y * 15) + 1;
                if (i == M1 || i == M2 || i == M3 || i == M4 || i == M5) {

                  if (!mouseButton(keyToMouseButton(i), true,
                                   mouse_wheel_enabled, toggle, true)) {
                    strcpy_P(button_buffer,
                             (char *)pgm_read_word(&(mouse_keys[i])));
                    canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL);
                  } else {
                    strcpy_P(button_buffer,
                             (char *)pgm_read_word(&(mouse_keys[i])));
                    canvas.printFixed(xpos + budge, ypos + budge, button_buffer,
                                      STYLE_BOLD);
                  }

                } else {
                  if (!isPressed[i]) {
                    strcpy_P(button_buffer,
                             (char *)pgm_read_word(&(mouse_keys[i])));
                    canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL);
                  } else {
                    strcpy_P(button_buffer,
                             (char *)pgm_read_word(&(mouse_keys[i])));
                    canvas.printFixed(xpos + budge, ypos + budge, button_buffer,
                                      STYLE_BOLD);
                  }
                }
                i++;
              }
            }
            canvas.blt(drawX, drawY);
          }
        } else {

          for (int i = 0; i < 13; i++) {
            if (wasPressed[i]) {
              redraw = true;
              // If a mouse button is being pressed
              if (i == 4) {
                // Send it to the function
                // The arguments are:
                // Actual mouse button (e.g. MOUSE_LEFT)
                // in wasReleased
                // mouse_wheel_enabled
                // toggle enabled
                // only return if its pressed/active, and don't actually touch
                // the mouse
                mouseButton(MOUSE_LEFT, false, false, toggle, false);
              }
              if (i == A) {
                mouseButton(MOUSE_LEFT, false, false, toggle, false);
              }
              if (i == B) {
                mouseButton(MOUSE_RIGHT, false, false, toggle, false);
              }
              if (i == L) {
                mouseSpeed(SpeedDown);
              }
              if (i == R) {
                mouseSpeed(SpeedUp);
              }
              // Checking if we're moving mouse
              // Args are keypad # and if in wasReleased
              borderMouse(i, false);
            }

            if (wasReleased[i]) {
              redraw = true;
              if (i == 4) {
                mouseButton(MOUSE_LEFT, true, false, toggle, false);
              }
              if (i == A) {
                mouseButton(MOUSE_LEFT, true, false, toggle, false);
              }
              if (i == B) {
                mouseButton(MOUSE_RIGHT, true, false, toggle, false);
              }
              borderMouse(i, true);
            }

            // Checking to see if anything was pushed to see if we're allowed to
            // move the mouse
            if (isPressed[i] && !(i >= A)) {
              if (i < 3) {
                movingUpDown = true;
              }
              if (i > 5) {
                movingUpDown = true;
              }
              if (i % 3 == 0) {
                movingLeftRight = true;
              }
              if ((i + 1) % 3 == 0) {
                movingLeftRight = true;
              }
            }
          }

          if (movingUpDown || movingLeftRight) {
            Mouse.move(counters[0], counters[1]);
          } else {
            // If we're not moving, make sure we reset as borderMouse() can
            // sometimes let little bits of speed stay after release.
            counters[0] = 0;
            counters[1] = 0;
          }

          if (redraw) {
            canvas.clear();
            // Printing current speed
            sprintf(counter_buffer, "Speed: %d", sub_mode);
            canvas.printFixed(15, 0, counter_buffer, STYLE_NORMAL);
            int i = 0;
            int budge = 3;
            for (int y = 0; y < 3; y++) {
              for (int x = 0; x < 3; x++) {
                int xpos = 15 * x + 20;
                int ypos = 21 * y + 1;
                ypos = 10 + (y * 15) + 1;
                if (!isPressed[i]) {
                  strcpy_P(button_buffer,
                           (char *)pgm_read_word(&(bmouse_keys[i])));
                  canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL);
                } else {
                  strcpy_P(button_buffer,
                           (char *)pgm_read_word(&(bmouse_keys[i])));
                  canvas.printFixed(xpos + budge, ypos + budge, button_buffer,
                                    STYLE_BOLD);
                }

                i++;
              }
            }
            canvas.blt(drawX, drawY);
          }
        }
        redraw = false;
      }
      // End Mode 7
      // Mode 8: Basic WASD
      else if (mode == 8) {

        // Bit map for counters[2]
        // 0 - If we're enabling the mouse for keys A and D
        // 1 - Nothing RN (maybe change to something else)
        // 2 - Mouse Wheel Active (haven't changed)

        // Read each bit of the counter to see the options
        bool mouse_move_enabled = bitRead(counters[2], 0);
        // bool toggle = bitRead(counters[2], 1);
        bool toggle = false;

        for (int i = 0; i < 9; i++) {
          if (wasPressed[i]) {
            redraw = true;
            // If a mouse button is being pressed
            if (i == M1 || i == M2) {
              // Send it to the function
              // The arguments are:
              // Actual mouse button (e.g. MOUSE_LEFT)
              // in wasReleased
              // mouse_wheel_enabled
              // toggle enabled
              // only return if its pressed/active, and don't actually touch the
              // mouse
              mouseButton(keyToMouseButton(i), false, false, toggle, false);
            } else {
              // But if it's A/D and we have the mouse on
              if ((i == LEFT || i == RIGHT) && mouse_move_enabled) {
                // Move the mouse
                mouseMoving(i, false);
              } else {
                // Otherwise just hit the KB key
                Keyboard.press(keyToPCGameButton(i));
              }
            }
          }

          if (wasReleased[i]) {
            redraw = true;
            if (i == M1 || i == M2) {
              mouseButton(keyToMouseButton(i), true, false, toggle, false);
            } else {
              if ((i == LEFT || i == RIGHT) && mouse_move_enabled) {
                mouseMoving(i, true);
              } else {
                Keyboard.release(keyToPCGameButton(i));
              }
            }
          }

          // Unused for now.
          if (isPressed[i]) {
          }

          // In case we messed up (and just in general), if theres no buttons
          // being held, don't move the mouse in the given axis
          if (!isPressed[LEFT] && !isPressed[RIGHT]) {
            counters[0] = 0;
          }
        }

        if ((isPressed[LEFT] || isPressed[RIGHT]) && mouse_move_enabled) {
          Mouse.move(counters[0], counters[1]);
        }

        if (redraw) {
          canvas.clear();
          // Printing current speed
          sprintf(counter_buffer, "Speed: %d", sub_mode);
          canvas.printFixed(15, 0, counter_buffer, STYLE_NORMAL);
          int i = 0;
          int budge = 3;
          for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
              int xpos = 30 * x + 1;
              int ypos = 21 * y + 1;
              ypos = 10 + (y * 15) + 1;

              if (!isPressed[i]) {
                strcpy_P(button_buffer,
                         (char *)pgm_read_word(&(pcgame_keys[i])));
                canvas.printFixed(xpos, ypos, button_buffer, STYLE_NORMAL);
              } else {
                strcpy_P(button_buffer,
                         (char *)pgm_read_word(&(pcgame_keys[i])));
                canvas.printFixed(xpos + budge, ypos + budge, button_buffer,
                                  STYLE_BOLD);
              }

              i++;
            }
          }
          canvas.blt(drawX, drawY);
        }
        redraw = false;
      }

      // End Mode 8
    }
  }
}

// Redo menu with a new char* array
/*
  Usage:
  item_count = sizeof(given_array) / sizeof(char *);
  showMenu(given_array);
*/
void showMenu(const char *given_array[]) {
  ssd1306_clearScreen();
  // delay(mode_start_delay);
  ssd1306_createMenu(&menu, given_array, item_count);
  ssd1306_showMenu(&menu);
}

// Run every loop when the menu is open
bool workMenu(bool show_prog) {
  chosen_in_menu = ssd1306_menuSelection(&menu);
  if (show_prog) {
    chosen_in_menu++;
    if (freeMemSet) {
      sprintf(menu_prog, "%d/%d", freeMemory(), item_count);
    } else {
      sprintf(menu_prog, "%d/%d", chosen_in_menu, item_count);
    }

    ssd1306_printFixed(99, 8, menu_prog, STYLE_NORMAL);
  }

  if (wasPressed[1]) {
    ssd1306_menuUp(&menu);
  } else if (wasPressed[7]) {
    ssd1306_menuDown(&menu);
  } else if (wasPressed[4] || wasPressed[5]) {
    if (mode == 0) {
      mode = chosen_in_menu;
    }
    mode_start_millis = current_millis;
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
  switch (new_mode) {
  // Main Menu
  case 0:
    // Need to set global item_count so the "progress" is accurate and the
    // amount of items is accurate
    item_count = sizeof(mainMenuItems) / sizeof(char *);
    showMenu(mainMenuItems);
    break;

  // F13-21
  case 1:
  // Arrow Keys
  case 3:
  // Macros
  case 6:
    delay(mode_start_delay * 3);
    Keyboard.begin();
    Mouse.begin();
    break;

  // Media Keys
  case 5:
    delay(mode_start_delay * 3);
    Consumer.begin();
    break;

  // RGB Settings
  case 2:
    item_count = sizeof(rgbMenuItems) / sizeof(char *);
    showMenu(rgbMenuItems);
    break;

  // Mouse
  case 7:
    counters[0] = 0;
    counters[1] = 0;
    counters[2] = 0;
    sub_mode = 1;
    if (isPressed[8]) {
      bitSet(counters[2], 1);
    }
    // If 1 isnt pushed and 7 is, set up mouse wheel mode
    // But if 1 is pushed, set up border mouse moving mode
    if (!isPressed[0]) {
      if (isPressed[6]) {
        bitSet(counters[2], 0);
      }
    } else {
      sub_mode = 3;
      bitSet(counters[2], 3);
      // Lower speed a bit if 3 is also pressed
      if (isPressed[2]) {
        sub_mode = 1;
      }
    }
    delay(mode_start_delay * 3);
    Mouse.begin();
    break;

  // WASD
  case 8:
    counters[0] = 0;
    counters[1] = 0;
    counters[2] = 0;
    sub_mode = 3;
    if (isPressed[6]) {
      bitSet(counters[2], 0);
    }
    if (isPressed[8]) {
      bitSet(counters[2], 1);
    }
    delay(mode_start_delay * 3);
    Keyboard.begin();
    Mouse.begin();
    break;

  // Counters
  case 4:
    ssd1306_setFixedFont(ssd1306xled_font5x7);
    // ssd1306_setFixedFont(digital_font5x7);
    if (isPressed[6]) {
      sub_mode = 1;
    } else if (isPressed[8]) {
      sub_mode = 2;
    }
    break;

  default:
    break;
  }
}

void counterButton(int counter_index, bool addition) {
  // If holding the corresponding counter top key, then change it by the factor
  // rather than just 1
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

void mouseSpeed(int key) {
  // For telling the func if it did change in the end
  // Used for changing the speed mid button-hold
  bool changed = true;

  // And seeing if we should turn the speed up or down if we do end up changing
  // it mid hold
  bool minus = false;

  // Changing the speed "modifier"
  if (key == SpeedDown || key == L) {
    sub_mode--;
    minus = true;
  } else if (key == SpeedUp || key == R) {
    sub_mode++;
  }

  // If it was an unvalid move (going out of bounds), ignore the change
  if (sub_mode < MinSpeed) {
    sub_mode = MinSpeed;
    changed = false;
  } else if (sub_mode > MaxSpeed) {
    sub_mode = MaxSpeed;
    changed = false;
  }

  if (changed) {
    if (counters[0] < 0) {
      if (!minus) {
        counters[0]--;
      } else {
        counters[0]++;
      }
    } else if (counters[0] > 0) {
      if (!minus) {
        counters[0]++;
      } else {
        counters[0]--;
      }
    }

    if (counters[1] < 0) {
      if (!minus) {
        counters[1]--;
      } else {
        counters[1]++;
      }
    } else if (counters[1] > 0) {
      if (!minus) {
        counters[1]++;
      } else {
        counters[1]--;
      }
    }
  }
}

void borderMouse(int key, bool release) {
  // If it's the middle key (Mouse 1), or the side keys, then ignore this and
  // move on
  if (key == 4 || key >= A) {
    return;
  }

  // Just used to make it a little nicer on the eyes
  int speed = sub_mode;

  if (!release) {
    // If we're not already moving the mouse
    if (true) {
      // Keys 1 through 3 (Top edge)
      if (key < 3) {
        counters[1] -= speed;
      }
      // Keys 6 through 8 (Bottom edge)
      if (key > 5) {
        counters[1] += speed;
      }
      // Keys 0, 3, 6 (Left edge)
      if (key % 3 == 0) {
        counters[0] -= speed;
      }
      // Keys 2, 5, 8 (Right edge)
      if ((key + 1) % 3 == 0) {
        counters[0] += speed;
      }
    } else {
      // !! Now impossible state. !!
      // If mouse is moving, stop it and redo this as to not re-add speed
      counters[0] = 0;
      counters[1] = 0;
      borderMouse(key, false);
    }
  } else {
    // Reversing the above action if a button is let go
    // Keys 1 through 3 (Top edge)
    if (key < 3) {
      counters[1] += speed;
    }
    // Keys 6 through 8 (Bottom edge)
    if (key > 5) {
      counters[1] -= speed;
    }
    // Keys 0, 3, 6 (Left edge)
    if (key % 3 == 0) {
      counters[0] += speed;
    }
    // Keys 2, 5, 8 (Right edge)
    if ((key + 1) % 3 == 0) {
      counters[0] -= speed;
    }

    // Leftovers.
    /*
    // Checking to see which one is being pushed and then pushing it
    // TODO: Maybe try this on mouseMoving?
    for (int i = 0; i < 9; i++) {
      if (i == 4) {
        continue;
      }
      if (isPressed[i]) {
        borderMouse(i, false);
        break;
      }
    }
     */
  }
}

void mouseMoving(int key, bool release) {
  // If it's a key we don't want, just stop here and go back to where we were
  // called
  switch (key) {
  case M1:
  case M2:
  case M3:
  case M4:
  case M5:
  case A:
  case B:
  case L:
  case R:
    return;
    break;
  }

  // Counter 0 is X
  // Counter 1 is Y

  // Just used to make it a little nicer on the eyes
  int speed = sub_mode;
  // If we weren't called in the wasReleased
  if (!release) {
    // And if we're going up or down
    if (key == UP || key == DOWN) {
      // AND if it wasn't already moving...
      if (counters[1] == 0) {
        // ...just set the movement
        if (key == UP) {
          counters[1] -= speed;
          return;
        } else if (key == DOWN) {
          counters[1] += speed;
        }
      } else {
        // But if we were already moving
        // Make it think we weren't, and rerun the function to change the
        // direction instantly Even if the old button is still being held
        counters[1] = 0;
        mouseMoving(key, false);
      }
    } else if (key == LEFT || key == RIGHT) {
      if (counters[0] == 0) {
        if (key == LEFT) {
          counters[0] -= speed;
          return;
        } else if (key == RIGHT) {
          counters[0] += speed;
        }
      } else {
        counters[0] = 0;
        mouseMoving(key, false);
      }
    }
  } else {
    // But if a key was released, we don't want to just stop the mouse moving
    // instantly So we check to see which direction we *should* be going based
    // on what still being held And rerun the function to keep the direction
    if (key == UP && isPressed[DOWN]) {
      counters[1] = 0;
      mouseMoving(DOWN, false);
      return;
    }
    if (key == DOWN && isPressed[UP]) {
      counters[1] = 0;
      mouseMoving(UP, false);
      return;
    }
    if (key == LEFT && isPressed[RIGHT]) {
      counters[0] = 0;
      mouseMoving(RIGHT, false);
      return;
    }
    if (key == RIGHT && isPressed[LEFT]) {
      counters[0] = 0;
      mouseMoving(LEFT, false);
      return;
    }
  }
}

// Ran every time a mouse key is pressed
bool mouseButton(char button, bool m_release, bool mouse_wheel, bool toggle,
                 bool reading) {

  // Bit map for counters[2]
  // 0 - Mouse Wheel Enabled
  // 1 - Toggle Enabled
  // 2 - Mouse Wheel Active

  // If we're not just ignoring everything to get the current state
  if (!reading) {
    // We check to see if we're dealing with the special Mouse 3/Middle Mouse
    // key
    if (button == MOUSE_MIDDLE) {
      // If the wheel is enabled at all
      if (mouse_wheel) {
        // We gotta see if we're going to change it
        // If toggle is enabled and we're not in the wasReleased
        if (toggle && !m_release) {
          // Check to see if it's already active
          // If it is, deactivate it
          if (bitRead(counters[2], 2)) {
            bitClear(counters[2], 2);
            return false;
          } else {
            // Otherwise, activate it
            bitSet(counters[2], 2);
            return true;
          }
        } else if (toggle && m_release) {
          // If we're in the release and we're toggling, ignore this
          return false;
        } else {
          // But if the wheel is enabled but we're not toggling
          // Disactivate if releasing, activate if not releasing
          if (m_release) {
            bitClear(counters[2], 2);
            return false;
          } else {
            bitSet(counters[2], 2);
            return true;
          }
        }
      }
    }
    // Done with middle mouse for now

    // Same deal with toggling like M3
    if (toggle && !m_release) {
      if (Mouse.isPressed(button)) {
        Mouse.release(button);
        return false;
      } else {
        Mouse.press(button);
        return true;
      }
    } else if (toggle && m_release) {
      return false;
    }

    // And same deal if not toggling like M3
    if (m_release) {
      Mouse.release(button);
      return false;
    } else {
      Mouse.press(button);
      return true;
    }
  } else {
    // But if we're just reading this and not changing anything...
    // Return the state of the wheel if that's what we're here for
    if (button == MOUSE_MIDDLE) {
      if (mouse_wheel) {
        if (bitRead(counters[2], 2)) {
          return true;
        } else {
          return false;
        }
      }
    }

    // Otherwise just say the state of the asked button
    return Mouse.isPressed(button);
  }

  // If nothing happened, just say false
  return false;
}

// Quick thing to get the requested button in case it's not a 0 1 2 layout
char keyToMouseButton(int key) {
  if (key == M2) {
    return MOUSE_RIGHT;
  } else if (key == M3) {
    return MOUSE_MIDDLE;
  } else if (key == M4) {
    return MOUSE_NEXT;
  } else if (key == M5) {
    return MOUSE_PREV;
  }
  return MOUSE_LEFT;
}

// Ditto for PC Game keys
char keyToPCGameButton(int key) {
  if (key == UP) {
    return 'w';
  } else if (key == DOWN) {
    return 's';
  } else if (key == LEFT) {
    return 'a';
  } else if (key == RIGHT) {
    return 'd';
  } else if (key == KeyE) {
    return 'e';
  } else if (key == KeyF) {
    return 'f';
  } else if (key == KeySpace) {
    return ' ';
  }
  return 'w';
}

// Run every loop to check the state of buttons and set them up
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
    // -- Side Buttons --
    buttonA.read();
    isPressed[9] = buttonA.isPressed();
    wasPressed[9] = buttonA.wasPressed();
    wasReleased[9] = buttonA.wasReleased();
    buttonB.read();
    isPressed[10] = buttonB.isPressed();
    wasPressed[10] = buttonB.wasPressed();
    wasReleased[10] = buttonB.wasReleased();
    buttonL.read();
    isPressed[11] = buttonL.isPressed();
    wasPressed[11] = buttonL.wasPressed();
    wasReleased[11] = buttonL.wasReleased();
    buttonR.read();
    isPressed[12] = buttonR.isPressed();
    wasPressed[12] = buttonR.wasPressed();
    wasReleased[12] = buttonR.wasReleased();
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
    // -- Side Buttons --
    buttonA.begin();
    buttonB.begin();
    buttonL.begin();
    buttonR.begin();
    basicButt(true);
  }
}