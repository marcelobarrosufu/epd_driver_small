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

typedef enum epd_scr_e
{
	// Monochrome screens and default color screens
	EPD_SCR_154 = 0x150C, // xE2154CSxxx
	EPD_SCR_213 = 0x2100, // xE2213CSxxx
	EPD_SCR_266 = 0x2600, // xE2266CSxxx
	EPD_SCR_271 = 0x2700, // xE2271CSxxx
	EPD_SCR_287 = 0x2800, // xE2287CSxxx
	EPD_SCR_292 = 0x2900, // xE2290CSxxx
	EPD_SCR_370 = 0x3700, // xE2370CSxxx
	EPD_SCR_417 = 0x4100, // xE2417CSxxx
	EPD_SCR_437 = 0x430C, // xE2437CSxxx
} epd_scr_t;



