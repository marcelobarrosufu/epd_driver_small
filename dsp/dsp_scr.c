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

#include "epd_cfg.h"
#include "epd_colors.h"
#include "epd_font.h"
#include "epd_fb.h"
#include "epd_drv.h"
#include "dsp_scr.h"
#include "dsp_imgs.h"

// this file is just an example of how to use the library,
// create or personalize it

// display buffers managment: you need two display buffers for fast update
static uint8_t scr_a[DSP_SCR_WIDTH*DSP_SCR_HEIGHT/8];
static uint8_t scr_b[DSP_SCR_WIDTH*DSP_SCR_HEIGHT/8];
static epd_fb_t fb_a;
static epd_fb_t fb_b;
static epd_fb_t *fbs[] = { &fb_a, &fb_b };
static uint8_t fb_current = 0;

static epd_fb_t *dsp_scr_swap_fb(void)
{
	fb_current = fb_current == 0 ? 1 : 0;
	return fbs[fb_current];
}

static epd_fb_t *dsp_scr_get_fb(void)
{
	return fbs[fb_current];
}

static epd_fb_t *dsp_scr_get_next_fb(void)
{
	uint8_t next_fb = fb_current == 0 ? 1 : 0;
	return fbs[next_fb];
}

static void dsp_scr_fast_update(void)
{
	epd_cog_initial_fast_update();
	epd_cog_fast_update(dsp_scr_get_fb()->image,dsp_scr_get_next_fb()->image);
	epd_cog_power_off();
	dsp_scr_swap_fb();
}

static void dsp_scr_global_update(void)
{
	epd_cog_initial_global_update();
	epd_cog_global_update(dsp_scr_get_fb()->image,dsp_scr_get_next_fb()->image);
	epd_cog_power_off();
	dsp_scr_swap_fb();
}

void dsp_scr_init(epd_drv_t *drv)
{
	// Starts with a blank screen and using global update.
	// Some flickering may occur
	fb_a = epd_fb_init(scr_a,DSP_SCR_WIDTH,DSP_SCR_HEIGHT);
	fb_b = epd_fb_init(scr_b,DSP_SCR_WIDTH,DSP_SCR_HEIGHT);

	epd_fb_rect(dsp_scr_get_fb(),0,0,DSP_SCR_WIDTH-1,DSP_SCR_HEIGHT-1,EPD_DB_COLOR_WHITE,true);
	epd_fb_rect(dsp_scr_get_next_fb(),0,0,DSP_SCR_WIDTH-1,DSP_SCR_HEIGHT-1,EPD_DB_COLOR_WHITE,true);

	epd_init(drv,EPD_SCR_154);

	dsp_scr_global_update();
}

void dsp_scr_run(void)
{
	// Any display operation must follow this sequence (for fast update, no flickering):
	//
	// 1. write into the next frame buffer, use dsp_scr_get_next_fb() to get it
	// 2. call dsp_scr_fast_update() to send the frame buffer to the display without flickering
	//    This function will swap the frame buffers at the end.

	// for instance, line/text rendering:
	epd_fb_line(dsp_scr_get_next_fb(),0,20,DSP_SCR_WIDTH-1,20,EPD_DB_COLOR_BLACK);
	epd_fb_str(dsp_scr_get_next_fb(),DSP_SCR_WIDTH/2,4,EPD_FB_ANCHOR_N,
			   (uint8_t *)"MENU",EPD_FONT_TERMINAL_08X12,EPD_DB_COLOR_BLACK,false);
	dsp_scr_fast_update();

	// you can render images too:
	epd_fb_img_overlay(dsp_scr_get_next_fb(),(uint8_t *)dsp_img_logo,0,0,DSP_SCR_WIDTH,DSP_SCR_HEIGHT);
	dsp_scr_fast_update();
}
