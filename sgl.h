#ifndef SGL_H
#define SGL_H

#include "pico/stdlib.h"

#include "spi1.h"
#include "st7789.h"

enum SGL_driver {
	ST7789
};

typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t pixels[];
} SGL_surface;

// For multiscreen support, this will need
// a special data field for tracking which
// interface the screen is on (spi1, hstx...)
// as well as things like the CS pin or I2C
// address.

typedef struct {
	enum SGL_driver driver;
	SGL_surface *surface;
} SGL_display;




typedef struct {
	int16_t x, y;
	int16_t w, h;
} SGL_rect;


void SGL_init();

SGL_display *SGL_create_display(enum SGL_driver driver);

SGL_surface *SGL_get_display_surface(SGL_display *display);

void SGL_destroy_display(SGL_display *display);


void SGL_fill(SGL_surface *surface, uint16_t color);

#endif
