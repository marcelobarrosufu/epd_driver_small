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

typedef struct epd_fb_s
{
	uint8_t *image;
    uint16_t width;
    uint16_t height;
} epd_fb_t;

// The library positioning follows the idea 
// of anchoring like we can find in TclTk.
// Your text position is defined by the anchor so
// it is easier to position text in the screen.
typedef enum epd_fb_anchor_e
{
	EPD_FB_ANCHOR_NW = 0,
	EPD_FB_ANCHOR_N,
	EPD_FB_ANCHOR_NE,
	EPD_FB_ANCHOR_W,
	EPD_FB_ANCHOR_CENTER,
	EPD_FB_ANCHOR_E,
	EPD_FB_ANCHOR_SW,
	EPD_FB_ANCHOR_S,
	EPD_FB_ANCHOR_SE,
} epd_fb_anchor_t;

void epd_fb_point(epd_fb_t *fb, uint16_t x, uint16_t y, epd_fb_colors_t color);
void epd_fb_line(epd_fb_t *fb, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, epd_fb_colors_t color);
void epd_fb_rect(epd_fb_t *fb, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, epd_fb_colors_t color, bool fill);
epd_fb_t epd_fb_init(uint8_t *buffer, uint16_t width, uint16_t height);
void epd_fb_str(epd_fb_t *fb, uint16_t x, uint16_t y, epd_fb_anchor_t anchor, uint8_t *str, epd_font_name_t epd_font,  epd_fb_colors_t color, bool paint_background);
void epd_fb_img_overlay(epd_fb_t *fb, uint8_t *img, uint16_t x, uint16_t y, uint16_t xw, uint16_t yh);
