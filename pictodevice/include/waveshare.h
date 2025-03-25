#include "Display_ST7789.h"

// NOTE: LCD_HEIGHT and LCD_WIDTH are switched around
// in this case waveshare: LCD_HEIGHT=320 and LCD_WIDTH=240
// because the screen is rotated!
#define MY_WIDTH  LCD_HEIGHT
#define MY_HEIGHT LCD_WIDTH

int16_t middle_box_width = 146;
int16_t middle_box_height = 146;