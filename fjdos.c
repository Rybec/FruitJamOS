#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "spi1.h"
#include "st7789.h"

#define ST7789_DC 6
#define ST7789_RST 7
#define ST7789_CS 10

#define swap_bytes_16(x) ((x << 8) | (x >> 8))

void main() {
	stdio_init_all();
	sleep_ms(5000);

	printf("\n\n\n\n\n\n\n\n");
	printf("Creating framebuffer...\n");
	uint16_t framebuffer[320 * 240];

	printf("Initializing SPI1...\n");
	SPI1_init();

	printf("Initializing ST7789...\n");
	ST7789_init(ROT90, ST7789_DC, ST7789_RST, ST7789_CS);

	ST7789_set_framebuffer(framebuffer, 320, 240);

	printf("Setting up data in framebuffer...\n");
	for (uint32_t y = 0; y < 240; y++) {
		for (uint32_t x = 0; x < 320; x++) {
			uint16_t r = (x % 0b11111) | (y % 0b11111);
			uint16_t g = (x + y) % 0b111111 >> 2;
			uint16_t b = (x * y / 4) % 0b11111;

			framebuffer[y * 320 + x] = (r << 11) | (g << 5) | b;
		}
	}


	while(1) {
		printf("Writing frame...\n");
//		ST7789_write_frame((uint8_t*)framebuffer);
		ST7789_blit();
		printf("Waiting 1 second...\n");
		sleep_ms(1000);
	}

}

