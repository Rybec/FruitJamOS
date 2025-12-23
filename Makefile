


./build/fjdos.uf2: fjdos.c spi1.c st7789.c sgl.c sgl_draw.c
	cmake -B ./build
	cmake --build ./build
