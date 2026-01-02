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
	uint8_t flags,
	uint16_t *pixels;
	uint16_t *sending_buffer;
} SGL_surface;
*/

SGL_display *SGL_create_display(enum SGL_driver driver, uint8_t flags) {
	switch (driver) {
		case ST7789:
			SGL_display *display = malloc(sizeof(SGL_display));
			SGL_surface *surface;
			if (flags & SGL_DOUBLE_BUFFER) {
				surface = malloc(sizeof(SGL_surface));
				surface->pixels = malloc(320 * 240 * 4);
				surface->sending_buffer = surface->pixels + 320 * 240 * 2;
				memset(surface->pixels, 0x00, 320 * 240 * 4);
			} else {
				surface = malloc(sizeof(SGL_surface) + 320 * 240 * 2);
				surface->pixels = malloc(320 * 240 * 2);
				surface->sending_buffer = NULL;
				memset(surface->pixels, 0x00, 320 * 240 * 2);
			}

			ST7789_init(ST7789_ROT90, ST7789_DC, ST7789_RST, ST7789_CS);
			sleep_ms(100);
			ST7789_set_framebuffer(surface->pixels, 320, 240);

			display->driver = driver;
			display->flags = flags;
			display->surface = surface;
			display->surface->width = 320;
			display->surface->height = 240;

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
	if (display->flags & SGL_DOUBLE_BUFFER) {
		if (display->surface->pixels < display->surface->sending_buffer) {
			free(display->surface->pixels);
		} else {
			free(display->surface->sending_buffer);
		}
	} else {
		free(display->surface->pixels);
	}

	free(display->surface);
	free(display);
}

void SGL_flip(SGL_display *display) {
	switch (display->driver) {
		case ST7789:
			if (display->flags & SGL_DOUBLE_BUFFER) {
				uint16_t *temp;
				temp = display->surface->pixels;
				display->surface->pixels = display->surface->sending_buffer;
				display->surface->sending_buffer = temp;

				ST7789_set_buff_addr(display->surface->sending_buffer);
			}

			ST7789_blit();
		default:
			return;
	}

}

void SGL_fill(SGL_surface *surface, uint16_t color) {
	for (uint32_t i = 0; i < (surface->width * surface->height); i++) {
		surface->pixels[i] = color;
	}
}

void SGL_set_rotation(SGL_display *display, uint16_t rotation) {
	switch (display->driver) {
		case ST7789:
			uint8_t rot;

			switch (rotation) {
				case 0:
					rot = ST7789_ROT0;
					display->surface->width = 240;
					display->surface->height = 320;
					break;
				case 90:
					rot = ST7789_ROT90;
					display->surface->width = 320;
					display->surface->height = 240;
					break;
				case 180:
					rot = ST7789_ROT180;
					display->surface->width = 240;
					display->surface->height = 320;
					break;
				case 270:
					rot = ST7789_ROT270;
					display->surface->width = 320;
					display->surface->height = 240;
					break;
				default:
					return;
			}

			ST7789_set_rotation(rot);
			break;
		default:
			return;
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
