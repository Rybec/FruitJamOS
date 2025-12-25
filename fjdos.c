#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/spi.h"

#include "st7789.h"
#include "sgl.h"
#include "sgl_draw.h"
#include "sgl_img.h"

#include "images.h"


#define FPS10 100000
#define FPS16 62500
#define FPS20 50000
#define FPS25 40000

#define swap_bytes_16(x) ((x << 8) | (x >> 8))

void main() {
	stdio_init_all();
	sleep_ms(100);

	printf("\n\n\n\n\n\n\n\n");

	SGL_init();
	sleep_ms(100);

	SGL_display *display = SGL_create_display(ST7789);
	SGL_surface *screen = SGL_get_display_surface(display);
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

	int16_t y = 0;
	int16_t x = 0;
	uint64_t next_tick = time_us_64();
	int64_t time_diff;


	while(1) {
		next_tick = next_tick + FPS25;


		// Render
		SGL_fill(screen, 0x0000);

		x = (x + 1) % 320;
		y = (y + 1) % 240;

		SGL_DRAW_hline(screen,
		               20, 300,
		               y, 31 << 11 | 0 << 5 | 0);

		SGL_DRAW_vline(screen,
		               x, 20, 220,
		               0 << 11 | 63 << 5 | 0);

		// From top left
		SGL_DRAW_line(screen,
		              0, 0,
		              x, y,
		              0 << 11 | 0 << 5 | 31);

		// From top right
		SGL_DRAW_line(screen,
		              319, 0,
		              x, y,
		              0 << 11 | 0 << 5 | 31);

		// From bottom left
		SGL_DRAW_line(screen,
		              0, 239,
		              x, y,
		              0 << 11 | 0 << 5 | 31);

		// From bottom right
		SGL_DRAW_line(screen,
		              319, 239,
		              x, y,
		              0 << 11 | 0 << 5 | 31);




		SGL_DRAW_circle(screen,
		                x, y,
		                10,
		                0xFFFF);

		SGL_DRAW_rect(screen,
		                x - 12, y - 12,
		                24, 24,
		                31 << 11 | 0 << 5 | 31);

		SGL_DRAW_triangle(screen,
		                  x, y - 40,
		                  x - 30, y + 20,
		                  x + 30, y + 20,
		                  0 << 11 | 63 << 5 | 31);

		SGL_rect srect = {0, 0, blue_roof_house.width, blue_roof_house.height};
		SGL_rect drect = {50, 50, 0, 0};
		SGL_IMG_blit(&blue_roof_house, &srect,
		             screen, &drect);

		ST7789_blit();


		// Busy wait till next tick
		int64_t time_diff = next_tick - time_us_64();
		while (time_diff > 0)
			time_diff = next_tick - time_us_64();
	}

}

