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
#include "epd_cfg.h"
#include "epd_drv.h"

// some registers collect by code inspection, we do not have the datasheet
enum epd_regs_e
{
	EPD_REG_PSR          = 0x00,
	EPD_REG_DCDC_PWR_OFF = 0x02,
	EPD_REG_DCDC_PWR_ON  = 0x04,
	EPD_REG_REFRESH      = 0x12,
	EPD_REG_DTM1         = 0x10,
	EPD_REG_DTM2         = 0x13,
	EPD_REG_PSR_ALT1     = 0x4d,
	EPD_REG_CDI          = 0x50,
	EPD_REG_ACTIVE_TEMP  = 0xe0,
	EPD_REG_TEMP         = 0xe5,
	EPD_REG_PSR_ALT2     = 0xe9,
};

typedef struct epd_drv_scr_size_s
{
	uint16_t size_in;
	uint16_t vertical;
	uint16_t horizontal;
	uint16_t diagonal;
	uint16_t refresh_time_ms;
} epd_drv_scr_size_t;

typedef struct epd_drv_data_s
{
	epd_drv_t drv;
	epd_scr_t scr;
	epd_drv_scr_size_t scr_size;
	uint32_t data_size;
} epd_drv_data_t;

static void epd_send_index_data(uint8_t index, const uint8_t *data, uint32_t len);
static void epd_soft_reset(void);
static void epd_display_refresh(void);
static void epd_reset(uint32_t ms1, uint32_t ms2, uint32_t ms3, uint32_t ms4, uint32_t ms5);
static void epd_dcdc_power_on(void);

static const epd_drv_scr_size_t scr_size_db[] =
{
	[0] = { .size_in = 0,    .vertical = 0,   .horizontal = 0,   .diagonal = 0,   .refresh_time_ms = 0,  },
	[1] = { .size_in = 0x15, .vertical = 152, .horizontal = 152, .diagonal = 154, .refresh_time_ms = 16, },
	[2] = { .size_in = 0x21, .vertical = 212, .horizontal = 104, .diagonal = 213, .refresh_time_ms = 15, },
	[3] = { .size_in = 0x26, .vertical = 296, .horizontal = 152, .diagonal = 266, .refresh_time_ms = 15, },
	[4] = { .size_in = 0x27, .vertical = 264, .horizontal = 176, .diagonal = 271, .refresh_time_ms = 19, },
	[5] = { .size_in = 0x28, .vertical = 296, .horizontal = 128, .diagonal = 287, .refresh_time_ms = 14, },
	[6] = { .size_in = 0x29, .vertical = 384, .horizontal = 168, .diagonal = 290, .refresh_time_ms = 14, },
	[7] = { .size_in = 0x37, .vertical = 416, .horizontal = 240, .diagonal = 370, .refresh_time_ms = 15, },
	[8] = { .size_in = 0x41, .vertical = 300, .horizontal = 300, .diagonal = 417, .refresh_time_ms = 19, },
	[9] = { .size_in = 0x43, .vertical = 480, .horizontal = 176, .diagonal = 437, .refresh_time_ms = 21, },
};

static epd_drv_data_t drv_data = { 0 };

bool epd_init(epd_drv_t *drv, epd_scr_t scr)
{
	if((drv == NULL) || (scr == 0))
		return false;

	drv_data.drv = *drv;
	drv_data.scr = scr;
	drv_data.scr_size = scr_size_db[0];
	uint16_t size_in = EPD_SCR_SIZE_GET(drv_data.scr);

	bool found = false;
	for(size_t pos = 0 ; pos < sizeof(scr_size_db)/sizeof(scr_size_db[0]) ; pos++)
	{
		if(size_in == scr_size_db[pos].size_in)
		{
			drv_data.scr_size = scr_size_db[pos];
			found = true;
			break;
		}
	}

	if(!found)
		return false;

	drv_data.data_size = (drv_data.scr_size.vertical*drv_data.scr_size.horizontal) / 8;

	return true;
}

void epd_cog_initial_global_update(void)
{
	drv_data.drv.pin_set(EPD_DRV_DC_PIN,true);
	drv_data.drv.pin_set(EPD_DRV_RST_PIN,true);
	drv_data.drv.pin_set(EPD_DRV_CS_PIN,true);

	epd_reset(1,5,5,5,1);
	epd_soft_reset();

	uint8_t value = 0x19;
	epd_send_index_data(EPD_REG_TEMP,&value,1);
	value = 0x02;
	epd_send_index_data(EPD_REG_ACTIVE_TEMP,&value,1);

	uint16_t size_in = EPD_SCR_SIZE_GET(drv_data.scr);

	if(size_in == 0x29)
	{
		uint8_t settings[] = {0x55, 0x02};
		epd_send_index_data(EPD_REG_PSR_ALT1,&settings[0],1);
		epd_send_index_data(EPD_REG_PSR_ALT2,&settings[1],1);
	}
	else
	{
		uint8_t settings[] = {0xcf, 0x8d};
		epd_send_index_data(EPD_REG_PSR,&settings[0],2);
	}
}

void epd_cog_initial_fast_update(void)
{
	drv_data.drv.pin_set(EPD_DRV_DC_PIN,true);
	drv_data.drv.pin_set(EPD_DRV_RST_PIN,true);
	drv_data.drv.pin_set(EPD_DRV_CS_PIN,true);

	epd_reset(1,5,5,5,1);
	epd_soft_reset();

	uint8_t value = 0x19 + 0x40;
	epd_send_index_data(EPD_REG_TEMP,&value,1);
	value = 0x02;
	epd_send_index_data(EPD_REG_ACTIVE_TEMP,&value,1);

	uint16_t size_in = EPD_SCR_SIZE_GET(drv_data.scr);

	if(size_in == 0x29)
	{
		uint8_t settings[] = {0x55, 0x02};
		epd_send_index_data(EPD_REG_PSR_ALT1,&settings[0],1);
		epd_send_index_data(EPD_REG_PSR_ALT2,&settings[1],1);
	}
	else
	{
		uint8_t settings[] = {0xcf | 0x10, 0x8d | 0x02};
		epd_send_index_data(EPD_REG_PSR,&settings[0],2);
	}

	value = 0x07;
	epd_send_index_data(EPD_REG_CDI,&value,1);
}

void epd_cog_power_off(void)
{
	uint8_t value = 0;
	epd_send_index_data(EPD_REG_DCDC_PWR_OFF,&value,0);

	while(drv_data.drv.pin_get(EPD_DRV_BUSY_PIN) == false){}

	drv_data.drv.pin_set(EPD_DRV_DC_PIN,false);
	drv_data.drv.pin_set(EPD_DRV_CS_PIN,false);
	// the next line is used in the original driver but
	// it only makes sense if busy pin is an open collector pin
	// as we have two outputs connected in epd schematics 
	// (possible contention if ports are push-pull and
	// datasheet only says "output" so it is dangerous to drive it).
	// drv_data.drv.pin_set(EPD_DRV_BUSY_PIN,false);
	drv_data.drv.delay_ms(150);
	drv_data.drv.pin_set(EPD_DRV_RST_PIN,false);
}

void epd_cog_global_update(const uint8_t *first_img, const uint8_t *second_img)
{
	epd_send_index_data(EPD_REG_DTM1,first_img,drv_data.data_size);
	epd_send_index_data(EPD_REG_DTM2,second_img,drv_data.data_size);

	epd_dcdc_power_on();
	epd_display_refresh();

	drv_data.drv.pin_set(EPD_DRV_CS_PIN,true);
}

void epd_cog_fast_update(const uint8_t *first_img, const uint8_t *second_img)
{
	uint16_t size_in = EPD_SCR_SIZE_GET(drv_data.scr);

	uint8_t value = 0x27;
	if(!(size_in == 0x27 || size_in == 0x28 || size_in == 0x41))
		epd_send_index_data(EPD_REG_CDI,&value,1);

	epd_dcdc_power_on();

	epd_send_index_data(EPD_REG_DTM1,first_img,drv_data.data_size);
	epd_send_index_data(EPD_REG_DTM2,second_img,drv_data.data_size);

	value = 0x07;
	if(!(size_in == 0x27 || size_in == 0x28 || size_in == 0x41))
		epd_send_index_data(EPD_REG_CDI,&value,1);

	epd_display_refresh();
}

static void epd_send_index_data(uint8_t index, const uint8_t *data, uint32_t len)
{
	drv_data.drv.pin_set(EPD_DRV_DC_PIN,false);
	drv_data.drv.pin_set(EPD_DRV_CS_PIN,false);

	drv_data.drv.spi_write(&index,1);

	drv_data.drv.pin_set(EPD_DRV_CS_PIN,true);
	drv_data.drv.pin_set(EPD_DRV_DC_PIN,true);
	drv_data.drv.pin_set(EPD_DRV_CS_PIN,false);

	if(len)
		drv_data.drv.spi_write(data,len);

	drv_data.drv.pin_set(EPD_DRV_CS_PIN,true);
}

static void epd_soft_reset(void)
{
	uint8_t value = 0x0e;
	epd_send_index_data(EPD_REG_PSR,&value,1);

	while(drv_data.drv.pin_get(EPD_DRV_BUSY_PIN) == false){}
}

static void epd_display_refresh(void)
{
	uint8_t value = 0;
	epd_send_index_data(EPD_REG_REFRESH,&value,0);

	while(drv_data.drv.pin_get(EPD_DRV_BUSY_PIN) == false){}
}

static void epd_reset(uint32_t ms1, uint32_t ms2, uint32_t ms3, uint32_t ms4, uint32_t ms5)
{
	drv_data.drv.delay_ms(ms1);
	drv_data.drv.pin_set(EPD_DRV_RST_PIN,true);
	drv_data.drv.delay_ms(ms2);
	drv_data.drv.pin_set(EPD_DRV_RST_PIN,false);
	drv_data.drv.delay_ms(ms3);
	drv_data.drv.pin_set(EPD_DRV_RST_PIN,true);
	drv_data.drv.delay_ms(ms4);
	drv_data.drv.pin_set(EPD_DRV_CS_PIN,true);
	drv_data.drv.delay_ms(ms5);
}

static void epd_dcdc_power_on(void)
{
	uint8_t value = 0;
	epd_send_index_data(EPD_REG_DCDC_PWR_ON,&value,0);

	while(drv_data.drv.pin_get(EPD_DRV_BUSY_PIN) == false){}
}

