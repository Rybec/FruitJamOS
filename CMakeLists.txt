cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
set(PICO_BOARD adafruit_fruit_jam)
set(PICO_PLATFORM rp2350-arm-s)
include(pico_sdk_import.cmake)

project(fjdos C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project

add_executable(fjdos
    fjdos.c
	spi1.c
	st7789.c
	sgl.c
	sgl_draw.c

)


target_include_directories(fjdos PRIVATE
    ${CMAKE_CURRENT_LIST_DIR} # For btstack config
    )

target_link_libraries(fjdos
        pico_stdlib              # for core functionality
		pico_malloc
		hardware_spi
		hardware_dma
        )

 

# create map/bin/hex/uf2 file in addition to ELF.

pico_add_extra_outputs(fjdos)
pico_enable_stdio_usb(fjdos 1)


