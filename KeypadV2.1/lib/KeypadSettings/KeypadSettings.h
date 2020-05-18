#pragma once
#include <Arduino.h>
#define DEBOUNCE_TIME 35  // Milliseconds

#define NUM_KEYPAD_BUTTONS 9

#define NUM_ablr_buttons 4

#define NUM_ALL_BUTTONS NUM_KEYPAD_BUTTONS + NUM_ablr_buttons

#define wasdUP 1
#define wasdDOWN 4
#define wasdLEFT 3
#define wasdRIGHT 5

#define ablrA 9
#define ablrB 10
#define ablrL 11
#define ablrR 12

// EEPROM Addresses for reference:
// 1  - RGBConfig (4 bytes long)
// 12 - Counter X
// 14 - Counter Y
// 16 - Counter Z
// 18 - MouseConfig

#define EEPROM_OFFSET_RGB 1

#define EEPROM_OFFSET_COUNT_X 12
#define EEPROM_OFFSET_COUNT_Y 14
#define EEPROM_OFFSET_COUNT_Z 16

#define EEPROM_OFFSET_MOUSE 20
#define EEPROM_OFFSET_KEYBOARD 40

#define EEPROM_TEST_VAL 69

#define RELEASED true
#define NOT_RELEASED false

#define ONLY_READING true
#define USING_BUTTON false

const char up[] = {(char)24, (char)32, '\0'};
const char down[] = {(char)25, (char)32, '\0'};
const char left[] = {(char)27, (char)32, '\0'};
const char right[] = {(char)26, (char)32, '\0'};