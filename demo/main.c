#include <rndmraster.h>

float vertices[12] = {
	0.20, 0.20,
	0.20, 0.80,
	0.80, 0.80,
	0.80, 0.80,
	0.80, 0.20,
	0.20, 0.20
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
