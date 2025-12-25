


./build/fjdos.uf2: fjdos.c spi1.c st7789.c sgl.c sgl_draw.c sgl_img.c images.c
	cmake -B ./build
	cmake --build ./build


clean:
	$(MAKE) -C build/ clean
