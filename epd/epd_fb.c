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
#include <stdlib.h>
#include <string.h>
#include "epd_colors.h"
#include "epd_font.h"
#include "epd_fb.h"

#define EPD_FB_SWAP_U16(a,b) { uint16_t w = (a); (a) = (b) ; (b) = (w); }

static bool epd_fb_check_limits(epd_fb_t *fb, uint16_t x, uint16_t y)
{
	return ((x < fb->height) && (y < fb->width));
}

static void epd_fb_abs_point(epd_fb_t *fb, uint16_t x, uint16_t y, epd_fb_colors_t color)
{
	if(color == EPD_DB_COLOR_WHITE)
	{
		fb->image[(x + y * fb->width) / 8] &= ~(0x80 >> (x % 8));
	}
	else if(color == EPD_DB_COLOR_BLACK)
	{
		fb->image[(x + y * fb->width) / 8] |= 0x80 >> (x % 8);
	}
}

void epd_fb_point(epd_fb_t *fb, uint16_t x, uint16_t y, epd_fb_colors_t color)
{
	if(epd_fb_check_limits(fb,x,y) == false)
		return;

	epd_fb_abs_point(fb,x,y,color);
}

void epd_fb_line(epd_fb_t *fb, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, epd_fb_colors_t color)
{
    if((x1 == x2) && (y1 == y2))
    {
    	epd_fb_point(fb,x1,y1,color);
    }
    else if(x1 == x2)
    {
        if(y1 > y2)
        {
        	EPD_FB_SWAP_U16(y1, y2);
        }
        for(uint16_t y = y1; y <= y2; y++)
        {
        	epd_fb_point(fb,x1,y,color);
        }
    }
    else if(y1 == y2)
    {
        if(x1 > x2)
        {
        	EPD_FB_SWAP_U16(x1, x2);
        }
        for(uint16_t x = x1; x <= x2; x++)
        {
        	epd_fb_point(fb,x,y1,color);
        }
    }
    else
    {
        int16_t wx1 = (int16_t)x1;
        int16_t wx2 = (int16_t)x2;
        int16_t wy1 = (int16_t)y1;
        int16_t wy2 = (int16_t)y2;

        bool flag = abs(wy2 - wy1) > abs(wx2 - wx1);
        if(flag)
        {
        	EPD_FB_SWAP_U16(wx1, wy1);
        	EPD_FB_SWAP_U16(wx2, wy2);
        }

        if(wx1 > wx2)
        {
        	EPD_FB_SWAP_U16(wx1, wx2);
        	EPD_FB_SWAP_U16(wy1, wy2);
        }

        int16_t dx = wx2 - wx1;
        int16_t dy = abs(wy2 - wy1);
        int16_t err = dx / 2;
        int16_t ystep;

        if(wy1 < wy2)
        {
            ystep = 1;
        }
        else
        {
            ystep = -1;
        }

        for(; wx1 <= wx2; wx1++)
        {
            if (flag)
            {
            	epd_fb_point(fb,wy1,wx1,color);
            }
            else
            {
            	epd_fb_point(fb,wx1,wy1,color);
            }

            err -= dy;
            if (err < 0)
            {
                wy1 += ystep;
                err += dx;
            }
        }
    }
}

void epd_fb_rect(epd_fb_t *fb, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, epd_fb_colors_t color, bool fill)
{
    if(fill == false)
    {
    	epd_fb_line(fb, x1, y1, x1, y2, color);
    	epd_fb_line(fb, x1, y1, x2, y1, color);
    	epd_fb_line(fb, x1, y2, x2, y2, color);
    	epd_fb_line(fb, x2, y1, x2, y2, color);
    }
    else
    {
        if(x1 > x2)
        {
        	EPD_FB_SWAP_U16(x1, x2);
        }
        if(y1 > y2)
        {
        	EPD_FB_SWAP_U16(y1, y2);
        }
        for(uint16_t x = x1; x <= x2; x++)
        {
            for(uint16_t y = y1; y <= y2; y++)
            {
            	epd_fb_point(fb, x, y, color);
            }
        }
    }
}

static void epd_fb_calc_anchor(epd_fb_t *fb, uint16_t *x, uint16_t *y, uint16_t w, uint16_t h,epd_fb_anchor_t anchor)
{
	int32_t nx, ny;

	switch(anchor)
	{
	case EPD_FB_ANCHOR_N:
		nx = *x - w/2;
		ny = *y;
		break;
	case EPD_FB_ANCHOR_NE:
		nx = *x - w;
		ny = *y;
		break;
	case EPD_FB_ANCHOR_CENTER:
		nx = *x - w/2;
		ny = *y - h/2;
		break;
	case EPD_FB_ANCHOR_W:
		nx = *x;
		ny = *y - h/2;
		break;
	case EPD_FB_ANCHOR_E:
		nx = *x - w;
		ny = *y - h/2;
		break;
	case EPD_FB_ANCHOR_SW:
		nx = *x;
		ny = *y - h;
		break;
	case EPD_FB_ANCHOR_S:
		nx = *x - w/2;
		ny = *y - h;
		break;
	case EPD_FB_ANCHOR_SE:
		nx = *x - w;
		ny = *y - h;
		break;
	case EPD_FB_ANCHOR_NW:
	default:
		nx = *x;
		ny = *y;
		break;
	}
	nx = nx < 0 ? 0 : nx;
	nx = nx >= fb->width ? fb->width-1  : nx;
	ny = ny < 0 ? 0 : ny;
	ny = ny >= fb->height ? fb->height-1  : ny;

	*x = (uint16_t) nx;
	*y = (uint16_t) ny;
}

void epd_fb_str(epd_fb_t *fb, uint16_t x, uint16_t y, epd_fb_anchor_t anchor, uint8_t *str, epd_font_name_t epd_font, epd_fb_colors_t color, bool paint_background)
{
	epd_font_t font_info = epd_font_get_info(epd_font);

	if(font_info.data == NULL)
		return;

	uint32_t len = strlen((char *)str);
	epd_fb_calc_anchor(fb,&x,&y,font_info.width*len,font_info.height,anchor);
	uint8_t *pc = str;
	uint16_t px = x;

	while(len--)
	{
		font_info.draw(fb,px,y,*pc++,color,paint_background);
		px += font_info.width;
	}
}

void epd_fb_img_overlay(epd_fb_t *fb, uint8_t *img, uint16_t x, uint16_t y, uint16_t xw, uint16_t yh)
{
	uint8_t *pimg = img;
	uint8_t bit_pos = 7;

	for(uint16_t row = y ; row < (y + yh) ; row++)
	{
		for(uint16_t col = x ; col < (x + xw) ; col++)
		{
			bool val = ((*pimg) >> bit_pos) & 0x01;
			epd_fb_point(fb,col,row,val ? EPD_DB_COLOR_BLACK : EPD_DB_COLOR_WHITE);
			if(bit_pos == 0)
			{
				bit_pos = 7;
				pimg++;
			}
			else
				bit_pos--;

		}
	}
}

epd_fb_t epd_fb_init(uint8_t *buffer, uint16_t width, uint16_t height)
{
	epd_fb_t fb = { 0 };

	fb.image = buffer;
	fb.height = height;
	fb.width = width % 8 ? width + 8 - (width % 8) : width; // round to 8bits/byte

	return fb;
}
