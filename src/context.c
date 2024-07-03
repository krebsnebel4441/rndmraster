#include <stdlib.h>
#include <string.h>
#include <rndmraster.h>

static void draw_ppm(struct context * context) {
	struct ppm_context * c = (struct ppm_context *) context;
	fprintf(c->file, "P3\n%d %d\n255\n", c->imagewidth, c->imageheight);
	for (int y = c->imageheight; y > 0; y--) {
		for (int x = 0; x < c->imagewidth; x++) {
			uint32_t val = c->buffer[(y-1) * c->imagewidth + x];
			uint32_t r = (val & 0xff0000) >> 16;
			uint32_t g = (val & 0x00ff00) >> 8;
			uint32_t b = val & 0x0000ff;
			fprintf(c->file, "%d %d %d\n", r, g, b);
		}
	}
}

struct ppm_context init_ppm(int width, int height, char * filename) {
	uint32_t * buffer = malloc(sizeof(uint32_t) * width * height);
	float * zbuffer = malloc(sizeof(float) * width * height);
	memset(zbuffer, 0xfe, sizeof(float) * width * height);
	memset(buffer, 0x0, sizeof(uint32_t) * width * height);
	FILE * dest = fopen(filename, "w");
	struct ppm_context context = {
		.imagewidth = width,
		.imageheight = height,
		.draw = &draw_ppm,
		.buffer = buffer,
		.zbuf = zbuffer,
		.file = dest,
	};
	return context;
}

void display(struct context * context) {
	context->draw(context);
}
