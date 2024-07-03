#include <rndmraster.h>

float edgefunc(int vertex[2], int corner[2], float pixel[2]) {
	return (pixel[0] - vertex[0]) * (corner[1] - vertex[1]) - (pixel[1] - vertex[1]) * (corner[0] - vertex[0]);
}

void drawtriangles(struct context * context, float * vertices, float * colors, int count) {
	int imagewidth = context->imagewidth;
	int imageheight = context->imageheight;
	uint32_t * fb = context->buffer;
	float * zbuf = context->zbuf;
	for (int c = 0; c < count; c++) {
		int verts[3][2];
		// Containing the three edges of the triangle
		for (int i = 0; i < 3; i++) {
			verts[i][0] = (int) (vertices[9*c + 3*i + 0] * imagewidth);
			verts[i][1] = (int) (vertices[9*c + 3*i + 1] * imageheight);
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
				float v2[2];
				v2[0] = verts[2][0]; v2[1] = verts[2][1];
				float area = edgefunc(verts[0], verts[1], v2);
				float l0 = det1 / area;
				float l1 = det2 / area;
				float l2 = det0 / area;
				float z = l0 * vertices[9*c+2] + l1 * vertices[9*c+5] + l2 * vertices[9*c+8];
				// Combining all determinants it is checked whether all determinants are
				// greater or equal than 0. If so the color is set to red
				if (det0 >= 0.0 && det1 >= 0.0 && det2 >= 0.0 && zbuf[y*imagewidth+x] <= -z) {
					float rb = (l0 * colors[c*9+0] + l1 * colors[c*9+3] + l2 * colors[c*9+6]);
					float gb = (l0 * colors[c*9+1] + l1 * colors[c*9+4] + l2 * colors[c*9+7]);
					float bb = (l0 * colors[c*9+2] + l1 * colors[c*9+5] + l2 * colors[c*9+8]);
					int r = ((int) (rb * 255.0)) << 16;
					int g = ((int) (gb * 255.0)) << 8;
					int b = ((int) (bb * 255.0));
					zbuf[y*imagewidth+x] = -z;
					fb[y * imagewidth + x] = r + g + b;
				}
			}
		}
	}
}
