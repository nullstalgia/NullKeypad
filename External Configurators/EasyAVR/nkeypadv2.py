"""
Keyboard definition for the NullKeypad V2 custom keyboard.
Auto-generated from '/home/tony/Downloads/keyboard-layout(3).json'
"""

#
# READ THIS
#
# This file contains a description of the various parameters that
# must be defined in order to completely describe the new keyboard.
# This file must be correct for the firmware to work, however if
# if is incorrect reprogramming the board won't cause any damage.
#
# This tool that created this file did not have all the information
# needed to make a perfect config.  Check over all the data for
# correctness.  Definitely make sure to fix the matix rows/columns in
# keyboard_definition, which are almost certainly wrong.
#
# This file uses two helper functions to make the configuration
# easier.  For more complicated hardware, it may be necessary to
# set the parameters by hand.  Look at other configs, such as
# boards/sigma.py, for examples if you want to try it.
#

# The first decision you have to make is to choose a hardware
# build.  A handwire board using a Teensy2.0 will probably want
# ATmega32U4_16MHz_TKL or ATmega32U4_16MHz_SIXTY.  The sizes are
# defined in the templates/__init__.py file of the keymapper.
# Leave the rest of the imports like they are here.

import easykeymap.templates.ATmega32U4_16MHz_SIXTY as firmware
from easykeymap.ioports import *
from easykeymap.helper import make_matrix_config, make_led_config


# The name of the board in the "New" dialog

description = "NullKeypad V2"

# Unique string to identify THIS exact hardware layout.  If you change
# this file after saving layouts in the GUI, bump the unique_id to
# tell the tool that the old save files should not be used.  (prevents
# corrupted builds)

unique_id = "NKEYPADV2_001"

# The name of the .cfg file the system will try to find for altered
# layout options.  See the configs subdir of the keymapper.

cfg_name = "nkeypadv2"


# Hand-wired boards usually use Teensy controllers.  Set this to
# True to make sure that the bootloader works.

teensy = False

# If your board has an exposed switch for going into the bootloader,
# you can set this to True and the system won't prompt you to add a
# BOOT key to your layout.

hw_boot_key = True


# The number of rows and columns in the matrix.  In a hand-wired board
# each of these will correspond to a single pin.

num_rows = 1
num_cols = 13


# Keyboards work by scanning a matrix to check each key.  The scan
# works by setting an active row/column (strobing) and then reading
# the status of every switch that crosses it (sensing).
# strobe_cols tells the firmware which direction you have your diodes
# installed.  If diodes go from column to row, then strobe_cols must
# be False.  If diodes go from row to column, then strobe_cols must be
# True.

strobe_cols = False

# strobe_low tells the firmware if a row/column should be activated
# by pulling the pin high or low.  Hand-wired boards will almost always
# use strobe_low = True

strobe_low = True


# The matrix_hardware, matrix_strobe, matrix_sense parameters tell
# the firmware how to initialize the ports, what pins must be set
# for each row/column, and what order to strobe/sense.  These are
# complicated and are explained fully elsewhere.  It is easiest to
# configure the matrix by using the make_matrix_config function as
# shown below.  Just customize 'rows' and 'cols' for your project.

matrix_hardware, matrix_strobe, matrix_sense = make_matrix_config(
    strobe_cols=strobe_cols,
    strobe_low=strobe_low,
    rows=[],
    cols=[F4, F5, F6, F7, B2, B3, B1, B6, B4, E6, D7, C6, D4],
    device=firmware.device
)


# The num_leds, num_ind, led_hardware, backlighting, num_bl_enab,
# and bl_modes parameters tell the firmware how to operate the LEDs
# for indicators (for example, Caps Lock) and for backlighting.  In
# order to fine-tune the configs, these may have to be defined manually
# but it is easiest to use make_led_config.
# LED_DRIVER_PULLUP is used when the pin is connected to the anode of
# the LED and the cathode is connected to ground.
# LED_DRIVER_PULLDOWN is used when the pin is connected to the cathode
# of the LED and the anode is connected to the power supply.
# Hand-wired boards will usually want to use LED_DRIVER_PULLDOWN.
# If there are no backlights, just leave the list empty (ie. just []).

num_leds, num_ind, led_hardware, backlighting, num_bl_enab, bl_modes = make_led_config(
    led_pins = [],
    led_dir=LED_DRIVER_PULLDOWN,
    backlight_pins = [],
    backlight_dir=LED_DRIVER_PULLDOWN
)


# Define the default assignments of the indicator LEDs.  The length
# of this list must equal the length of led_pins.  For each LED, the
# first string is the description of the key shown in the GUI.  The
# second string is the default function assigned to that LED.  LED
# functions must be strings as defined in led_assignments of gui.py.
# Choices are 'Num Lock', 'Caps Lock', 'Scroll Lock', 'Compose', 'Kana',
# 'Win Lock', 'Fn1 Active', 'Fn2 Active', 'Fn3 Active', 'Fn4 Active',
# 'Fn5 Active', 'Fn6 Active', 'Fn7 Active', 'Fn8 Active', 'Fn9 Active',
# 'Any Fn Active', 'Recording', 'USB Init', 'USB Error', 'USB Suspend',
# 'USB Normal', 'Backlight', and 'Unassigned'.

led_definition = []


# Define your layout.  This is a list of rows.  Each row is a list
# of keys.  Each key is a tuple of three items.  First item is a tuple
# defining the width,height of the key.  If it is just a number, it
# will be a space instead of a key.  All units are in quarter key lengths,
# so a standard key would be (4,4).  Second item is a tuple defining the
# row,column in the matrix for that key.  Third item is the default scancode
# for that key, from scancodes.py.  If a row is a number instead of a list,
# it will just make a vertical spacer.
# ((key width, key height), (matrix row, matrix column), 'default mapping')

keyboard_definition = [[((4, 4), (0, 0), 'HID_KEYBOARD_SC_A'),
                        ((4, 4), (0, 1), 'HID_KEYBOARD_SC_B')],
                        [((4, 4), (0, 2), 'HID_KEYBOARD_SC_L'), 
                         ((4, 4), (0, 3), 'HID_KEYBOARD_SC_R')],
                        [(8, None, '0'),
                         ((4, 4), (0, 4), 'HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS'),
                         ((4, 4), (0, 5), 'HID_KEYBOARD_SC_1_AND_EXCLAMATION'),
                         ((4, 4), (0, 6), 'HID_KEYBOARD_SC_2_AND_AT')],
                        [(8, None, '0'),
                         ((4, 4), (0, 7), 'HID_KEYBOARD_SC_3_AND_HASHMARK'),
                         ((4, 4), (0, 8), 'HID_KEYBOARD_SC_4_AND_DOLLAR'),
                         ((4, 4), (0, 9), 'HID_KEYBOARD_SC_5_AND_PERCENTAGE')],
                        [(8, None, '0'),
                         ((4, 4), (0, 10), 'HID_KEYBOARD_SC_6_AND_CARET'),
                         ((4, 4), (0, 11), 'HID_KEYBOARD_SC_7_AND_AND_AMPERSAND'),
                         ((4, 4), (0, 12), 'HID_KEYBOARD_SC_8_AND_ASTERISK')]]


# Just leave this here as-is.
KMAC_key = None
