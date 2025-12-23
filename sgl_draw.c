
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

void SGL_DRAW_hline(SGL_display *display,
                    uint8_t x0, uint8_t x1,
                    uint8_t y, uint16_t color) {
	uint8_t _x0;
	uint8_t _x1;

	if (x0 <= x1) {
		_x0 = x0;
		_x1 = x1;
	} else {
		_x0 = x1;
		_x1 = x0;
	}
	
	for (uint8_t x = _x0; x < x1; x++)
		display->buffer[y * 320 + x] = color;
}



