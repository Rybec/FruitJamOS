#ifndef ST7789_H
#define ST7789_H


#define ST7789_DC 6
#define ST7789_RST 7
#define ST7789_CS 10


#define ST7789_ROT0   0b00000000
#define ST7789_ROT90  0b01100000
#define ST7789_ROT180 0b11000000
#define ST7789_ROT270 0b10100000

#define ST7789_ROT0_MIRROR   0b01000000
#define ST7789_ROT90_MIRROR  0b00100000
#define ST7789_ROT180_MIRROR 0b10000000
#define ST7789_ROT270_MIRROR 0b11100000


void ST7789_init(uint8_t rotation, uint8_t _dc, uint8_t _rst, uint8_t _cs);
void ST7789_set_framebuffer(uint16_t *buffer, uint16_t w, uint16_t h);

// This does not update width and height or viewport
void ST7789_set_buff_addr(uint16_t *buffer);

void ST7789_blit();


uint16_t ST7789_get_width();
uint16_t ST7789_get_height();
uint8_t ST7789_get_depth();

// Note that while the size of the framebuffer
// will not change, higher level tracking should
// update the dimensions if needed.
void ST7789_set_rotation(uint8_t rotation);


void ST7789_set_viewport(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void ST7789_reset_viewport();


/*  Does not work with Adafruit displays, as the blacklight
    is controlled with the BL pin and not the controller.
void ST7789_set_brightness(uint8_t brightness);
*/







// Obsolete functions

/*
	Writes buffer contents to full screen

	Buffer size must be no smaller than:
		153,600 bytes

void ST7789_write_frame(uint8_t *buffer);
*/
/*
	Writes buffer contents to specified rectangular area

	Buffer size must be no smaller than:
		(w - x) * (h - y) * 2 bytes

void ST7789_write_rect(uint8_t *buffer, uint16_t x, uint16_t y,
                                        uint16_t w, uint16_t h);
*/
/*
	Slightly faster frame write.  This will write to the same
	region as the previous write call.  Ensure that the buffer
	is large enough for that region.

void ST7789_cont_write(uint8_t *buffer, size_t len);
*/


#endif
