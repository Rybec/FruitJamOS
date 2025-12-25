#include <stdlib.h>

#include "pico/stdlib.h"

#include "sgl.h"


/*
typedef struct {
	uint16_t width;
	uint16_t height;
	enum SGL_driver driver;
	uint16_t *buffer;
} SGL_display;
*/

void SGL_DRAW_pixel(SGL_display *display,
                    int16_t x, int16_t y,
                    int16_t color) {
	if ((x >= 0) && (y >= 0) &&
        (x < display->width) && (y < display->height))
		display->buffer[y * display->width + x] = color;
}

void SGL_DRAW_hline(SGL_display *display,
                    int16_t x0, int16_t x1,
                    int16_t y,
                    uint16_t color) {
	int16_t _x0;
	int16_t _x1;

	if (x0 <= x1) {
		_x0 = x0;
		_x1 = x1;
	} else {
		_x0 = x1;
		_x1 = x0;
	}

	for (int16_t x = _x0; x < _x1; x++)
		SGL_DRAW_pixel(display, x, y, color);
}

void SGL_DRAW_vline(SGL_display *display,
                    int16_t x,
                    int16_t y0, int16_t y1,
                    uint16_t color) {
	int16_t _y0;
	int16_t _y1;

	if (y0 <= y1) {
		_y0 = y0;
		_y1 = y1;
	} else {
		_y0 = y1;
		_y1 = y0;
	}

	for (int16_t y = _y0; y < _y1; y++)
		SGL_DRAW_pixel(display, x, y, color);
}


void SGL_DRAW_line(SGL_display *display,
                   int16_t x0, int16_t y0,
                   int16_t x1, int16_t y1,
                   uint16_t color) {
	if (x0 == x1) {
		SGL_DRAW_vline(display, x0,
		               y0, y1, color);
	} else if (y0 == y1) {
		SGL_DRAW_hline(display,
		               x0, x1,
		               y0, color);
	} else {
		int16_t dx = abs(x1 - x0);
		int16_t sx = x0 < x1 ? 1 : -1;
		int16_t dy = abs(y1 - y0);
		int16_t sy = y0 < y1 ? 1 : -1;
		int16_t err = (dx > dy ? dx : -dy) / 2;
		int16_t e2;

		while (true) {
			SGL_DRAW_pixel(display, x0, y0, color);

			if (x0 == x1 && y0 == y1)
				break;

			e2 = err;

			if (e2 > - dx) {
				err -= dy;
				x0 += sx;
			}

			if (e2 < dy) {
				err += dx;
				y0 += sy;
			}
		}
	}
}


void SGL_DRAW_rect(SGL_display *display,
                   int16_t x, int16_t y,
                   int16_t w, int16_t h,
                   uint16_t color) {
	SGL_DRAW_hline(display, x, x + w, y, color);
	SGL_DRAW_hline(display, x, x + w, y + h, color);

	SGL_DRAW_vline(display, x, y, y + h, color);
	SGL_DRAW_vline(display, x + w, y, y + h, color);
}

void SGL_DRAW_triangle(SGL_display *display,
                       int16_t x0, int16_t y0,
                       int16_t x1, int16_t y1,
                       int16_t x2, int16_t y2,
                       uint16_t color) {
	SGL_DRAW_line(display,
	              x0, y0, x1, y1,
	              color);
	SGL_DRAW_line(display,
	              x1, y1, x2, y2,
	              color);
	SGL_DRAW_line(display,
	              x0, y0, x2, y2,
	              color);
}

void SGL_DRAW_circle(SGL_display *display,
                     int16_t x0, int16_t y0,
                     uint16_t radius, uint16_t color) {

	int16_t f = 1 - radius;
	int16_t dx = 0;
	int16_t dy = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

	SGL_DRAW_pixel(display, x0, y0 + radius, color);
	SGL_DRAW_pixel(display, x0, y0 - radius, color);
	SGL_DRAW_pixel(display, x0 + radius, y0, color);
	SGL_DRAW_pixel(display, x0 - radius, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			dy += 2;
			f += dy;
		}

		x++;
		dx += 2;
		f += dx + 1;

		SGL_DRAW_pixel(display, x0 + x, y0 + y, color);
		SGL_DRAW_pixel(display, x0 - x, y0 + y, color);
		SGL_DRAW_pixel(display, x0 + x, y0 - y, color);
		SGL_DRAW_pixel(display, x0 - x, y0 - y, color);
		SGL_DRAW_pixel(display, x0 + y, y0 + x, color);
		SGL_DRAW_pixel(display, x0 - y, y0 + x, color);
		SGL_DRAW_pixel(display, x0 + y, y0 - x, color);
		SGL_DRAW_pixel(display, x0 - y, y0 - x, color);
	}
}



