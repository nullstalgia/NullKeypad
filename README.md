# NullKeypad

An Arduino Pro Micro-based keypad made with 9 Cherry-style switches, an I2C OLED Panel, and WS2812B RGB LEDs  (3535 sized)

RGB Control Menu:

![](https://i.imgur.com/RUK60wB.jpg) 

## Modes:
In order of appearance:

1. F13-21 Keys

	a. Each key is hooked up to the extra Function keys in sequential order

![](https://i.imgur.com/YTGj252.jpg) 

2. RGB Settings

	a. Brightness Adjust

	b. RGB "Mode" (what kind of LED Effect, shown below)

	c. Push Action (what happens when button is pressed)

	d. Speed (how fast the Rainbow effect cycles, higher option is slower)

	e. Test + Info (Shows current settings and ignores inputs so you can test without affecting PC)

![](https://i.imgur.com/vA60KZu.jpg ) 

3. Arrow Keys

	a. WASD-style arrow keys with extra keys

	b. Extra keys: TAB, ESC, Left Control, Left Alt, Return/Enter

![](https://i.imgur.com/16QXEe1.jpg) 

4. Counter

	a. Is used as a basic counter that saves to EEPROM

	b. Secret functions are listed below

![](https://i.imgur.com/C6YQLYD.jpg) 

5. Media Keys

	a. Volume +/-

	b. Mute

	c. Previous/Next

	d. Play/Pause

	e. Rewind/Fast Forward

	f. Stop

![](https://i.imgur.com/4AAPE3a.jpg) 

6. Macros

	a. Are pre-programmed macros that I add/use as I see fit. Such as...

	b. Arduino Compile

	c. Arduino Upload

	d. Arduino Upload via Programmer

	e. "F"

![](https://i.imgur.com/rywaGZF.jpg) 

7. Mouse

	a. Acts as a USB mouse. Default mode is WASD style.
	
	b. Has Left, Right, and Middle click functions
	
	c. Adjustable speed
	
	d. Secret functions listed below

![](https://i.imgur.com/IaQaNMY.jpg) 

8. WASD/PC Game Mode

	a. Mouse 1/2
	
	b. W, A, S, D
	
	c. E, F, Space

![](https://i.imgur.com/Uq4xCJy.jpg) 

9. TODO? Gamepad/XInput Mode

## RGB Modes:
Mode 0 is Black
1. White
2. Red
3. Green
4. Blue
5. Rainbow (all keys same color, very small gradient)
6. Rainbow (slight gradient)
7. Rainbow (larger gradient)
8. 8 through 10, same as 5 through 7, but the Rainbow is reversed

## Secret Modes:

#### Counter:
- Hold 7 (index 6) to activate Hex counting
- Hold 9 (index 8) to activate Binary counting
- In any mode, hold top button of a column and press the up/down of that column to raise/lower by a larger amount

![](https://i.imgur.com/kDiVZ4S.jpg) 

#### Mouse:
- Hold 7 (index 6) to enable Mouse Wheel Scrolling. Hold the Mouse Wheel key and press Up or Down to scroll. More speed == More scroll
- Hold 9 (index 8) to activate Toggling. All mouse keys (including mouse wheel enable) will be toggled upon pressing.

- Hold 1 (index 0) to activate Border Mouse mode, all keys except 5 (index 4) move the mouse in the corresponding direction. The middle key will press Mouse 1 (left click)
    - Hold 3 (index 2) to set Speed to 1 for this mode.

![](https://i.imgur.com/l4nUslG.jpg)

#### WASD:
- Hold 7 (index 6) to enable Horizontal Mouse mode. Pressing A and D on the Keypad will instead move the mouse in the corresponding direction.

## Wiring:

#### I2C OLED:

| Name | Pin # |
|------|-------|
| SDA  | 2     |
| SCL  | 3     |

#### RGB LEDs:

| Name | Pin # |
|------|-------|
| Data Out  | 9     |

#### Buttons/Keys:

Arranged in a  3x3 grid.

| Button # | Index | Pin # |
|----------|:-----:|------:|
| 1        | 0     | 16    |
| 2        | 1     | 14    |
| 3        | 2     | 15    |
| 4        | 3     | 10    |
| 5        | 4     | 8     |
| 6        | 5     | 7     |
| 7        | 6     | 6     |
| 8        | 7     | 5     |
| 9        | 8     | 4     |

## Required Libraries For Compilation:
[NicoHood's HID-Project](https://github.com/NicoHood/HID) 
[lexus2k's ssd1306](https://github.com/lexus2k/ssd1306/) 
[evert-arias's EasyButton](https://github.com/evert-arias/EasyButton) 
[FastLED](https://github.com/FastLED/FastLED/) 
[MemoryFree](https://github.com/maniacbug/MemoryFree) 

## V1 and V2 PCB Difference?

All V1 code is compatible with V2, but V2 has 4 extra pushbuttons, so not everything will work.

All of the Cherry MX style keys, RGB, and OLED pins are kept the same.

## PCB V2 Images:

Coming Soon

## PCB V1 Images:

![](https://i.imgur.com/dCcjW8I.jpg) 

![](https://i.imgur.com/m2erbym.jpg) 