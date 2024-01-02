/*
MIT License

Copyright (c) 2023- Marcelo Barros de Almeida <marcelobarrosalmeida@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#pragma once

typedef enum epd_font_name_e
{
	EPD_FONT_TERMINAL_06X08 = 0,
	EPD_FONT_TERMINAL_08X12,
	EPD_FONT_TERMINAL_12X16,
	EPD_FONT_TERMINAL_16X24,
	EPD_FONT_LCD_26X40,
	EPD_FONT_LCD_22X32,
	EPD_FONT_NUM_FONTS,
} epd_font_name_t;

struct epd_fb_s;

typedef struct epd_font_s
{
	const uint8_t *data;
	uint8_t width;
	uint8_t height;
	uint8_t first_char;
	uint8_t last_char;
	uint8_t bytes_per_char;
	void (*draw)(struct epd_fb_s *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background);
} epd_font_t;

epd_font_t epd_font_get_info(epd_font_name_t font_name);
