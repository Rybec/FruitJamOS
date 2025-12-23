#include "spi1.h"
#include "st7789.h"

enum SGL_driver {
	ST7789
};


// For multiscreen support, this will need
// a special data field for tracking which
// interface the screen is on (spi1, hstx...)
// as well as things like the CS pin or I2C
// address. 

typedef struct {
	uint16_t width;
	uint16_t height;
	enum SGL_driver driver;
	uint16_t *buffer;
} SGL_display;






void SGL_init();

SGL_display *SGL_create_display(enum SGL_driver driver);

void SGL_destroy_display(SGL_display *display);

void SGL_fill(SGL_display *display, uint16_t color);


