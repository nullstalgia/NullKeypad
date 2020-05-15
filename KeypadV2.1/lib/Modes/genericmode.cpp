#include "genericmode.h"


void Mode::printInvertingButton(uint8_t x, uint8_t y, const char *text,
                                uint8_t button_index, bool force_on) {
  printInvertingButton(x, y, text, button_index, _grid_width, _grid_height, force_on);
}

void Mode::printInvertingButton(uint8_t x, uint8_t y, const char *text,
                                uint8_t button_index, uint8_t grid_width,
                                uint8_t grid_height, bool force_on) {
  _Display->positiveMode();
  if (force_on || _Buttons->isPressed[button_index]) {
    _Display->negativeMode();
  }
  uint8_t new_x, new_y;
  new_x = x * grid_width;
  new_y = y * grid_height;
  _Display->printFixed(new_x, new_y, text, STYLE_NORMAL);
}

void Mode::printInvertingButton(const char *text, uint8_t button_index, bool force_on) {
  uint8_t new_x, new_y;
  if (!_ablr_buttons) {
    if (button_index <= 8) {
      // Assuming 3x3 grid without ABLR buttons

      // Getting X
      new_x = button_index % 3;

      new_x *= 4;

      new_x += 3;

      // Getting Y
      new_y = button_index / 3;

      new_y *= 3;

      new_y++;
    } else {
      return;
    }
  } else {
    if (button_index < 9) {
      // Getting X
      new_x = button_index % 3;

      new_x *= 4;

      new_x += 4;

      if(_grid_width < 8){
        new_x += 2;
      }

      // Getting Y
      new_y = button_index / 3;

      new_y *= 2;

      new_y += 3;
    } else {
      uint8_t smaller_button_index;
      smaller_button_index = button_index - 9;
      new_x = (smaller_button_index % 2);
      if (new_x) {
        new_x += 2;
      }
      new_y = smaller_button_index / 2;
      // new_y += 1;
      // char buffer[10];
      // sprintf(buffer, "x:%d y:%d", new_x, new_y);
      // printInvertingButton(0, smaller_button_index, buffer, button_index);
      printInvertingButton(new_x, new_y, text, button_index, 6, 8, force_on);
      return;
      // return;
      // new_x += 3;
      // new_y += 1;

      // new_y = button_index * 3;
      // new_x = 0;
    }
  }

  printInvertingButton(new_x, new_y, text, button_index, force_on);
}