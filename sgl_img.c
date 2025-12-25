#include <stdio.h>  // For debugging, remove

#include "pico.h"
#include "pico/stdlib.h"


#include "sgl.h"
#include "sgl_img.h"


/*
typedef struct {
	uint16_t width,
	uint16_t height,
	uint16_t data[]
} SGL_surface;


typedef struct {
	int16_t x, y;
	int16_t w, h;
} SDL_Rect;
*/


void SGL_IMG_blit(SGL_surface *src, const SGL_rect *srcrect,
                  SGL_surface *dst, SGL_rect *dstrect) {
	// If any of these are negative, the associated rect
	// is invalid.
	if ((srcrect->w | srcrect->h) < 0)
		return;

	uint16_t src_w = src->width;
	uint16_t src_h = src->height;
	uint16_t dst_w = dst->width;
	uint16_t dst_h = dst->height;

	uint16_t src_x0 = MIN(MAX(0, srcrect->x), src_w);
	uint16_t src_y0 = MIN(MAX(0, srcrect->y), src_h);
	uint16_t src_x1 = MIN(MAX(0, srcrect->x + srcrect->w), src_w);
	uint16_t src_y1 = MIN(MAX(0, srcrect->y + srcrect->h), src_h);

	uint16_t dst_x0 = MIN(MAX(0, dstrect->x), dst_w);
	uint16_t dst_y0 = MIN(MAX(0, dstrect->y), dst_h);

	// Constrain width to fit destination window
	src_x1 = MIN(src_x1, dst_w - dst_x0 + src_x0);
	src_y1 = MIN(src_y1, dst_h - dst_y0 + src_y0);

	/*
	dst_w - dst_x0 = width to the right of the dest point
	src_x1 - src_x0 = width of the image part to be blitted
	(dst_w - dst_x0) => (src_x1 - src_x0) must be true to
		stay within the bounds of the dst surface.
	If the source rect is too wide, we will constrain its
		width such that (dst_w - dst_x0) == (src_x1 - src_x0)

	Solve for src_x1:

	dst_w - dst_x0 = src_x1 - src_x0
	dst_w - dst_x0 + src_x0 = src_x1
	scr_x1 = dst_w - dst_x0 + src_x0

	If src_x1 calculated this way is smaller than the current
	src_x1, we replace the current one, otherwise the source
	aready fits and does not need to be reduced in width.

	The same reduction form applies to src_y1.
	*/

	for (uint16_t sy = src_y0; sy < src_y1; sy++) {
		uint16_t dy = dst_y0 + sy - src_y0;
		for (uint16_t sx = src_x0; sx < src_x1; sx++) {
			uint16_t dx = dst_x0 + sx - src_x0;
			if (src->pixels[sx + sy * src_w] != 0b0000000000100000)
				dst->pixels[dx + dy * dst_w] = src->pixels[sx + sy * src_w];
		}
	}
}

