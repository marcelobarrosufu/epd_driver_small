CC = gcc
TARGET = app_demo

CFLAGS = -g -Wall -std=c11 -Idsp -Iepd 
LDFLAGS = 

SRCS =  app.c dsp/dsp_scr.c epd/epd_drv.c epd/epd_fb.c epd/epd_font.c 

OBJS = $(SRCS:.c=.o)
MAKEFLAGS += --jobs=8

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -fR $(OBJS) $(TARGET)
