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
	uint16_t *pixels;
	uint16_t *sending_buffer;
} SGL_surface;

// SGL_surface flags
#define SGL_DOUBLE_BUFFER 0b00000001



// For multiscreen support, this will need
// a special data field for tracking which
// interface the screen is on (spi1, hstx...)
// as well as things like the CS pin or I2C
// address.

typedef struct {
	enum SGL_driver driver;
	uint8_t flags;  // bit 0 - double buffer
	SGL_surface *surface;
} SGL_display;




typedef struct {
	int16_t x, y;
	int16_t w, h;
} SGL_rect;


void SGL_init();

SGL_display *SGL_create_display(enum SGL_driver driver, uint8_t flags);

SGL_surface *SGL_get_display_surface(SGL_display *display);

void SGL_destroy_display(SGL_display *display);

void SGL_set_rotation(SGL_display *display, uint16_t rotation);

void SGL_flip(SGL_display *display);

void SGL_fill(SGL_surface *surface, uint16_t color);

#endif
