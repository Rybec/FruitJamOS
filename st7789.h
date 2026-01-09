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


// Set viewport to the full dimensions of the screen.
void ST7789_reset_viewport();
void ST7789_set_viewport(uint16_t x, uint16_t y, uint16_t w, uint16_t h);


void ST7789_blit(uint16_t *buffer);


// Note that while the size of the framebuffer
// will not change, the viewport and higher level
// tracking should update the dimensions.
void ST7789_set_rotation(uint8_t rotation);


uint16_t ST7789_get_width();
uint16_t ST7789_get_height();
uint8_t ST7789_get_depth();



/*  Does not work with Adafruit displays, as the blacklight
    is controlled with the BL pin and not the controller.
void ST7789_set_brightness(uint8_t brightness);
*/



#endif
