# Hardware driver for small Pervasive Displays' e-paper panels

This library is a C implementation of [drivers written in C++ (Arduino)](https://github.com/PervasiveDisplays/EPD_Driver_GU_small) from the company Pervasive Displays. While I understand that providing drivers for Arduino platform facilitates initial testing, I also consider that is extremely important to have options for pure C development. After all, many professional applications do not run using the Arduino platform.

Pervasive Displays did not provide the datasheet for its display controller and the driver was rewritten through analysis of the original C++ code. So you can see some magic numbers in the code when configuring display registers. Furthermore, graphic primitives such as point, line and rectangle were ported from the [graphics library provided by Pervasive](https://github.com/PervasiveDisplays/PDLS_EXT3_Basic_Global), allowing very simple screens to be built without any external dependencies. Again, the original code was written in C++.

The entire portability mechanism was changed, aiming to make  easier to adapt the code to platforms other than Arduino. I recommend reading the documentation about the displays. Although they do not deal with register level information, they provide details on the general functioning of the driver in relation to updates.

There is an small script called (gen_image.py) for converting image files into c code (header files). Take a look at it and modify it according to your interest.

This driver only includes small BW displays but can be easily modified to work with BWR or even BWRY displays.

# Files

Short explanation about library files:

* `epd_drv.c/.h`: C implementation of the display driver
* `epd_fb.c/.h`: Functions for writing primitives to a frame buffer (line, points, rectangles and images)
* `epd_font.c/.h`: Fonts handling (similar to other fonts files found on Internet)
* `epd_cfg.h`: Enumeration of supported displays, following original Pervasive Display strategy.
* `epd_cfg.h`: Enumeration of supported colors.
* `dsp_scr.c/.h`: Example of how to create an abstraction for your display (how to initialize the dual frame buffer, execute global ou fast updates, etc)
* `main.c`: Initialize all the library and shows how to create your portability layer.
* `gen_image.py`: Script for converting image files into c code (in fact, header files).

# Links

* [Pervasive Display wiki](https://docs.pervasivedisplays.com/)
* [Pervasive Display github](https://github.com/PervasiveDisplays
)

# Authoring and Licensing

MIT License

Copyright (c) 2023- [Marcelo Barros de Almeida](marcelobarrosalmeida@gmail.com)