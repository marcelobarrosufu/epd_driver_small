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
#include "epd_drv.h"
#include "dsp_scr.h"

static void app_drv_pin_set(epd_drv_pins_t pin, bool state)
{
	// TODO: write a function to set the pin state based on pin enumeration
	switch(pin)
	{
		case EPD_DRV_BUSY_PIN:
		// set the pin state. For example using stm32 HAL:
		//HAL_GPIO_WritePin(port,pin,state == true ? GPIO_PIN_SET : GPIO_PIN_RESET);
			break;
		case EPD_DRV_DC_PIN:
		// set the pin state
			break;
		case EPD_DRV_RST_PIN:
		// set the pin state
			break;
		case EPD_DRV_CS_PIN:
		// set the pin state
			break;
		default:
			break;
	}	
}

static bool app_drv_pin_get(epd_drv_pins_t pin)
{
	bool state = true;
	// TODO: write a function to get the pin state based on pin enumeration and return it
	switch(pin)
	{
		case EPD_DRV_BUSY_PIN:
		// get the pin state. For example using stm32 HAL:
		//  state = HAL_GPIO_ReadPin(port,pin) == GPIO_PIN_SET;
			break;
		case EPD_DRV_DC_PIN:
		// get the pin state
			break;
		case EPD_DRV_RST_PIN:
		// get the pin state
			break;
		case EPD_DRV_CS_PIN:
		// get the pin state
			break;
		default:
			break;
	}

	return state;
}

static void app_drv_spi_write(const uint8_t *data, uint32_t size)
{
	// TODO: write a function to write data to the SPI bus (cs will be asserted by the driver)
	// For example using stm32 HAL:
	// HAL_SPI_Transmit(&hspi1, (uint8_t *)data, size, 1000);
}

static void app_drv_delay_ms(uint32_t t_ms)
{
	// TODO: write a function to delay t_ms milliseconds
	// For example using stm32 HAL:
	// HAL_Delay(t_ms);
}

// your driver functions
static epd_drv_t drv =
{
	.pin_set = app_drv_pin_set,
	.pin_get = app_drv_pin_get,
	.spi_write = app_drv_spi_write,
	.delay_ms = app_drv_delay_ms,
};

int main(void)
{
	// initialize the display and your portable driver
	dsp_scr_init(&drv);

	// run demo
	dsp_scr_run();

	return 0;
}
