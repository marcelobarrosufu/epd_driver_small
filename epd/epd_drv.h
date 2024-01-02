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

// BEFORE CALLING ANY DRIVER FUNCTION:
// 1) Configure IO pins as above (see input/output annotation)
// 2) Do the SPI setup as MSB first, SPI mode 0, 8MHz, 8 bits
// 3) Create your functions for epd_drv_t and initialize the driver

#define EPD_SCR_SIZE_GET(x) ((uint16_t) ((x) >> 8))

typedef enum epd_drv_pins_e
{
	/* INP */ EPD_DRV_BUSY_PIN = 0,
	/* OUT */ EPD_DRV_DC_PIN,
	/* OUT */ EPD_DRV_RST_PIN,
	/* OUT */ EPD_DRV_CS_PIN,
} epd_drv_pins_t;

typedef struct epd_drv_s
{
	void (*pin_set)(epd_drv_pins_t pin, bool state);
	bool (*pin_get)(epd_drv_pins_t pin);
	void (*spi_write)(const uint8_t *data, uint32_t size);
	void (*delay_ms)(uint32_t t_ms);
} epd_drv_t;

bool epd_init(epd_drv_t *drv, epd_scr_t scr);
void epd_cog_initial_global_update(void);
void epd_cog_initial_fast_update(void);
void epd_cog_power_off(void);
void epd_cog_global_update(const uint8_t *first_img, const uint8_t *second_img);
void epd_cog_fast_update(const uint8_t *first_img, const uint8_t *second_img);


