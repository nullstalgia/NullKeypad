#include <Arduino.h>
#include <EasyButton.h> // https://github.com/evert-arias/EasyButton
#include <FastLED.h> // https://github.com/FastLED/FastLED/
#include "HID-Project.h"


// https://github.com/NicoHood/HID
// Pin for RGB LED control
#define RGB_PIN 9
#define LED_TYPE NEOPIXEL
#define NUM_LEDS 4



// Array which holds each LED's RGB value
CRGB leds[NUM_LEDS];

// These control the global RGB
byte RGB_BRIGHTNESS = 20;
byte RGB_MODE = 5;
byte RGB_ON_PUSH = 1;
byte RGB_SPEED = 25;

// This is used for the rolling rainbow effect
uint8_t gHue = 0;

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
int pins[9] = {
    16,
    14,
    15,
    10,
    8,
    7,
    6,
    5,
    4
};
bool isPressed[10];
bool wasPressed[10];
bool wasReleased[10];

void basicButt(bool read_b);

unsigned long current_millis = 0;

#define IDE_SWITCH 3
#define IDE_SWITCH_LED 3

void setup() { // put your setup code here, to run once:
    delay(1000);

    for (int i = 0; i < 10; i ++) {
        isPressed[i] = false;
        wasPressed[i] = false;
        wasReleased[i] = false;
    }
    FastLED.addLeds<LED_TYPE, RGB_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(RGB_BRIGHTNESS);

    // Getting the button array ready and then started (pullups and the like)
    basicButt(false);
}

void loop() {
    // put your main code here, to run repeatedly:

    // Read every button and put the values in the right arrays
    basicButt(true);
    current_millis = millis();
    for (int i = 0; i < 9; i ++) {
        // buttons[i]->read();
        // pressed[i] = buttons[i]->wasPressed();
    }


    // LED Stuff to *always* run

    EVERY_N_MILLISECONDS(RGB_SPEED) {
        gHue ++;
    };

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
            fill_gradient_RGB(leds, NUM_LEDS, CHSV(gHue, 255, 255), CHSV(gHue + 10, 255, 255));
            break;

        case 6:
            fill_gradient_RGB(leds, NUM_LEDS, CHSV(gHue, 255, 255), CHSV(gHue + 70, 255, 255));
            break;

        case 7:
            fill_gradient_RGB(leds, NUM_LEDS, CHSV(gHue, 255, 255), CHSV(gHue + 120, 255, 255));
            break;

        case 8:
            fill_gradient_RGB(leds, NUM_LEDS, CHSV(-gHue, 255, 255), CHSV(-gHue + 10, 255, 255));
            break;

        case 9:
            fill_gradient_RGB(leds, NUM_LEDS, CHSV(-gHue, 255, 255), CHSV(-gHue + 70, 255, 255));
            break;

        case 10:
            fill_gradient_RGB(leds, NUM_LEDS, CHSV(-gHue, 255, 255), CHSV(-gHue + 120, 255, 255));
            break;

        default:
            break;
    }

    // If the on_push option is enabled
    if (RGB_ON_PUSH != 0) {
        for (int i = 0; i < NUM_LEDS; i ++) {
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

    if(isPressed[IDE_SWITCH]){
      leds[IDE_SWITCH_LED] = CRGB(0x001010);
    } else {
      leds[IDE_SWITCH_LED] = CRGB(0x001000);
    }

    // Done with RGB stuff, send it over to the LEDs.
    FastLED.show();

    for (int i = 0; i < 9; i ++) {
        if (wasPressed[i]) {

            if (!isPressed[IDE_SWITCH]) { // Arduino Compile
                if (i == 0) {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press('r');
                    Keyboard.releaseAll();
                }

                // Regular Arduino Upload
                if (i == 1) {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press('u');
                    Keyboard.releaseAll();
                }

                // Arduino Upload with Programmer
                if (i == 2) {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_SHIFT);
                    Keyboard.press('u');
                    Keyboard.releaseAll();
                }
            } else {
                // PlatformIO Compile
                if (i == 0) {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.press('b');
                    Keyboard.releaseAll();
                }

                // Regular PlatformIO Upload
                if (i == 1) {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_ALT);
                    Keyboard.press('u');
                    Keyboard.releaseAll();
                }

                // PlatformIO Upload with Programmer
                if (i == 2) {
                    Keyboard.press(KEY_LEFT_CTRL);
                    Keyboard.press(KEY_LEFT_SHIFT);
                    Keyboard.press('p');
                    Keyboard.releaseAll();
                    delay(350);
                    Keyboard.println("tasks");
                    delay(350);
                    Keyboard.println("programmer");
                }
            }
        }
        if (wasReleased[i]) {}
    }

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
