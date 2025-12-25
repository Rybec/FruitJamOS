#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/spi.h"

#include "st7789.h"
#include "sgl.h"
#include "sgl_draw.h"


#define swap_bytes_16(x) ((x << 8) | (x >> 8))

void main() {
	stdio_init_all();
	sleep_ms(100);

	printf("\n\n\n\n\n\n\n\n");

	SGL_init();
	sleep_ms(100);

	SGL_display *display = SGL_create_display(ST7789);
	sleep_ms(100);

/*
	printf("Setting up data in framebuffer...\n");
	for (uint32_t y = 0; y < 240; y++) {
		for (uint32_t x = 0; x < 320; x++) {
			uint16_t r = (x % 0b11111) | (y % 0b11111);
			uint16_t g = (x + y) % 0b111111 >> 2;
			uint16_t b = (x * y / 4) % 0b11111;

			display->buffer[y * 320 + x] = (r << 11) | (g << 5) | b;
		}
	}
*/

	uint16_t y = 0;
	uint64_t next_tick = time_us_64();
	uint8_t fps = 40;

	while(1) {
		next_tick = next_tick + (1000000 / fps);


		// Render
		SGL_fill(display, 0x0000);

		y = (y + 1) % 240;

		SGL_DRAW_hline(display,
		               20, 300,
		               y, 16 << 0 | 32 << 5 | 8);

		ST7789_blit();


		// Busy wait till next tick
		while (next_tick < time_us_64());
	}

}

