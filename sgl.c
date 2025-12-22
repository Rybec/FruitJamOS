/* Small Graphics Library */

/* Designed for real-time rendering
on microcontrollers.  Uses patterns
from SDL, adapted to low resource
platforms. */



/*
	Options?:

		Draw surface directly to display (unbuffered)
			Do we need this, or should user call driver
			functions directly for this?  Surface object
			should contain dimension data, color depth,
			and a buffer.  User could use dim and buffer
			to do unbuffered draw through driver.  This
			does not use DMA and is thus slow.

		Driver needs to contain framebuffer, for DMA.
			Driver function for initializing framebuffer.
			Sets up DMA.  Framebuffer treated as normal
			surface, and "flip()" function activates DMA
			write?  Maybe this module wraps framebuffer
			in Surface object.  This means Surface struct
			has width, height, and buffer pointer.



	Get graphics surface

	Data needed from driver:
		screen size
		color depth

	What this should not provide
	access to:
		Special functions from driver
			Backlight brightness
			rotation change
			(Maybe callback for rotation change, to
			update screen size.)


*/
