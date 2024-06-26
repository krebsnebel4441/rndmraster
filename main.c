#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Vectors are generally represented as an array with a length equal to the dimension

// Function to determine wheter a pixel is on a specific side of an edge
float edgefunc(int vertex[2], int corner[2], float pixel[2]);

// The precalculated relative position of the surrounding points of the triangle on the screen
const float vertices[3][5] = {
	{0.25, 0.25, 1.0, 0.0, 0.0},
	{0.50, 0.75, 0.0, 1.0, 0.0},
	{0.75, 0.25, 0.0, 0.0, 1.0},
};

int main(int argc, char * argv[]) {
	// The dimensions of the image
	int imagewidth = 800;
	int imageheight = 600;
	// A buffer large enough to hold all the pixel data
	uint32_t * fb = malloc(sizeof(uint32_t)*imagewidth*imageheight);
	memset(fb, 0x0, sizeof(uint32_t)*imagewidth*imageheight);

	// For holding the absolute positions of the points on screen
	int verts[3][2];
	// Containing the three edges of the triangle
	for (int i = 0; i < 3; i++) {
		verts[i][0] = (int) (vertices[i][0] * imagewidth);
		verts[i][1] = (int) (vertices[i][1] * imageheight);
	}

	// In order to build up a bounding box of the triangle
	int minx = INT32_MAX, miny = INT32_MAX, maxx = 0, maxy = 0;
	for (int i = 0; i < 3; i++) {
		minx = verts[i][0] < minx ? verts[i][0] : minx;
		miny = verts[i][1] < miny ? verts[i][1] : miny;
		maxx = verts[i][0] > maxx ? verts[i][0] : maxx;
		maxy = verts[i][1] > maxy ? verts[i][1] : maxy;
	}
	if (minx < 0) minx = 0;
	if (miny < 0) miny = 0;
	if (maxx > imagewidth) maxx = imagewidth;
	if (maxy > imageheight) maxy = imageheight;
	// Rendering of the triangle
	for (int y = miny; y < maxy; y++) {
		for (int x = minx; x < maxx; x++) {
			float pixel[2];
			pixel[0] = x + 0.5;
			pixel[1] = y + 0.5;
			// Calculating the determinants which basically just checks whether
			// the pixel is to a side of the edge
			float det0 = edgefunc(verts[0], verts[1], pixel);
			float det1 = edgefunc(verts[1], verts[2], pixel);
			float det2 = edgefunc(verts[2], verts[0], pixel);
			// Combining all determinants it is checked whether all determinants are
			// greater or equal than 0. If so the color is set to red
			if (det0 >= 0.0 && det1 >= 0.0 && det2 >= 0.0) {
				float v2[2];
				v2[0] = verts[2][0]; v2[1] = verts[2][1];
				float area = edgefunc(verts[0], verts[1], v2);
				float l0 = det1 / area;
				float l1 = det2 / area;
				float l2 = det0 / area;
				float rb = (l0 * vertices[0][2] + l1 * vertices[1][2] + l2 * vertices[2][2]);
				float gb = (l0 * vertices[0][3] + l1 * vertices[1][3] + l2 * vertices[2][3]);
				float bb = (l0 * vertices[0][4] + l1 * vertices[1][4] + l2 * vertices[2][4]);
				int r = ((int) (rb * 255.0)) << 16;
				int g = ((int) (gb * 255.0)) << 8;
				int b = ((int) (bb * 255.0));
				fb[y * imagewidth + x] = r + g + b;
			}
		}
	}

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

float edgefunc(int vertex[2], int corner[2], float pixel[2]) {
	return (pixel[0] - vertex[0]) * (corner[1] - vertex[1]) - (pixel[1] - vertex[1]) * (corner[0] - vertex[0]);
}
