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
	// TODO: write a function to set the pin state
}

static bool app_drv_pin_get(epd_drv_pins_t pin)
{
	// TODO: write a function to get the pin state and return it
	return true;
}

static void app_drv_spi_write(const uint8_t *data, uint32_t size)
{
	// TODO: write a function to write data to the SPI bus
}

static void app_drv_delay_ms(uint32_t t_ms)
{
	// TODO: write a function to delay t_ms milliseconds
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
