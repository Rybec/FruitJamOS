## Description

This is a collection of Fruit Jam drivers and libraries currently under construction, slated for future addition to a full OS for the Fruit Jam and/or CircuitPythonmodules for the Fruit Jam.

## Adafruit Fruit Jam OS

If you are looking for the official Adafruit Fruit Jam OS, you've managed to find the wrong thing.  You'll want to go to Adafruit's repository which can be found [here](https://github.com/adafruit/Fruit-Jam-OS).

## Goals

I started this project in response to personal complaints with the CircuitPython `displayio` module.  It's slow, and it's too high level, both of which make it poorly suited to real-time graphics applications.  `displayio` is a great choice for applications like status displays and still image displays, and while it does have some features designed to mitigate the slowness, for serious real-time applications it just isn't sufficient, especially for the Fruit Jam.

This project has several key goals.  First, I want to create a strong lower level graphics library that is fast and unencumbered by expensive UI layering.  I am modeling the design of this library as closely to SDL as is reasonable for the target platform, because SDL's design is very good, and because it may be familiar to those with C or C++ experience writing games or other real-time graphical applications.

Second, I want to produce a larger set of tools that can be used to turn a Fruit Jam combined with an appropriate display into a video game console.  I currently don't think "cartridge" style game loading on microSD cards is going to work very well, as the RP2350 doesn't seem to be able to run code directly from PSRAM and has fairly limited SRAM, but 16MB of Flash is enough to hold a handful of small games combined with a game loader or one professional size handheld console game.  (GBA games typically ranged in size from 8MB to 32MB, and many of the most popular GBA games were around 16MB.)  Since the Fruit Jam doesn't have a built in gamepad controller, part of this will include C USB gamepad drivers, starting with [Adafruit's SNES clone controller](http://adafru.it/6285).

Third, I want to turn some of this into CircuitPython modules.  CircuitPython already has support for USB gamepads and for audio playback, so this is mainly going to focus on graphics library.  This won't make CircuitPython suitable for higher performance games, as it is still much slower and bulkier than pure C, but it should allow for significantly higher framerates than are achievable with `displayio`, making CircuitPython on Fruit Jam as well as Adafruit's Fruit Jam OS better suited to basic real-time games and other real-time graphical applications.



## WIP

Everything here should be treated a work in progress, unless noted otherwise.  I cannot guarantee that every commit will compile and run correctly, however I will try to not push to Github unless the current commit compiles and runs correctly.

Note that this means that as elements are completed, the structure of the repository make change significantly.  For example, I'm currently working on graphics, with the SPI driver, the ST7789 display controller driver, and a simple graphics library (SGL) all in the root directory of the repository.  Once this element is complete, I will probably move it out of the root directory and into a subdirectory before (or perhaps shortly after) starting on the next element.

Specifically the file `fjdos.c` is subject to frequent and extreme change, as it is the testing program for this entire project.  At the time of writing, it is testing nearly all of the graphics driver and library functions.  Once this element is complete, I'll probably remove nearly all of the graphics calls and only leave enough to provide feedback for whatever come next.  As such, `fjdos.c` won't be a perpetual example of how everything should be used.  As I do this, I will try to add documentation here for all of the functions of completed elements.

Note also that even the name of this repository is subject to change.  In fact, I _intend_ on eventually changing it, to avoid confusion with the official Adafruit Fruit Jam OS.

# Usage

## Compiling

This requires Pico SDK and arm-none-eabi to compile.  It assumes you are building in a Unix-like shell, like Bash.  I've provided a Makefile for starting the build process and for cleaning up the build tree.  It shouldn't be terribly difficult to setup your favorite IDE to handle the build process instead, if that's what you prefer.

You can find information on setting up Pico SDK on its [Github repository](https://github.com/raspberrypi/pico-sdk) and in the official ["Getting Started" PDF](https://rptl.io/pico-get-started).

I should also note: In `CMakeLists.txt`, I've set it to build as a "Release" version (for performance profiling).  This will likely cause significant problems with debuggers, since it does not preserve debug information and optimizes in ways that can make tracking down bugs very difficult.  If you need to use a debugger (or are using an IDE with built a built in debugger), you may want to comment out that line or change it to "Debug", to disable optimizations and retain debug information.


## Library Documentation

As various elements of this project become mature, I'll add documentation for them, including notes, usage information, and anything else that might be useful to know.

### SPI1

### ST7789

### SGL

#### SGL\_DRAW

#### SGL\_IMG



