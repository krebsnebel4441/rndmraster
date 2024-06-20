#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[]) {
	// The dimensions of the image
	int imagewidth = 800;
	int imageheight = 600;
	// A buffer large enough to hold all the pixel data
	uint32_t * fb = malloc(sizeof(uint32_t)*imagewidth*imageheight);
	memset(fb, 0x0, sizeof(uint32_t)*imagewidth*imageheight);
	printf("P3\n%d %d\n255\n", imagewidth, imageheight);
	for (size_t y = imageheight; y > 0; y--) {
		for (size_t x = 0; x < imagewidth; x++) {
			uint32_t val = fb[(y-1) * imagewidth + x];
			uint32_t r = (val & 0xff0000) >> 16;
			uint32_t g = (val & 0x00ff00) >> 8;
			uint32_t b = val & 0x0000ff;
			printf("%d %d %d\n", r, g, b);
		}
	}
	return 0;
}
