# NullKeypad V2.1
### ~~For gamers, by gamers~~

Keypad layout:


|   |   |   |   |   |
|---|---|---|---|---|
| A | B |   |   |   |
| L | R |   |   |   |
|   |   | 1 | 2 | 3 |
|   |   | 4 | 5 | 6 |
|   |   | 7 | 8 | 9 |

This layout will be used to show what buttons do what. Check Main Menu for example.

## Main Menu
![Main Menu](https://i.imgur.com/Fli39si.jpg)Pretty self explanatory.

After choosing an option, you can always return to this menu by pressing **Reset**  once on the top left of the unit.

Navigation through this menu (and in Settings, the only other navigable menu) is done with:



|     |     |     |      |     |
|-----|-----|-----|------|-----|
| Sel | N/A |     |      |     |
| N/A | N/A |     |      |     |
|     |     | N/A | Up   | N/A |
|     |     | N/A | Sel  | N/A |
|     |     | N/A | Down | N/A |


## F13-21 Keys

![F13 through F21 menu](https://i.imgur.com/yeboOYA.jpg)
**Important Note:** The control information in this section is applicable to every other mode!

Most standard keyboards have F1 through F12 keys. However, the F keys go up to F24 (and I have seen this on some Apple keyboards).

This is an easy way to get access to those!

This mode is very simple, pushing a button on the keypad will light up the pushed button on screen and send the button command to your computer. Top left button is the top left text (F13) on the screen. The same applies to pretty much every mode on the keypad. So if you forget what button does what, look at the screen!




([F22-24 can be enabled](https://i.imgur.com/sLxJfZK.jpg) in Settings, or by pressing R (`>`) when in the mode.)

## Arrow Keys
![Arrow Keys menu](https://i.imgur.com/a8PC4KJ.jpg)

Useful for navigating a PC BIOS while on the floor looking up at the monitor.

## Mouse

![Mouse Mode Menu](https://i.imgur.com/Ka9kbcS.jpg)
This mode moves your mouse around by pressing the shown arrow keys. 

Pushing L (`<`) or R (`>`) on the top left button cluster will lower and raise the mouse speed, respectively.

|    |                          |
|----|--------------------------|
| M1 | Left Click               |
| M2 | Right Click              |
| MW | Mouse Wheel/Middle Click |
| M4 | "Back" or Mouse 4        |
| M5 | "Forward" or Mouse 5     |

MW has multiple functions. Normally, pressing it will do a normal mouse Middle Click (Mouse 3). When the Scroll Wheel option is enabled under the Mouse Settings, its functionality is replaced to scroll the mouse wheel up and down when the Up and Down directions are pressed. Speed of the scroll is dependent on set mouse speed. The mouse cursor does not move when scrolling the wheel.

There is also "Border Mouse" mode, enabled by pressing L (`<`) and R (`>`) at the same time. It can also be toggled in the Mouse Settings. 

![Border Mouse Mode](https://i.imgur.com/CNT4Bs4.jpg)
When in this mode, to go down, you push the lower-most button(s). To go left, push left-most. Up and right, push the top-right button. Middle of the keypad will Left Click.

## Media Keys

![Media Keys Menu](https://i.imgur.com/kmYC5IK.jpg)
|     |     |                |            |              |
|-----|-----|----------------|------------|--------------|
| N/A | N/A |                |            |              |
| N/A | N/A |                |            |              |
|     |     | Volume Up      | Mute       | Volume Down  |
|     |     | Previous Track | Play/Pause | Next Track   |
|     |     | Rewind         | Stop       | Fast Forward |

## WASD

![WASD Mode](https://i.imgur.com/D5TpE7E.jpg)
This mode is meant to act as a simple WASD replacement. It won't work for all games, but I built it with Rainbow 6: Siege in mind, namely the Attack's Drone stage (best in combination with WASD Mouse)

### WASD Mouse

![WASD Mouse](https://i.imgur.com/zBrIKUY.jpg)

If WASD Mouse is enabled in the Keyboard Settings, A and D will instead move the mouse Left and Right, and C and CTRL are replaced in favor of mouse speed controls.

## Simple Macros

Pushing L (`<`) and R (`>`) will change pages.

### PC Keep-Awake Macros

![PC Keep-Awake Macros](https://i.imgur.com/vSLQM5U.jpeg)
Pushing the Top Middle button will enable the Keyboard Keep-Awake. That keep-awake alternates between pushing the Left and Right arrow keys (as of writing, as fast as it can).

Pushing the Middle keypad button will enable the Mouse Keep-Awake. This one quickly wiggles the mouse up and down in a tight area. Tight enough that you might not need to turn it off if you need to adjust something on the host device.

### Arduino IDE Upload Macros

![Arduino IDE Upload Macros](https://i.imgur.com/4mVajiB.jpeg)
Macros for the Arduino IDE:

| Label          | Keyboard Shortcut      | Explanation                                 |
|----------------|------------------------|---------------------------------------------|
| Upload W/ Prog | Ctrl+Shift+U           | Uploads to AVR with set Programmer in Tools |
| Upload         | Ctrl+U                 | Upload with Arduino IDE                     |
| Build          | Ctrl+R                 | Verify/Compile                              |
| Bootl          | Alt+T, Up Arrow, Enter | Flashes bootloader and fuses to AVR         |
| Bin            | Ctrl+Alt+S             | Export binary (.hex) to Sketch folder       |

### PlatformIO Macros

![PlatformIO Macros](https://i.imgur.com/jAZss94.jpeg)
| Label          | Keyboard Shortcut                   | Explanation                                          |
|----------------|-------------------------------------|------------------------------------------------------|
| Upload W/ Prog | Ctrl+Shift+P, "tasks", "programmer" | Uploads to AVR with set Programmer in platformio.ini |
| Upload         | Ctrl+Alt+U                          | Upload with PlatformIO                               |
| Build          | Ctrl+Alt+B                          | Verify/Compile                                       |

## Settings

![Main Settings Menu](https://i.imgur.com/7LlMP3t.jpeg)

Main Settings Menu, same controls as the Main Menu.

### Keyboard Settings

![Keyboard Settings Menu](https://i.imgur.com/k9BEK7f.jpeg)

 - Toggle Buttons
	 - If enabled, When you push a keyboard button on the keypad with this enabled, it will be held down until you either push the same keypad key again, or push and release that same button on another keyboard.
 - WASD Mouse
	 - If enabled, A and D will instead move the mouse Left and Right, and C and CTRL are replaced in favor of mouse speed controls.
- Enable F22-F24 
	- Enables the 3 remaining buttons on the left button cluster.
	- https://i.imgur.com/sLxJfZK.jpeg

### Mouse Settings

![Mouse Settings Menu](https://i.imgur.com/1IZiLmj.jpeg)

 - Toggle Buttons
	 - Same as on the Keyboard, but it also will toggle the functionality of the Mouse Wheel enable.
 - Toggle Movement
	 - When you push a direction, it will be held down until you push that same direction again.
 - Scroll Wheel
	 - When enabled, MW's function is replaced to instead scroll the mouse wheel up and down when the Up and Down directions are pressed. Speed of the scroll is dependent on set mouse speed. The mouse cursor does not move when scrolling the wheel.
 - Border Mouse
	 - When enabled, the keypad will go into Border Mouse mode by default when opening the Mouse.

### RGB Settings

![RGB Settings](https://i.imgur.com/ERg98wm.jpeg)

 - Brightness
	 - From 0 (off) to 10.
 - Lighting Modes:
	 - None ("Black")
	 - White
	 - Red
	 - Green
	 - Blue
	 - Rainbow
		 - All keys change to the same, shifting, color
	 - Rainbow Ribbon
		 - Colors gradually move from bottom to top
	 - Rainbow Long Ribbon
		 - Same as the ribbon, but the distance between hues is larger between keys.
	 - The last 3 repeated, but from top to bottom.
 - Push Action: How the LED under the button reacts when you press it
	 - User -> White
		 - Normally, button is your set color, pressing sets it to White.
	- User -> Black
		 - Normally, button is your set color, pressing sets it to Black (off).
	 - White -> User
		 - Normally, button is White, pressing it sets it to your set color.
	 - Black -> User
		 - Normally, button is Black, pressing it sets it to your set color.
 - Speed (of the rainbow)
	 - Reset is required to see the change!
	 - From 10 to 0 (0 is not off, just quite slow)
 - Test + Info
	 - A sandbox for you to push all buttons (except Reset) without messing anything up and looking at the pretty RGB.

## Secret Functions

- When in a keyboard-using mode, if you Hold **B** and press R (`>`), it will enable keyboard button toggling. Holding **B** and pressing L (`<`) will disable.
- When in the Mouse mode, pressing L (`<`) and R (`>`) together will toggle the Border Mouse mode.

## Firmware Update

Pushing **Reset** twice quickly (but not TOO quickly) will put the device into LUFA DFU Update mode. You'll know for sure if the two red LEDs on the Arduino board are flashing alternately. 

Pushing it again once or power-cycling the keypad should take it out of that mode. If not, you might have a corrupted firmware ~~(if so, how???)~~. Clearing the EEPROM and reflashing it with your compiled version/my provided .hex should clear that up.

Device info:

atmega32u4 @ 16MHZ External Crystal, 5V, 2048 word bootloader

Some tools you can use are:

https://github.com/dfu-programmer/dfu-programmer

https://github.com/nullstalgia/dfu-progui (A GUI I wrote for the above)

Note for Linux users: You might need a udev rule (example below) or to run the flasher as root if it doesn't work.

`/etc/udev/rules.d/50-atmel-dfu.rules`

```
# Atmel ATMega32U4
# SUBSYSTEMS=="usb", ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="2ff4", MODE:="0666"
# Atmel USBKEY AT90USB1287
# SUBSYSTEMS=="usb", ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="2ffb", MODE:="0666"
# Atmel ATMega32U2
# SUBSYSTEMS=="usb", ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="2ff0", MODE:="0666"
# Atmel ATMega16U2
# SUBSYSTEMS=="usb", ATTRS{idVendor}=="03eb", ATTRS{idProduct}=="2fef", MODE:="0666"

# Atmel DFU Vendor
SUBSYSTEMS=="usb", ATTRS{idVendor}=="03eb", MODE:="0666"
# HORI Vendor
SUBSYSTEMS=="usb", ATTRS{idVendor}=="0f0d", MODE:="0666"
# Maple Leaf Vendor
SUBSYSTEMS=="usb", ATTRS{idVendor}=="1eaf", MODE:="0666"
# STM32 HID Bootloader
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2019", ATTRS{idProduct}=="beba", MODE:="0666"
```

(This does not set it to a specific group or user, and leaves those USB devices open to *everyone* on your machine.)

You can use also some tools for updating custom mechanical keyboards, as they use the same chip and very similar bootloaders!

Example:

https://github.com/qmk/qmk_toolbox (Should be easy to use for Windows users! Recommended with the Auto Flash checkboxes!)

# Compiling

Ok so, this is an issue for a couple reasons.

When compiling normally with PlatformIO, I get this

![Program size greater than max error](https://i.imgur.com/CrDiOdp.png)
Also, when using this as a keypad during development, having the extra Serial port dangling can be an issue (especially if writing for the same Arduino variant, but for a different project).

The Arduino IDE was able to give me smaller compiled binaries, and I was able to strip out the Serial/CDC port. no_serial_micro in [this repository here](https://github.com/nullstalgia/arduino-hardware-profiles).

I will always provide a .hex when I update code, so you do not need to compile it yourself. You also don't *need* the same bootloaders I use (however, I highly recommend it for reliability and speed purposes).

> Written with [StackEdit](https://stackedit.io/).
