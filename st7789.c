#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"

#include "spi1.h"


#define swap_bytes_16(x) ((x << 8) | (x >> 8))


uint8_t dc;
uint8_t rst;
uint8_t cs;

uint8_t depth;

uint16_t width;
uint16_t height;

uint16_t *framebuffer = NULL;
uint16_t fb_width = 0;
uint16_t fb_height = 0;

uint16_t viewport_x = 0;
uint16_t viewport_y = 0;
uint16_t viewport_w = 0;
uint16_t viewport_h = 0;

uint8_t dirty_ramwr = 1;  // Track when DMA needs to resend frame commands


// Must handle manually
// D6 - GPIO6 (SCK0) - DC Pin
// D7 - GPIO7 (TX0) - RST Pin
// D10 - GPIO10 (SCK1) - CS Pin


void write_command(uint8_t cmd) {
	gpio_put(cs, 0);
	gpio_put(dc, 0);
	SPI1_write(&cmd, 1);
	gpio_put(cs, 1);
}

void write_data(uint8_t *data, size_t len) {
	gpio_put(cs, 0);
	gpio_put(dc, 1);
	SPI1_write(data, len);
	gpio_put(cs, 1);
}

void write_register(uint8_t reg, uint32_t argc, uint8_t argv[]) {
	gpio_put(cs, 0);
	gpio_put(dc, 0);
	SPI1_write(&reg, 1);

	gpio_put(dc, 1);
	SPI1_write(argv, argc);

	gpio_put(cs, 1);
}




void ST7789_init(uint8_t rotation, uint8_t _dc, uint8_t _rst, uint8_t _cs) {
	dc = _dc;
	rst = _rst;
	cs = _cs;

	if ((rotation & 0b00100000) == 0) {
		width = 240;
		height = 320;
	} else {
		width = 320;
		height = 240;
	}

	gpio_init(dc);
	gpio_set_dir(dc, GPIO_OUT);
	gpio_put(dc, 0);

	gpio_init(rst);
	gpio_set_dir(rst, GPIO_OUT);
	gpio_put(rst, 1);
	sleep_ms(100);
	gpio_put(rst, 0);
	sleep_ms(100);
	gpio_put(rst, 1);
	sleep_ms(100);

	gpio_init(cs);
	gpio_set_dir(cs, GPIO_OUT);
	gpio_put(cs, 1);


// \x01 - SWRESET (no args, 5ms delay or if SLPIN, 120ms before SLPOUT)
	write_command(0x01);
	sleep_ms(120);

// \x11 - SLPOUT (5ms delay, or 120ms delay before SLPIN)
	write_command(0x11);
	sleep_ms(5);

// \x3a - COLMOD (8bit arg, no delay required)
// \x55 - 0101:0101, 16 bit color
	write_register(0x3a, 1, "\x55");
	depth = 16;

// \x36 - MADCTL (8 bit arg, no delay required)
	write_register(0x36, 1, &rotation);

// \xB0 = RAMCTRL (16 bit arg, no delay required)
// Flip endianness to avoid having to manually flip
// during every pixel color write to framebuffer.
	write_register(0xB0, 2, "\x00\x08");

// \x13 - NORON (no args, no delay required)
	write_command(0x13);

// \x21 - INVON (no args, no delay required)
	write_command(0x21);

// \x29 - DISPON (no args, no delay required)
	write_command(0x29);
}


void ST7789_set_viewport(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	viewport_x = x;
	viewport_y = y;
	viewport_w = w;
	viewport_h = h;

	dirty_ramwr = 1;
}


void ST7789_reset_viewport() {
	viewport_x = 0;
	viewport_y = 0;
	viewport_w = fb_width;
	viewport_h = fb_height;

	dirty_ramwr = 1;
}


// Note that this resets viewport to new framebuffer
void ST7789_set_framebuffer(uint16_t *buffer, uint16_t w, uint16_t h) {
	SPI1_DMA_wait();
	framebuffer = buffer;
	fb_width = w;
	fb_height = h;

	ST7789_reset_viewport();

	SPI1_DMA_set_buf((uint8_t*)buffer, w * h * 2);

	dirty_ramwr = 1;
}


void ST7789_blit() {
	SPI1_DMA_wait();

	if (dirty_ramwr) {
		uint16_t r[2] = {swap_bytes_16(viewport_x),
		                 swap_bytes_16(viewport_w - 1)};
		write_register(0x2A, 4, (uint8_t*)r);

		r[0] = swap_bytes_16(viewport_y);
		r[1] = swap_bytes_16(viewport_h - 1);
		write_register(0x2B, 4, (uint8_t*)r);

		dirty_ramwr = 0;
	}

	// RAMWR
	write_command(0x2C);

	gpio_put(cs, 0);
	gpio_put(dc, 1);
	SPI1_DMA_start_tx();
}


inline uint16_t ST7789_get_width() {
	return width;
}


inline uint16_t ST7789_get_height() {
	return height;
}


// Returns color depth in bits
inline uint8_t ST7789_get_depth() {
	return depth;
}


void ST7789_set_rotation(uint8_t rotation) {
	SPI1_DMA_wait();

	if ((rotation & 0b00100000) == 0) {
		width = 240;
		height = 320;
	} else {
		width = 320;
		height = 240;
	}

// \x36 - MADCTL (8 bit arg, no delay required)
	write_register(0x36, 1, &rotation);
}





// Maybe add function for writing to subsection of
// screen or maybe add full "viewport" support.






/*  Does not work with Adafruit displays, as the blacklight
    is controlled with the BL pin and not the controller.
// Change brightness using display controller
void ST7789_set_brightness(uint8_t brightness) {
	SPI1_DMA_wait();
	write_register(0x51, 1, &brightness);
}
*/


// !!! Non-DMA functions, preserved for informational purposes only. !!!

/*
	Writes buffer contents to full screen

	Buffer size must be no smaller than:
		153,600 bytes

void ST7789_write_frame(uint8_t *buffer) {
	uint16_t r[2] = {0, swap_bytes_16(width - 1)};
	write_register(0x2A, 4, (uint8_t*)r);

	r[1] = swap_bytes_16(height - 1);
	write_register(0x2B, 4, (uint8_t*)r);

	write_register(0x2C, width * height * 2, buffer);
}
*/

/*
	Writes buffer contents to specified rectangular area

	Buffer size must be no smaller than:
		(w - x) * (h - y) * 2 bytes

void ST7789_write_rect(uint8_t *buffer, uint16_t x, uint16_t y,
                                        uint16_t w, uint16_t h) {
	uint16_t r[2] = {x, swap_bytes_16(w + x - 1)};
	write_register(0x2A, 4, (uint8_t*)r);

	r[0] = y;
	r[1] = swap_bytes_16(h + y - 1);
	write_register(0x2B, 4, (uint8_t*)r);

	write_register(0x2C, w * h * 2, buffer);
}
*/

/*
	Slightly faster frame write.  This will write to the same
	region as the previous write call.  Ensure that the buffer
	is large enough for that region.

void ST7789_cont_write(uint8_t *buffer, size_t len) {
	write_data(buffer, len);
}
*/


