
void SGL_DRAW_hline(SGL_display *display,
                    int16_t x0, int16_t x1,
                    int16_t y, uint16_t color);

void SGL_DRAW_vline(SGL_display *display,
                    int16_t x, int16_t y0,
                    int16_t y1, uint16_t color);

void SGL_DRAW_line(SGL_display *display,
                   int16_t x0, int16_t y0,
                   int16_t x1, int16_t y1,
                   uint16_t color);

void SGL_DRAW_rect(SGL_display *display,
                   int16_t x, int16_t y,
                   int16_t w, int16_t h,
                   uint16_t color);

void SGL_DRAW_triangle(SGL_display *display,
                       int16_t x0, int16_t y0,
                       int16_t x1, int16_t y1,
                       int16_t x2, int16_t y2,
                       uint16_t color); 

void SGL_DRAW_circle(SGL_display *display,
                     int16_t x0, int16_t y0,
                     uint16_t radius, uint16_t color);
