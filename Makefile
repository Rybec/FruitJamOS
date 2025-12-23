


./build/fjdos.uf2: fjdos.c spi1.c st7789.c sgl.c
	cmake -B ./build
	cmake --build ./build
