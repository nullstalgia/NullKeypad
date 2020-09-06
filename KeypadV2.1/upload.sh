#!/bin/bash
dfu-programmer $1 erase
dfu-programmer $1 flash $2
dfu-programmer $1 launch
