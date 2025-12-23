/* Small Graphics Library */

/* Designed for real-time rendering
on microcontrollers.  Uses patterns
from SDL, adapted to low resource,
baremetal platforms. */

#include <stdlib.h>

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
	uint16_t width;
	uint16_t height;
	enum SGL_Driver driver;
	uint16_t *buffer;
} SGL_display;
*/

SGL_display *SGL_create_display(enum SGL_driver driver) {
	switch (driver) {
		case ST7789:
			uint16_t *buffer = calloc(320 * 240, 2);

			ST7789_init(ROT90, ST7789_DC, ST7789_RST, ST7789_CS);
			ST7789_set_framebuffer(buffer, 320, 240);

			SGL_display *display = malloc(sizeof(SGL_display));

			display->width = 320;
			display->height = 240;
			display->driver = driver;
			display->buffer = buffer;

			ST7789_blit();

			return display;
		default:
			return NULL;
	}
}


void SGL_destroy_display(SGL_display *display) {
	free(display->buffer);
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
