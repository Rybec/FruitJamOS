/* Small Graphics Library */

/* Designed for real-time rendering
on microcontrollers.  Uses patterns
from SDL, adapted to low resource,
baremetal platforms. */

#include <stdlib.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/malloc.h"

#include "spi1.h"
#include "st7789.h"
#include "sgl.h"


void SGL_init() {
	SPI1_init();
}

/*
typedef struct {
	enum SGL_driver driver;
	SGL_surface *surface;
} SGL_display;

typedef struct {
	uint16_t width,
	uint16_t height,
	uint16_t pixels[]
} SGL_surface;
*/

SGL_display *SGL_create_display(enum SGL_driver driver) {
	switch (driver) {
		case ST7789:
			SGL_display *display = malloc(sizeof(SGL_display));
			SGL_surface *surface = malloc(sizeof(SGL_surface) + 320 * 240 * 2);

			ST7789_init(ROT90, ST7789_DC, ST7789_RST, ST7789_CS);
			sleep_ms(100);
			ST7789_set_framebuffer(surface->pixels, 320, 240);

			display->driver = driver;
			display->surface = surface;
			display->surface->width = 320;
			display->surface->height = 240;
			memset(display->surface->pixels, 0x00, 320 * 240 * 2);

			ST7789_blit();

			return display;
		default:
			return NULL;
	}
}

SGL_surface *SGL_get_display_surface(SGL_display *display) {
	return display->surface;
}

void SGL_destroy_display(SGL_display *display) {
	free(display->surface);
	free(display);
}

void SGL_fill(SGL_surface *surface, uint16_t color) {
	for (uint32_t i = 0; i < (surface->width * surface->height); i++) {
		surface->pixels[i] = color;
	}
}




/*
	Options?:

		Draw surface directly to display (unbuffered)
			Do we need this, or should user call driver
			functions directly for this?  Surface object
			should contain dimension data, color depth,
			and a buffer.  User could use dim and buffer
			to do unbuffered draw through driver.  This
			does not use DMA and is thus slow.

		Driver needs to contain framebuffer, for DMA.
			Driver function for initializing framebuffer.
			Sets up DMA.  Framebuffer treated as normal
			surface, and "flip()" function activates DMA
			write?  Maybe this module wraps framebuffer
			in Surface object.  This means Surface struct
			has width, height, and buffer pointer.



	Get graphics surface

	Data needed from driver:
		screen size
		color depth

	What this should not provide
	access to:
		Special functions from driver
			Backlight brightness
			rotation change
			(Maybe callback for rotation change, to
			update screen size.)


*/
