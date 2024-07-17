#include <rndmraster.h>
#include <stdlib.h>
#include <stdio.h>

void project(struct context * ctx, struct vertexshader * shader,
                float * vertices, unsigned int * coords) {
	for (int i = 0; i < 3; i++) {
	        float near = shader->near;
	        float x = vertices[3*i] * near / vertices[3*i+2];
	        float y = vertices[3*i+1] * near / vertices[3*i+2];
	        coords[2*i] = ((unsigned int) ((x + 1.0) * ctx->imagewidth) / 2);
	        coords[2*i+1] = ((unsigned int) ((y + 1.0) * ctx->imageheight) / 2);
	}
}

float edgefunc(int vertex[2], int corner[2], float pixel[2]) {
	return (pixel[0] - vertex[0]) * (corner[1] - vertex[1]) - (pixel[1] - vertex[1]) * (corner[0] - vertex[0]);
}

void doraster(struct context * ctx, struct fragmentshader * fragment,
                float * vertices, unsigned int * coords,
                int minx, int miny, int maxx, int maxy) {
        for (int y = miny; y < maxy; y++) {
                for (int x = minx; x < maxx; x++) {
                        float pixel[2];
                        pixel[0] = x + 0.5;
                        pixel[1] = y + 0.5;
                        // Calculating the determinants which basically just checks whether
                        // the pixel is to a side of the edge
                        float det0 = edgefunc(&coords[0], &coords[2], pixel);
                        float det1 = edgefunc(&coords[2], &coords[4], pixel);
                        float det2 = edgefunc(&coords[4], &coords[0], pixel);
                        float v2[2];
                        v2[0] = coords[4]; v2[1] = coords[5];
                        float area = edgefunc(&coords[0], &coords[2], v2);
                        float l0 = det1 / area;
                        float l1 = det2 / area;
                        float l2 = det0 / area;
                        float z = l0 * vertices[2] + l1 * vertices[5] + l2 * vertices[8];
                        // Combining all determinants it is checked whether all determinants are
                        // greater or equal than 0. If so the color is set to red
                        if (det0 >= 0.0 && det1 >= 0.0 && det2 >= 0.0 && ctx->zbuf[y*ctx->imagewidth+x] <= -z) {
				ctx->zbuf[y*ctx->imagewidth+x] = -z;
				int r, g, b;
				switch(fragment->colorization) {
				case INTERPOLATE:
					float * colors = fragment->data.colors;
	                                float rb = (l0 * colors[0] + l1 * colors[3] + l2 * colors[6]);
	                                float gb = (l0 * colors[1] + l1 * colors[4] + l2 * colors[7]);
	                                float bb = (l0 * colors[2] + l1 * colors[5] + l2 * colors[8]);
					r = ((int) (rb * 255.0)) << 16;
					g = ((int) (gb * 255.0)) << 8;
					b = ((int) (bb * 255.0));
					break;
				case FILL:
					float * color = fragment->data.color;
					r = ((int) (color[0] * 255.0)) << 16;
					g = ((int) (color[1] * 255.0)) << 8;
					b = ((int) (color[2] * 255.0));
					break;
				}
				ctx->buffer[y*ctx->imagewidth+x] = r + g + b;
			}
		}
	}
}

void drawtriangles(struct context * context, float * vertices, int count,
		struct vertexshader * vertex, struct fragmentshader * fragment) {
	unsigned int * coords = alloca(2 * 3 * sizeof(unsigned int));
	for (int c = 0; c < count; c++) {
		project(context, vertex, &vertices[9*c], coords);
		int minx = INT32_MAX, miny = INT32_MAX, maxx = 0, maxy = 0;
		for (int i = 0; i < 3; i++) {
			minx = coords[2*i] < minx ? coords[2*i] : minx;
			miny = coords[2*i+1] < miny ? coords[2*i+1] : miny;
			maxx = coords[2*i] > maxx ? coords[2*i] : maxx;
			maxy = coords[2*i+1] > maxy ? coords[2*i+1] : maxy;
               }
	       if (minx < 0) minx = 0;
	       if (miny < 0) miny = 0;
	       if (maxx > context->imagewidth) maxx = context->imagewidth;
	       if (maxy > context->imageheight) maxy = context->imageheight;
	       doraster(context, fragment, &vertices[3*c], coords, minx, miny, maxx, maxy);
	}
}
