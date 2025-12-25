import os
import sys
import warnings
warnings.filterwarnings("ignore")

os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = '1'
import pygame


if (len(sys.argv) < 2):
	print("Usage:")
	print("\tpython img_to_c.py <filename>")
	print()
	print("\tConverts image file to 16 bit C array in")
	print("\tsurface struct designed for use with SGL.")
	print()

	sys.exit()


def convert(r, g, b, a):
	r = r >> 3
	g = g >> 2
	b = b >> 3
	if (a == 0):
		r = 0
		g = 1
		b = 0
	elif (r == 0 and g == 1 and b == 0):
		g = 2

	return r << 11 | g << 5 | b



image = pygame.image.load(sys.argv[1])
pixels = pygame.PixelArray(image)

size = image.get_size()


#typedef struct {
#	uint16_t width,
#	uint16_t height,
#	uint16_t data[]
#} SGL_surface;



#print("SGL_surface {}")

#typedef struct {
#	uint16_t width,
#	uint16_t height,
#	uint16_t data[]
#} SGL_surface;

var_name = os.path.splitext(sys.argv[1])[0].replace(" ", "_")

print("\n\n")
print("SGL_surface {} = {{".format(var_name))
print("\t{},".format(size[0]))
print("\t{},".format(size[1]))
print("\t{")

for y in range(size[1]):
	print("\t\t", end="")
	num = 1
	for x in range(size[0] - 1):
		color = convert(*image.unmap_rgb(pixels[x, y]))

		print("0x{:04x}".format(color), end=",")

		if (num == 0):
			print("\n\t\t", end="")
		else:
			print(" ", end="")

		num = (num + 1) % 8


	color = convert(*image.unmap_rgb(pixels[-1, y]))
	if y != size[1] - 1:
		print("0x{:04x}".format(color), end=",\n\n")
	else:
		print("0x{:04x}".format(color), end="\n")
print("\t}")
print("};")

sys.stderr.write("\nextern SGL_surface {};\n".format(var_name))


