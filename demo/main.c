#include <rndmraster.h>

float vertices[18] = {
	0.25, 0.25, 0.1,
	0.50, 0.75, 0.1,
	0.75, 0.25, 0.1,
	0.25, 0.15, 0.4,
	0.60, 0.85, 0.4,
	0.95, 0.15, 0.4
};

float colors[18] = {
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
};

int main() {
	int imagewidth = 800, imageheight = 600;
	char filename[] = "image.ppm";
	struct ppm_context context = init_ppm(imagewidth, imageheight, filename);
	drawtriangles((struct context *)&context, vertices, colors, 2);
	display((struct context *)&context);
	return 0;
}
