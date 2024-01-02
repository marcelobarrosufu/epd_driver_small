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
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "epd_colors.h"
#include "epd_font.h"
#include "epd_fb.h"

#include "fonts/epd_font_06x08.h"
#include "fonts/epd_font_08x12.h"
#include "fonts/epd_font_12x16.h"
#include "fonts/epd_font_16x24.h"
#include "fonts/epd_font_26x40.h"
#include "fonts/epd_font_lcd_22_32.h"

static void epd_font_term_06x08_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background);
static void epd_font_term_08x12_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background);
static void epd_font_term_12x16_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background);
static void epd_font_term_16x24_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background);
static void epd_font_lcd_26x40_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background);
static void epd_font_lcd_22x32_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background);

const epd_font_t epd_font_db[] =
{
		[EPD_FONT_TERMINAL_06X08] = { .data = (uint8_t *) epd_font_06x08, .width = 6,  .height = 8,  .first_char = 32, .last_char = 255, .bytes_per_char = 6,   .draw = epd_font_term_06x08_draw },
		[EPD_FONT_TERMINAL_08X12] = { .data = (uint8_t *) epd_font_08x12, .width = 8,  .height = 12, .first_char = 32, .last_char = 255, .bytes_per_char = 16,  .draw = epd_font_term_08x12_draw },
		[EPD_FONT_TERMINAL_12X16] = { .data = (uint8_t *) epd_font_12x16, .width = 12, .height = 16, .first_char = 32, .last_char = 255, .bytes_per_char = 24,  .draw = epd_font_term_12x16_draw },
		[EPD_FONT_TERMINAL_16X24] = { .data = (uint8_t *) epd_font_16x24, .width = 16, .height = 24, .first_char = 32, .last_char = 255, .bytes_per_char = 48,  .draw = epd_font_term_16x24_draw },
		[EPD_FONT_LCD_26X40]      = { .data = (uint8_t *) epd_font_26x40, .width = 26, .height = 40, .first_char = 44, .last_char = 57,  .bytes_per_char = 130, .draw = epd_font_lcd_26x40_draw  },
		[EPD_FONT_LCD_22X32]      = { .data = (uint8_t *) epd_font_22x32, .width = 22, .height = 32, .first_char = 44, .last_char = 57,  .bytes_per_char = 88,  .draw = epd_font_lcd_22x32_draw  },
		[EPD_FONT_NUM_FONTS]      = { .data = 0, .width = 16, .height = 0, .draw = 0},
};

epd_font_t epd_font_get_info(epd_font_name_t font_name)
{
	if(font_name >= EPD_FONT_NUM_FONTS)
		return epd_font_db[EPD_FONT_NUM_FONTS];
	else
		return epd_font_db[font_name];
}

static void epd_font_draw(const epd_font_t *pf, epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background)
{
	if((chr < pf->first_char) || (chr > pf->last_char))
		return;

	uint16_t chr_offset = chr - pf->first_char;
	epd_fb_colors_t bg_color = color == EPD_DB_COLOR_BLACK ? EPD_DB_COLOR_WHITE : EPD_DB_COLOR_BLACK;
	uint8_t bytes_per_col = pf->bytes_per_char/pf->width;

	for(size_t col = 0; col < pf->width ; col++)
	{
		for(size_t byte_ofs = 0; byte_ofs < bytes_per_col ; byte_ofs++)
		{
			uint8_t line = *(pf->data + chr_offset*pf->bytes_per_char + col*bytes_per_col + byte_ofs);

			for(size_t row = 0; row < 8; row++)
			{
				bool bit_val = ((line >> row) & 0x01) > 0;
				if(bit_val)
					epd_fb_point(fb,x + col, y + row + 8*byte_ofs,color);
				else if(paint_background)
					epd_fb_point(fb,x + col, y + row + 8*byte_ofs,bg_color);
			}
		}
	}
}

void epd_font_term_06x08_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background)
{
	const epd_font_t *pf = &epd_font_db[EPD_FONT_TERMINAL_06X08];

	epd_font_draw(pf,fb,x,y,chr,color,paint_background);
}

static void epd_font_term_08x12_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background)
{
	const epd_font_t *pf = &epd_font_db[EPD_FONT_TERMINAL_08X12];

	epd_font_draw(pf,fb,x,y,chr,color,paint_background);
}

static void epd_font_term_12x16_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background)
{
	const epd_font_t *pf = &epd_font_db[EPD_FONT_TERMINAL_12X16];

	epd_font_draw(pf,fb,x,y,chr,color,paint_background);
}

static void epd_font_term_16x24_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background)
{
	const epd_font_t *pf = &epd_font_db[EPD_FONT_TERMINAL_16X24];

	epd_font_draw(pf,fb,x,y,chr,color,paint_background);
}

static void epd_font_lcd_26x40_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background)
{
	const epd_font_t *pf = &epd_font_db[EPD_FONT_LCD_26X40];

	epd_font_draw(pf,fb,x,y,chr,color,paint_background);
}

static void epd_font_lcd_22x32_draw(epd_fb_t *fb, uint16_t x, uint16_t y, uint8_t chr, epd_fb_colors_t color, bool paint_background)
{
	const epd_font_t *pf = &epd_font_db[EPD_FONT_LCD_22X32];

	epd_font_draw(pf,fb,x,y,chr,color,paint_background);
}
