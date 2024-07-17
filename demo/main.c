#include <rndmraster.h>

float vertices[18] = {
	-0.5, -0.5, 0.09,
	 0.0,  0.5, 0.09,
	 0.5, -0.5, 0.09,
	 0.0,  0.0, 0.20,
	 0.4,  0.6, 0.20,
	 0.8,  0.0, 0.20
};

static struct vertexshader vertex = {
	.near = 0.1,
};

static struct fragmentshader fragment = {
	.colorization = INTERPOLATE,
	.data = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	}
};

int main() {
	int imagewidth = 800, imageheight = 600;
	char filename[] = "image.ppm";
	struct ppm_context context = init_ppm(imagewidth, imageheight, filename);
	drawtriangles((struct context *)&context, vertices, 2, &vertex, &fragment);
	display((struct context *)&context);
	return 0;
}
